#ifndef TEXTUTIL_H
#define TEXTUTIL_H

/**
 * \file   textutil.h
 * \author  <pangelo@doublemv.com>
 * \date   Sat Jun 28 18:48:30 2008
 * 
 * \brief  Misc utility functions for dealing with text
 * 
 * 
 */

#include <string>
#include <climits>
#include <cmath>

namespace curitiba {

  namespace system {

    namespace textutil {
      
      // Trims whitespace from the beginning and end of a string
      std::string TrimWhitespace (const std::string &InputString);

      // Create a random filename useful for temp files. Size is the size of the filename without the extension 
      std::string CreateRandomFilename (unsigned int Size = 8, const std::string Extension = "txt");

      // Parse an integer value from a string
      int ParseInt (const std::string &InputString);

      // Parse a float value from a string
      float ParseFloat (const std::string &InputString);

      // Create a string from an integer value
      std::string ToString (int Value);

      // Create a string from a float value
      std::string ToString (float Value);

      // Parse error constants
      const int ParseIntError = INT_MAX;
      const float ParseFloatError = (float)HUGE_VAL;

	  float * ParseFloats (const std::string &InputString, int count);
	  int *ParseInts (const std::string &InputString, int count);


    } // namespace textutil

  } // namespace system

} // namespace curitiba

#endif // TEXTUTIL_H
