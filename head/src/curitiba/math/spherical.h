#ifndef SPHERICAL_H
#define SPHERICAL_H

#include <curitiba/math/vec3.h>

//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>

namespace curitiba
{
	namespace math
	{

		//! \brief This class defines spherical coordinates.
		//!
		//!	
		class spherical {
		 public:
		  		  
		   
			/// \name Constructors
			//@{
		   
			//! Default class constructor. Initialize vector to (0,0,0,0)
			explicit spherical (): m_Alpha(0.0f), m_Beta(0.0f) {};
		   
			//! Complete class constructor
			explicit spherical (float alpha, float beta): m_Alpha(alpha), m_Beta(beta) {};
		   
			//! Receives Cartesian and creates spherical
			spherical (float x, float y, float z);

			//! Copy constructor. 
			spherical (const spherical &v);
		   
			//! Class destructor 
			~spherical () {};								
			
			//@}
		   
			//! \name Vector Accessing and Copying
			//@{

			float alpha();
			float beta();
			void setAlpha(float alpha);
			void setBeta(float beta);
		   
			//! Make this vector a copy of the vector <i>v</i>
			void copy (const spherical &v);

			const vec3 toCartesian();
			// returns a vector in the XZ plane perpendicular to me
			const vec3 getRightVector();
			const vec3 getNaturalUpVector();

			//! Return a new vector with the same contents a this vector
			spherical * clone () const;

			//! Initialize ou change the vector's components
			void set (float alpha,float beta);	
			void set (float x, float y, float z);

		   	void cap(); 

			// Assignment operator
			const spherical& operator = (const spherical &v);
		   
			//@}
		   
			//! \name Overloaded comparation
			//@{
			
			//! Equal
			bool operator == (const spherical &v) const;
			
			//! Diferent
			bool operator != (const spherical &v) const;

			//@}

			//! \name Methods
			//@{
		   
			//! Interpolated vector between this vector and vector <i>v</i> at
			//! position alpha (0.0 <= alpha <= 1.0)
			const spherical lerp (const spherical &v, float alpha) const;   
		   
			//! Vector Equality
			bool equals (const spherical &v, float tolerance=-1.0f) const;
		   
			//@}			
			
		protected:
			//! alpha beta vector components 
			// beta measures the elevation angle from the ZX plane
			// ranges between -90 and 90 in radians
			// alfa measures the angle in the ZX plane where Z = 0, and X = PI/4
			float m_Alpha,m_Beta;							


		};
	};
};
#endif // spherical_H
