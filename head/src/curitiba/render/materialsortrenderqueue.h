#ifndef MATERIALSORTRENDERQUEUE_H
#define MATERIALSORTRENDERQUEUE_H

#include <curitiba/render/irenderqueue.h>

#include <curitiba/material/material.h>
#include <curitiba/material/imaterialgroup.h>
#include <curitiba/material/materialid.h>
#include <curitiba/math/itransform.h>

namespace curitiba
{
	namespace render
	{
		class MaterialSortRenderQueue : public IRenderQueue 
		{
		friend class RenderQueueFactory;
		
		public:
			void clearQueue (void);
			void addToQueue (curitiba::scene::SceneObject* aObject,
				std::map<std::string, curitiba::material::MaterialID> &materialMap);
			void processQueue (void);
		protected:
			MaterialSortRenderQueue(void);
		public:
			~MaterialSortRenderQueue(void);

		private:
			std::map<int, 
					std::map<curitiba::material::Material*, 
							std::vector<std::pair<curitiba::material::IMaterialGroup*, 
												  curitiba::math::ITransform*> >* >* > m_RenderQueue;
		};
	};
};

#endif //MATERIALSORTRENDERQUEUE_H
