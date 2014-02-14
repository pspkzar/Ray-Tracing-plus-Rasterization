#ifndef SCENEPOSES_H
#define SCENEPOSES_H

#include <curitiba/scene/scene.h>
#include <curitiba/geometry/meshposeanim.h>

using namespace curitiba::geometry;


namespace curitiba {

	namespace scene {

		class ScenePoses : public Scene
		{
			public:
				ScenePoses(void);
				~ScenePoses(void);

				virtual void compile();
			
				virtual std::string getType (void);
				virtual void eventReceived(const std::string &sender, const std::string &eventType, IEventData *evt);

				void setActiveAnim(std::string aName);
				std::string getActiveAnim();
				void addAnim(std::string aName, float aLength);
				MeshPoseAnim &getAnim(std::string aName);
				void addAnimTrack(std::string aName, unsigned int aTrack);

			private:
				void setPose(int index);
				void setPose(std::string name);
				void setRelativeTime(std::string aAnim, float time);


				std::map<std::string, MeshPoseAnim> m_Anims;
				std::string m_ActiveAnim;

		};
	};
};



#endif
