#ifndef IRENDERABLE_H
#define IRENDERABLE_H

//#include <curitiba/render/irenderer.h>
#include <curitiba/material/imaterialgroup.h>
#include <curitiba/render/vertexdata.h>
#include <curitiba/math/vec3.h>
#include <vector>
#include <set>

#include <curitiba/event/ilistener.h>

using namespace curitiba::event_;

namespace curitiba
{
	namespace render 
	{
		class IRenderable: public IListener 
		{
		public:

			virtual void setName (std::string name) = 0;
			virtual std::string& getName () = 0;
			virtual unsigned int getDrawingPrimitive() = 0; 
			virtual unsigned int getRealDrawingPrimitive() = 0;
			virtual void setDrawingPrimitive(unsigned int aDrawingPrimitive) = 0;

			virtual void prepareTriangleIDs(unsigned int sceneObjectID) = 0;
			virtual void unitize(float min, float max) = 0;
			virtual void createUnifiedIndexVector() = 0;

			virtual curitiba::render::VertexData& getVertexData (void) = 0;
			virtual std::vector<curitiba::material::IMaterialGroup*>& 
												getMaterialGroups (void) = 0;

			virtual void getMaterialNames(std::set<std::string> *nameList) = 0;
			virtual void addMaterialGroup (curitiba::material::IMaterialGroup*) = 0;
			virtual void addMaterialGroup (curitiba::material::IMaterialGroup* materialGroup, 
				curitiba::render::IRenderable *aRenderable) = 0; 
			virtual int getNumberOfVertices (void) = 0;
			virtual int getNumberOfPrimitives(void) = 0;
			virtual std::string getType (void) = 0;
			virtual void resetCompilationFlags() = 0;

			virtual ~IRenderable(void) {};
		};
	};
};

#endif //IRENDERABLE_H

