#include <optix_device.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
#include <optixu/optixu_aabb.h>
#include <optixu/optixu_aabb_namespace.h>

using namespace optix;

#define COEF 1.1f
#define MAX_DEPTH 5

#define SQRT_SAMPLES 10
#define OCC_DIST 0.5
#define OCC_INTEN 1.6f

struct PerRayDataResult
{
	float3 radiance;
	int depth;
};

struct PerRayData_Shadow{
	float val;
	int hit;
};

rtDeclareVariable(float3,        eye, , );

rtDeclareVariable(float4, lightDir, , );
rtDeclareVariable(float3, lightColor, , );

rtDeclareVariable(rtObject,      top_object, , );

// Material
rtDeclareVariable(float4, diffuse, , );
rtDeclareVariable(int, texCount, , );
rtTextureSampler<float4, 2> tex0;

rtBuffer<float4> vertex_buffer;     
rtBuffer<uint> index_buffer;
rtBuffer<float4> normal;
rtBuffer<float4> texCoord0;

rtTextureSampler<float4,2> pos_buffer;
rtTextureSampler<float4,2> norm_buffer;
rtTextureSampler<float4,2> color_buffer;

rtBuffer<float4,2> output0;


rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );
rtDeclareVariable(uint2, launch_dim,   rtLaunchDim, );
rtDeclareVariable(PerRayDataResult, prdr, rtPayload, );
rtDeclareVariable(PerRayData_Shadow, prds, rtPayload, );

rtDeclareVariable(float3, texCoord, attribute texcoord, ); 
rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, ); 
rtDeclareVariable(float3, shading_normal, attribute shading_normal, ); 

rtDeclareVariable(int, Phong, , );
rtDeclareVariable(int, Shadow, , );

rtDeclareVariable(float, t_hit, rtIntersectionDistance, );

__device__ inline float rand(float2 co){
	float intpart;
    return modf(sin(dot(co,make_float2( 12.9898, 78.233))) * 43758.5453,&intpart);
}

RT_PROGRAM void buffer_camera(void)
{

	float4 pos = tex2D( pos_buffer, launch_index.x, launch_index.y );
	float4 norm = tex2D( norm_buffer, launch_index.x, launch_index.y );
	float4 color = tex2D( color_buffer, launch_index.x, launch_index.y );
	PerRayDataResult prdr;
	PerRayData_Shadow prds;
	
	float3 color_res=make_float3(0.0);
	
	if(color.x>0.0f && color.y==0.0f && color.z>0.0 && color.w==0.0f){
		float3 dir = make_float3(pos) - eye;
		dir=normalize(dir);
		
		float3 dir_reflect=reflect(dir,make_float3(norm));
		Ray reflected = make_Ray(make_float3(pos),dir_reflect,Phong,0.0001f,RT_DEFAULT_MAX);
		prdr.depth=0;
		rtTrace(top_object,reflected,prdr);
		float3 color_reflect = prdr.radiance;
		prdr.radiance=color_reflect;

		float3 dir_refract=make_float3(1.f);
		if(refract(dir_refract,dir,make_float3(norm),COEF)){
			Ray refracted = make_Ray(make_float3(pos),dir_refract,Phong,0.001f,RT_DEFAULT_MAX);
			prdr.depth=0;
			rtTrace(top_object,refracted,prdr);
			float3 color_refract=prdr.radiance;

			float coef=1-COEF*COEF*(1-pow(dot(make_float3(norm),-dir),2));

			color_res=(coef*color_refract)+((1-coef)*color_reflect);
		}
		else{
			color_res = prdr.radiance;
		}
		

	}
	else {
		color_res=make_float3(color);

		Ray shadow=make_Ray(make_float3(pos),make_float3(-lightDir),Shadow,0.0001f,RT_DEFAULT_MAX);
		rtTrace(top_object,shadow,prds);
		color_res*=prds.val;
	}
	
	
	if(SQRT_SAMPLES){
		int result=SQRT_SAMPLES*SQRT_SAMPLES;
		Onb onb(make_float3(norm));
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

				float3 occ_dir;
				cosine_sample_hemisphere(u1, u2,occ_dir);
				onb.inverse_transform(occ_dir);
				Ray occ_ray = make_Ray(make_float3(pos),occ_dir,Shadow,0.0001f,OCC_DIST);
				rtTrace(top_object,occ_ray,prds);
				result-=prds.hit;
			}
		}
	

		float occlusion_level=float(result)/float(SQRT_SAMPLES*SQRT_SAMPLES);
		occlusion_level=pow(occlusion_level,OCC_INTEN);
		color_res*=occlusion_level;
	}

	output0[launch_index]=make_float4(color_res,1.0);
}

RT_PROGRAM void any_hit_shadow(){
	prds.val=0.5;
	prds.hit=1;
	rtTerminateRay();
}

RT_PROGRAM void any_hit_shadow_glass(){
	prds.val*=0.9;
	prds.hit=0;
	rtIgnoreIntersection();
}

