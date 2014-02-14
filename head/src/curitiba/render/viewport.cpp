#include <curitiba/render/viewport.h>
#include <curitiba.h>

using namespace curitiba::render;
using namespace curitiba::math;


Viewport::Viewport(void) :
	m_Origin (),
	m_Size (),
	m_RelSize(),
	m_RelOrigin(),
	m_Name("default"),
	m_Fixed (false),
	m_Ratio(-1)
{
	m_BackgroundColor.set(0.0, 0.0, 0.0, 0.0);
	EVENTMANAGER->addListener("WINDOW_SIZE_CHANGED", this);
}


Viewport::~Viewport(void)
{
	EVENTMANAGER->removeListener("WINDOW_SIZE_CHANGED",this);
}


void 
Viewport::setName(std::string aName)
{
	m_Name = aName;
}


std::string&
Viewport::getName() 
{
	return m_Name;
}


void
Viewport::eventReceived(const std::string &sender, const std::string &eventType, IEventData *evtData) {

	if (eventType != "WINDOW_SIZE_CHANGED")
		return;

	vec3 *ev = (vec3 *)(evtData->getData());

	if (false == m_Fixed) {
	
		m_Size.x = ev->x;
		m_Size.y = ev->y;
	}
	else {
		if (m_RelOrigin.x != -1)
			m_Origin.x = ev->x * m_RelOrigin.x;

		if (m_RelOrigin.y != -1)
			m_Origin.y = ev->y * m_RelOrigin.y;

		if (m_RelSize.x != -1 )
			m_Size.x = ev->x * m_RelSize.x;

		if (m_RelSize.y != -1 )
			m_Size.y = ev->y * m_RelSize.y;

		if (m_Ratio > 0)
			m_Size.y = m_Size.x * m_Ratio;
	}
	EVENTMANAGER->notifyEvent("VIEWPORT_CHANGED", m_Name, "", NULL); 
}


float 
Viewport::getRatio()
{
	if (m_Size.y == 0)
		return 1;
	else
		return m_Size.x / m_Size.y;
}


void 
Viewport::setRatio(float m) 
{
	if (m > 0) {
		m_Ratio = m;
		m_Size.y = m_Size.x * m_Ratio;
	}
}


const vec3&
Viewport::getSize (void) 
{
	return m_Size;
}


void
Viewport::setSize (float width, float height)
{
	if (width <=1) {
		m_RelSize.x = width;
		m_Size.x = m_RelSize.x * CURITIBA->getWindowWidth();
	}
	else {
		m_RelSize.x = -1;
		m_Size.x = width;
	}

	if (height <= 1) {
		m_RelSize.y = height;
		m_Size.y = m_RelSize.y * CURITIBA->getWindowHeight();
	}
	else {
		m_RelSize.y = -1;
		m_Size.y = height;
	}

	if (m_Ratio > 0)
		m_Size.y = m_Size.x * m_Ratio;
}


const vec3&
Viewport::getOrigin (void)
{
	return m_Origin;
}


void
Viewport::setOrigin (float x, float y)
{
	if (x < 1) {
		m_RelOrigin.x = x;
		m_Origin.x = CURITIBA->getWindowWidth() * x;
	}
	else {
		m_RelOrigin.x = -1;
		m_Origin.x = x;
	}

	if (y < 1) {
		m_RelOrigin.y = y;
		m_Origin.y = CURITIBA->getWindowHeight() * y;
	}
	else {
		m_RelOrigin.y = -1;
		m_Origin.y = y;
	}
	
}


const vec4&
Viewport::getBackgroundColor (void)
{
	return m_BackgroundColor;
}


void
Viewport::setBackgroundColor (const vec4& aColor)
{
	m_BackgroundColor = aColor;
}


bool 
Viewport::isFixed (void)
{
	return m_Fixed;
}


void 
Viewport::setFixed (bool value)
{
	m_Fixed = value;
}


bool 
Viewport::isRelative (void)
{
	return (m_RelOrigin.x != -1 || m_RelOrigin.y != -1 || 
		    m_RelSize.x != -1 || m_RelSize.y != -1);
}

