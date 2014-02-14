#ifndef SCENEAUX_H
#define SCENEAUX_H

#include <curitiba/scene/scene.h>

/* -------------------------------------------------------

This class only exists for debug purposes
This type of scene is to be used with Cameras, Lights,
and other objects that one may want to visualize although
normally they wouldn't hava a graphical representation

------------------------------------------------------- */

namespace curitiba {

	namespace scene {

		class SceneAux : public Scene
		{
		protected:
			//bool m_Compiled;

		public:
			SceneAux (void);

			virtual void unitize();
			virtual void compile();

			virtual std::string getType (void);


		public:
			~SceneAux (void);
		};
	};
};

#endif
