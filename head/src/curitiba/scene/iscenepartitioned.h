#ifndef ISCENEPARTITIONED_H
#define ISCENEPARTITIONED_H

#include <curitiba/scene/iscene.h>
#include <curitiba/geometry/frustum.h>
#include <curitiba/geometry/boundingbox.h>

namespace curitiba {

	namespace scene {

		class IScenePartitioned : public IScene
		{
		protected:

			bool m_Built;

		public:

			IScenePartitioned(): IScene(), m_Built(false) {}

			//virtual bool load (std::string &aSceneFile);
			virtual void build (void) = 0;
			virtual bool isBuilt(void) {return m_Built;};

			virtual std::string getType (void) = 0;

			~IScenePartitioned (void) { }
		};
	};
};

#endif
