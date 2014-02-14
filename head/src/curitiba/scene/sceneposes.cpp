#include <curitiba/scene/sceneposes.h>
#include <curitiba/geometry/meshwithpose.h>
#include <curitiba/slogger.h>

using namespace curitiba::scene;

ScenePoses::ScenePoses(void): Scene(), m_ActiveAnim("")
{
	// have to register for events
	EVENTMANAGER->addListener("SET_POSE_BY_INDEX",(curitiba::event_::IListener *)this);
	EVENTMANAGER->addListener("SET_POSE_BY_NAME",(curitiba::event_::IListener *)this);
	EVENTMANAGER->addListener("ANIMATE",(curitiba::event_::IListener *)this);
}


ScenePoses::~ScenePoses(void) {

	// have to remove events
	EVENTMANAGER->removeListener("SET_POSE_BY_INDEX",(curitiba::event_::IListener *)this);
	EVENTMANAGER->removeListener("SET_POSE_BY_NAME",(curitiba::event_::IListener *)this);
	EVENTMANAGER->removeListener("ANIMATE",(curitiba::event_::IListener *)this);

}


std::string
ScenePoses::getType() {

	return("ScenePoses");
}


void
ScenePoses::eventReceived(const std::string &sender, const std::string &eventType, IEventData *evt) 
{
	if (eventType == "SET_POSE_BY_INDEX")
		setPose(*(int *)evt->getData());
	else if (eventType == "SET_POSE_BY_NAME")
		setPose(*(std::string*)evt->getData());
	else if (eventType == "ANIMATE")
		setRelativeTime(m_ActiveAnim, * ((float *)evt->getData()));
}


void 
ScenePoses::compile()
{
}


void 
ScenePoses::setActiveAnim(std::string aName) 
{
	if (m_Anims.count(aName) != 0)
		m_ActiveAnim = aName;
	else
		m_ActiveAnim = "";
}


std::string
ScenePoses::getActiveAnim() 
{
	return m_ActiveAnim;
}


void 
ScenePoses::addAnim(std::string aName, float aLength)
{
	m_Anims[aName] = MeshPoseAnim();
	m_Anims[aName].setLength(aLength);

	// by default set the active anim to be the first one added.
	if (m_ActiveAnim == "")
		m_ActiveAnim = aName;
}


void 
ScenePoses::addAnimTrack(std::string aName, unsigned int meshIndex) 
{
	if (m_Anims.count (aName) > 0)
		m_Anims[aName].addTrack(meshIndex);
}


void
ScenePoses::setPose(int index) {

	MeshPose *mp;

	unsigned int numScenes = m_SceneObjects.size();
	for (unsigned int i = 0; i < numScenes; i++) {
		
		mp = (MeshPose *)m_SceneObjects[i];
		mp->setPose(i);
	}
}


void
ScenePoses::setPose(std::string name) {

	MeshPose *mp;

	unsigned int numScenes = m_SceneObjects.size();
	for (unsigned int i = 0; i < numScenes; i++) {
		
		mp = (MeshPose *)m_SceneObjects[i];
		mp->setPose(name);
	}
}


// must exist, otherwise ...
MeshPoseAnim &
ScenePoses::getAnim(std::string aName)
{
	return m_Anims[aName];
}


void 
ScenePoses::setRelativeTime(std::string aAnim, float time) 
{
	SLOG("time = %f\n", time);
	if (m_Anims.count (aAnim) > 0) {

		MeshPoseAnim mpa = m_Anims[aAnim];

		for (unsigned int i = 0; i < m_SceneObjects.size(); i++) {
		
			((MeshPose *)m_SceneObjects[i]->_getRenderablePtr())->setPose(mpa.getInfluences(i,time * mpa.getLength()));
		} 
	}
}


