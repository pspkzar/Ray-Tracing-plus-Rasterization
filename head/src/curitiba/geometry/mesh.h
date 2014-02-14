#ifndef MESH_H
#define MESH_H

#include <curitiba/render/irenderer.h>
#include <curitiba/render/irenderable.h>
#include <curitiba/material/imaterialgroup.h>

#include <curitiba/resource/resourcemanager.h>


#include <string>

namespace curitiba
{
	namespace geometry
	{
		class Mesh : public curitiba::render::IRenderable
		{
		protected:
			curitiba::render::VertexData* m_pVertexData;
			std::vector<curitiba::material::IMaterialGroup*> m_vMaterialGroups;
			unsigned int m_DrawPrimitive;
			unsigned int m_RealDrawPrimitive;
			std::string m_Name;
			int m_NumberOfPrimitives;
			std::vector<unsigned int> m_UnifiedIndex;

			void createUnifiedIndexVector();
			void prepareIndexData(); 
			void resetCompilationFlags();
			Mesh(void);

		public:
			friend class curitiba::resource::ResourceManager;

			static Mesh *createUnregisteredMesh();
			virtual ~Mesh (void);

			virtual void setName (std::string name);
			virtual std::string& getName (void);

			unsigned int getDrawingPrimitive(); 
			unsigned int getRealDrawingPrimitive();
			void setDrawingPrimitive(unsigned int aDrawingPrimitive);

			void prepareTriangleIDs(unsigned int sceneObjectID);

			void unitize(float min, float max);

			void addMaterialGroup (curitiba::material::IMaterialGroup*);
			void addMaterialGroup (curitiba::material::IMaterialGroup* materialGroup, 
				curitiba::render::IRenderable *aRenderable); 
			std::vector<curitiba::material::IMaterialGroup*>& getMaterialGroups (void);
			virtual void getMaterialNames(std::set<std::string> *nameList);

			void merge (curitiba::render::IRenderable *aRenderable);

			virtual curitiba::render::VertexData& getVertexData (void);
			virtual int getNumberOfVertices (void);
			virtual int getNumberOfPrimitives(void);
			int getPrimitiveOffset(void);

			virtual std::string getType (void);
		};
	};
};

#endif
