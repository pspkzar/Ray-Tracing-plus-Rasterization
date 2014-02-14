#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <string>
#include <vector>
#include <map>

#include <curitiba/resource/font.h>
#include <curitiba/material/materialid.h>

using namespace curitiba::material;
using namespace curitiba::resource;

namespace curitiba 
{
	namespace resource
	{

		class FontManager {

		public:


			static void addFont(std::string fontName, std::string fontDefFile, std::string fontImageFile);
			static bool hasFont(std::string fontName);
			static const Font &getFont(std::string fontName);

		protected:

			FontManager();
			~FontManager();
			static std::map<std::string,Font> mFonts;
		};
	};
};


#endif