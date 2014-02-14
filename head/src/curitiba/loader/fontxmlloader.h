#ifndef FONTXMLLOADER_H
#define FONTXMLLOADER_H

#include <curitiba/resource/font.h>

using namespace curitiba::resource;

namespace curitiba 
{

	namespace loader 
	{
		class FontXMLLoader
		{
		public:
			static void loadFont (Font *aFont, std::string &aFilename);

		private:
			FontXMLLoader(void) {};
			~FontXMLLoader(void) {};
		};
	};
};

#endif //FONTXMLLOADER_H
