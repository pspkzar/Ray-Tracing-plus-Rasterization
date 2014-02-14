#ifndef MESHBONES_H
#define MESHBONES_H

#include <curitiba/geometry/mesh.h>
#include <curitiba/render/irenderer.h>
#include <curitiba/render/irenderable.h>
#include <curitiba/material/imaterialgroup.h>


#include <string>

namespace curitiba
{
	namespace geometry
	{
		class MeshBones : public Mesh
		{
		protected:
			// the weight for a bone
			typedef std::pair<unsigned int, float> BoneWeight;
			// the weights for all bones assigned to a vertex
			typedef std::vector<BoneWeight> BoneWeights;
			// the bone assignements for each vertex
			typedef std::map<unsigned int, BoneWeights> BoneAssignments;

			BoneAssignments m_BoneAssignments;

			MeshBones(void);

		public:

			friend class curitiba::resource::ResourceManager;
			~MeshBones (void);

			void addBoneWeight(unsigned int vertex, unsigned int bone, float weight);
			virtual std::string getType (void);
		};
	};
};

#endif
