#include <curitiba/loader/passloader.h>

#include <tinyxml.h>

#include <curitiba.h>
#include <curitiba/render/pipeline.h>
#include <curitiba/render/pass.h>

using namespace curitiba::loader;
using namespace curitiba::render;

bool
PassLoader::load (std::string file)
{
	TiXmlDocument doc (file.c_str());
	bool loadOkay = doc.LoadFile();

	if (true == loadOkay) {
		TiXmlHandle hDoc (&doc);
		TiXmlElement *pElem;
		TiXmlHandle hRoot (0);
		TiXmlHandle handle (0);

		{ //root
			pElem = hDoc.FirstChildElement().Element();
			if (0 == pElem) {
				return false;
			}
			hRoot = TiXmlHandle (pElem);
		}
		
		{ //pipeline
		}
	}
	return loadOkay;
}
