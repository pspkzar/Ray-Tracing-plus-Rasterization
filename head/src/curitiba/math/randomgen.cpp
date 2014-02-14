#include <curitiba/math/randomgen.h>

#include "mtrand.h"

struct curitiba::math::RandImpl {
  MTRand gen;
};

// Default constructor. Initializes generator with default seed
curitiba::math::RandomGenerator::RandomGenerator ():
  m_pRandomGen (new RandImpl)
{
}
  
// Destructor
curitiba::math::RandomGenerator::~RandomGenerator()
{
  delete m_pRandomGen;
}
  
// Get a random integer in the closed interval [MIN,MAX]
int 
curitiba::math::RandomGenerator::randomInt (int Min, int Max)
{
  unsigned int interval = Max - Min;
  return (Min + m_pRandomGen->gen.randInt (interval));
}

// Overloaded () operator to allow the object to be used as a Random
// Number Generator in STL algorithms. 
// Returns a random integer between 0 and N
  
unsigned int 
curitiba::math::RandomGenerator::operator () (unsigned int n)
{
  return randomInt (0, n-1);
}
