#include <optix_device.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
#include <optixu/optixu_aabb.h>
#include <optixu/optixu_aabb_namespace.h>

using namespace optix;


#define SQRT_SAMPLES 10

#define OCC_DIST 0.5

#define OCC_INTEN 1.6f

struct PerRayDataResult
{
  int occlusion;
};


//rtDeclareVariable(float4, lightDir, , );

rtDeclareVariable(rtObject,      top_object, , );

rtBuffer<float4> vertex_buffer;     
rtBuffer<uint> index_buffer;

rtTextureSampler<float4,2> pos_buffer;
rtTextureSampler<float4,2> norm_buffer;

rtBuffer<float4,2> output0;


rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );
rtDeclareVariable(uint2, launch_dim,   rtLaunchDim, );
rtDeclareVariable(PerRayDataResult, prdr, rtPayload, );

__device__ inline float rand(float2 co){
	float intpart;
    return modf(sin(dot(co,make_float2( 12.9898, 78.233))) * 43758.5453,&intpart);
}

RT_PROGRAM void buffer_camera(void)
{

	float4 pos = tex2D( pos_buffer, launch_index.x, launch_index.y );
	float4 norm = tex2D( norm_buffer, launch_index.x, launch_index.y );
	PerRayDataResult prdr;	
	int result=SQRT_SAMPLES*SQRT_SAMPLES;
	prdr.occlusion = 0;
	float3 ffnormal =make_float3(norm);// faceforward(make_float3(norm),make_float3(pos),make_float3(norm));
	//if (pos.w > 0.0f) {	
		Onb onb(ffnormal);
		float3 ray_origin = make_float3(pos);
		float3 dir;
		float inv_sqrt_samples=1/float(SQRT_SAMPLES);
		for(int i=0; i<SQRT_SAMPLES; i++){
			for(int j=0; j<SQRT_SAMPLES; j++){
				int a=i+1, b=j+1;
				float rand1=rand(make_float2(pos.x*a,pos.y*b));
				float rand2=rand(make_float2(pos.x*b,pos.z*a));
				float rand3=rand(make_float2(pos.y*a,pos.z*b));
				float rand4=rand(make_float2(rand1*b,rand2*a));
				float rand5=rand(make_float2(rand3*a,rand4*b));

				float u1=(float(i)+rand5)*inv_sqrt_samples;
				float u2=(float(j)+rand4)*inv_sqrt_samples;
				cosine_sample_hemisphere(u1, u2,dir);
				onb.inverse_transform(dir);
				optix::Ray ray = optix::make_Ray(ray_origin, dir, 0, 0.0001f, OCC_DIST);
				rtTrace(top_object, ray, prdr);
				result-=prdr.occlusion;
			}
		}
		
	/*}
	else
		result=0;*/

		float4 out=make_float4(pow(float(result)/float(SQRT_SAMPLES*SQRT_SAMPLES),OCC_INTEN));
	//prdr.result.x = ray_origin.x/256.0; prdr.result.y = ray_origin.y/256.0; prdr.result.z = ray_origin.z/256.0; prdr.result.w = 1.0;
	output0[launch_index] = out;

}


RT_PROGRAM void any_hit_shadow()
{
	prdr.occlusion =  1;
	rtTerminateRay();
}


RT_PROGRAM void miss()
{
	prdr.occlusion=0;
}

RT_PROGRAM void exception(void)
{
	output0[launch_index] = make_float4(1.f, 0.f, 0.f, 1.f);
}


RT_PROGRAM void geometryintersection(int primIdx)
{

	float4 vecauxa = vertex_buffer[index_buffer[primIdx*3]];
	float4 vecauxb = vertex_buffer[index_buffer[primIdx*3+1]];
	float4 vecauxc = vertex_buffer[index_buffer[primIdx*3+2]];
//	float3 e1, e2, h, s, q;
//	float a,f,u,v,t;

	float3 v0 = make_float3(vecauxa);
	float3 v1 = make_float3(vecauxb);
	float3 v2 = make_float3(vecauxc);

  // Intersect ray with triangle
  float3 n;
  float  t, beta, gamma;
  if( intersect_triangle( ray, v0, v1, v2, n, t, beta, gamma ) ) {

    if(  rtPotentialIntersection( t ) ) {

 /*     float3 n0 = make_float3(normal[ index_buffer[primIdx*3]]);
      float3 n1 = make_float3(normal[ index_buffer[primIdx*3+1]]);
      float3 n2 = make_float3(normal[ index_buffer[primIdx*3+2]]);

	  float3 t0 = make_float3(texCoord0[ index_buffer[primIdx*3]]);
	  float3 t1 = make_float3(texCoord0[ index_buffer[primIdx*3+1]]);
	  float3 t2 = make_float3(texCoord0[ index_buffer[primIdx*3+2]]);

      shading_normal   = normalize( n0*(1.0f-beta-gamma) + n1*beta + n2*gamma );
	  texCoord =  t0*(1.0f-beta-gamma) + t1*beta + t2*gamma ;
      geometric_normal = normalize( n );*/

	  rtReportIntersection(0);
    }
  }
}


RT_PROGRAM void boundingbox(int primIdx, float result[6])
{
	float3 v0 = make_float3(vertex_buffer[index_buffer[primIdx*3]]);
	float3 v1 = make_float3(vertex_buffer[index_buffer[primIdx*3+1]]);
	float3 v2 = make_float3(vertex_buffer[index_buffer[primIdx*3+2]]);  
	
	const float  area = length(cross(v1-v0, v2-v0));

	optix::Aabb* aabb = (optix::Aabb*)result;

	if(area > 0.0f && !isinf(area)) {
		aabb->m_min = fminf( fminf( v0, v1), v2 );
		aabb->m_max = fmaxf( fmaxf( v0, v1), v2 );
	} 
	else {
	    aabb->invalidate();
	}
}