RT_PROGRAM void miss_shadow(){
	prds.val=1.0;
	prds.hit=0;
}

RT_PROGRAM void closest_hit(){
	float3 world_geo_normal=normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, geometric_normal));
	float3 world_shade_normal=normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
	float3 ffnormal=faceforward(world_shade_normal, -ray.direction, world_geo_normal);
	float3 position=t_hit*ray.direction+ray.origin;

	float intensity=max(dot(make_float3(-lightDir),ffnormal),0.0f);
	float3 lightInensityDiffuse=lightColor*intensity;
	float3 tmp_color=make_float3(1.f);

	if(texCount==0){
		tmp_color=make_float3(diffuse)*lightInensityDiffuse + (0.3*make_float3(diffuse));
	}
	else{
		tmp_color=(make_float3(diffuse)*lightInensityDiffuse+0.3)*make_float3(tex2D(tex0, texCoord.x, texCoord.y));
	}

	prds.val=1;
	Ray shadow=make_Ray(position,make_float3(-lightDir),Shadow,0.0001f,RT_DEFAULT_MAX);
	rtTrace(top_object,shadow,prds);
	tmp_color*=prds.val;

	if(SQRT_SAMPLES){
		int result=SQRT_SAMPLES*SQRT_SAMPLES;
		Onb onb(ffnormal);
		float inv_sqrt_samples=1/float(SQRT_SAMPLES);
		for(int i=0; i<SQRT_SAMPLES; i++){
			for(int j=0; j<SQRT_SAMPLES; j++){
				int a=i+1, b=j+1;
				float rand1=rand(make_float2(position.x*a,position.y*b));
				float rand2=rand(make_float2(position.x*b,position.z*a));
				float rand3=rand(make_float2(position.y*a,position.z*b));
				float rand4=rand(make_float2(rand1*b,rand2*a));
				float rand5=rand(make_float2(rand3*a,rand4*b));

				float u1=(float(i)+rand5)*inv_sqrt_samples;
				float u2=(float(j)+rand4)*inv_sqrt_samples;

				float3 occ_dir;
				cosine_sample_hemisphere(u1, u2,occ_dir);
				onb.inverse_transform(occ_dir);
				Ray occ_ray = make_Ray(position,occ_dir,Shadow,0.0001f,OCC_DIST);
				rtTrace(top_object,occ_ray,prds);
				result-=prds.hit;
			}
		}

		float occlusion_level=float(result)/float(SQRT_SAMPLES*SQRT_SAMPLES);
		occlusion_level=pow(occlusion_level,OCC_INTEN);
		tmp_color*=occlusion_level;
	}

	prdr.radiance=tmp_color;
}

RT_PROGRAM void closest_hit_glass(){
	if(prdr.depth<MAX_DEPTH){
		
		prdr.depth++;
		int current_depth=prdr.depth;

		float3 world_geo_normal=normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, geometric_normal));
		float3 world_shade_normal=normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
		float3 ffnormal=faceforward(world_shade_normal, -ray.direction, world_geo_normal);
		float3 position = ray.origin + t_hit * ray.direction;

		float3 dir_reflect=reflect(ray.direction,ffnormal);
		Ray refl=make_Ray(position,dir_reflect,Phong,0.0001, RT_DEFAULT_MAX);
		rtTrace(top_object,refl,prdr);
		float3 color_reflect=0.99*prdr.radiance;
		


		float3 dir_refract=make_float3(1.f);
		if(refract(dir_refract,ray.direction,ffnormal,COEF)){
			Ray refracted=make_Ray(position,dir_refract,Phong,0.0001f,RT_DEFAULT_MAX);
			prdr.depth=current_depth;
			rtTrace(top_object,refracted,prdr);
			float3 color_refract=0.99*prdr.radiance;

			float coef=1-COEF*COEF*(1-pow(dot(ffnormal,-ray.direction),2));
			prdr.radiance=color_reflect;//(coef*color_refract)+((1-coef)*color_reflect);
		}
	}
	else{
		prdr.radiance=make_float3(0.f);
	}
}

RT_PROGRAM void miss(){
	prdr.radiance=make_float3(0.0f);
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

      float3 n0 = make_float3(normal[ index_buffer[primIdx*3]]);
      float3 n1 = make_float3(normal[ index_buffer[primIdx*3+1]]);
      float3 n2 = make_float3(normal[ index_buffer[primIdx*3+2]]);

	  float3 t0 = make_float3(texCoord0[ index_buffer[primIdx*3]]);
	  float3 t1 = make_float3(texCoord0[ index_buffer[primIdx*3+1]]);
	  float3 t2 = make_float3(texCoord0[ index_buffer[primIdx*3+2]]);

      shading_normal   = normalize( n0*(1.0f-beta-gamma) + n1*beta + n2*gamma );
	  texCoord =  t0*(1.0f-beta-gamma) + t1*beta + t2*gamma ;
      geometric_normal = normalize( n );

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
