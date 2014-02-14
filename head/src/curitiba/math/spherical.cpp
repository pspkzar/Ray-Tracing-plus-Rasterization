#include <curitiba/math/spherical.h>
#include <curitiba/math/utils.h>

using namespace curitiba::math;


// Tolerance factor to prevent division by 0
static const double tol = FLT_EPSILON;


// Private copy constructor
spherical::spherical(const spherical &v) : m_Alpha(v.m_Alpha), m_Beta(v.m_Beta) {}

// construct from cartesian coordinates
spherical::spherical(float x, float y, float z) {

	set(x,y,z);
}

float 
spherical::alpha() 
{
	return m_Alpha;
}

float 
spherical::beta()
{
	return m_Beta;	
}

// Private assignment operator
const spherical& 
spherical::operator =(const spherical &v) 
{
	if (this != &v) {
		this->m_Alpha = v.m_Alpha;
		this->m_Beta = v.m_Beta;
	}
	return *this;
}



// Make this vector a copy the vector <i>v</i>
void 
spherical::copy (const spherical &v)
{
	m_Alpha = v.m_Alpha;
	m_Beta = v.m_Beta;
}
   
// Return a new vector with the same contents a this vector
spherical * 
spherical::clone () const
{
	return new spherical(*this);
}

// Initialize ou change the vector's components
void 
spherical::set(float alpha,float beta) 
{
	m_Alpha = alpha;
	setBeta(beta);
}

void
spherical::setAlpha(float alpha)
{
	m_Alpha = alpha;
}

void
spherical::setBeta(float beta)
{
	m_Beta = fmod(beta, (float)(M_PI * 2.0f));
	if (m_Beta > M_PI)
		m_Beta = m_Beta - (float)(2.0f * M_PI);
	if (m_Beta < -M_PI)
		m_Beta = (float)(2.0f * M_PI) + m_Beta;
	if (m_Beta > M_PI || m_Beta < -M_PI)
		int x = 0;
}
   
void
spherical::set(float x, float y ,float z) 
{
	vec3 aux(x,y,z);
	aux.normalize();

	m_Beta = asin(aux.y);

	if (aux.z >= 0)
		m_Alpha = asin(aux.x / sqrt(aux.x*aux.x + aux.z*aux.z));
	else
		m_Alpha = (float)M_PI - asin(aux.x / sqrt(aux.x*aux.x + aux.z*aux.z));
}


const vec3 
spherical::toCartesian()
{
	vec3 v;
	float beta_aux, alpha_aux;

	if (m_Beta > M_PI * 0.5f) {
		beta_aux = (float)M_PI - m_Beta;
		alpha_aux = m_Alpha - (float)M_PI;
	}
	else if (m_Beta < -(float)M_PI * 0.5f) {
	
		beta_aux = (float)M_PI - m_Beta;
		alpha_aux = m_Alpha - (float)M_PI; 
	}
	else {
		beta_aux = m_Beta;
		alpha_aux = m_Alpha;
	}


	v.x = cos(beta_aux) * sin(alpha_aux);
	v.z = cos(beta_aux) * cos(alpha_aux);
	v.y = sin(beta_aux);

	return v;
}

const vec3
spherical::getRightVector() 
{
	float alpha_aux;
	vec3 v;
	if (m_Beta > M_PI * 0.5f) {
	
		alpha_aux = m_Alpha - (float)M_PI * 0.5f;
	}
	else if  (m_Beta < -M_PI * 0.5f) {
	
		alpha_aux = m_Alpha - (float)M_PI * 0.5f;
	}
	else {
		alpha_aux = m_Alpha - (float)M_PI * 0.5f;
	}

	v.x = cos(0.0f) * sin(alpha_aux);
	v.z = cos(0.0f) * cos(alpha_aux);
	v.y = sin(0.0f);

	return v;
}

const vec3
spherical::getNaturalUpVector() 
{
	float alpha_aux, beta_aux;
	vec3 v;
	// 2nd quadrant
	if (m_Beta > M_PI * 0.5f) {
	
		alpha_aux = m_Alpha;
		beta_aux = (float)M_PI - m_Beta;

	}
	// 3rd quadrant
	else if (m_Beta < -M_PI * 0.5f) {
	
		alpha_aux = m_Alpha  - (float)M_PI;
		beta_aux = (float)M_PI - m_Beta;

	} 
	// 1st quadrant
	else if ( m_Beta >= 0.0f) {

		alpha_aux = m_Alpha - (float)M_PI;
		beta_aux = m_Beta;
	}
	// 4th quadrant
	else {
		alpha_aux = m_Alpha;
		beta_aux = m_Beta;
	}

	v.x = (float)cos(M_PI * 0.5f - fabs(beta_aux)) * sin(alpha_aux);
	v.z = (float)cos(M_PI * 0.5f - fabs(beta_aux)) * cos(alpha_aux);
	v.y = (float)sin(M_PI * 0.5f - fabs(beta_aux));

	return v;

	//if (m_ElevationAngle >= 0)
	//	alpha = m_ZXAngle - M_PI;
	//else 
	//	alpha = m_ZXAngle;
	//m_UpVector.x = cos( float(M_PI * 0.5 - fabs(m_ElevationAngle)) ) * sin( alpha );
	//m_UpVector.z = cos( float(M_PI * 0.5 - fabs(m_ElevationAngle)) ) * cos( alpha );
	//m_UpVector.y = sin( float(M_PI * 0.5 - fabs(m_ElevationAngle)) );
}

// Equal
bool 
spherical::operator == (const spherical &v) const
{
	return equals(v);
}
	
//! Diferent
bool 
spherical::operator != (const spherical &v) const
{
	return !equals(v);
}

void 
spherical::cap() 
{
	if (m_Beta > M_PI * 0.48)
		m_Beta = (float)M_PI * 0.48f;
	else if (m_Beta < -M_PI * 0.48)
		m_Beta = - (float)M_PI * 0.48f;
}
   

// Interpolated vector between this vector and vector <i>v</i> at
// position factor (0.0 <= factor <= 1.0)
const spherical 
spherical::lerp (const spherical &v, float factor) const {
	spherical result;

	float ialpha = m_Alpha + ((v.m_Alpha - m_Alpha) * factor);
	float ibeta = m_Beta + ((v.m_Beta - m_Beta) * factor);

	result.set(ialpha, ibeta);

	return (result);
}

   
// Vector Equality
bool 
spherical::equals (const spherical &v, float tolerance) const {
	return (FloatEqual(m_Alpha, v.m_Alpha, tolerance) && 
		FloatEqual(m_Beta, v.m_Beta, tolerance));
}   
