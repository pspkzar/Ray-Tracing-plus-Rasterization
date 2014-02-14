#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <curitiba/math/vec3.h>
#include <curitiba/math/vec4.h>
#include <string>

#include <curitiba/event/ilistener.h>
#include <curitiba/event/eventVec3.h>

using namespace curitiba::event_;

namespace curitiba
{
	namespace render
	{
		class Viewport: public IListener
		{
		private:
			curitiba::math::vec3 m_Origin;
			curitiba::math::vec3 m_Size;
			curitiba::math::vec3 m_RelSize;
			curitiba::math::vec3 m_RelOrigin;
			curitiba::math::vec4 m_BackgroundColor;
			bool m_Fixed;
			float m_Ratio;
			std::string m_Name;

		public:
			Viewport(void);

			void eventReceived(const std::string &sender, const std::string &eventType, IEventData *evtData);
			std::string &getName();

			void setName(std::string);
			const curitiba::math::vec3& getOrigin (void);
			void setOrigin (float x, float y);
			const curitiba::math::vec3& getSize (void);
			void setSize (float width, float height);

			const curitiba::math::vec4& getBackgroundColor (void);
			void setBackgroundColor (const curitiba::math::vec4& aColor);

			float getRatio();
			void setRatio(float m);

			bool isFixed (void);
			void setFixed (bool value);

			

			bool isRelative(void);

		public:
			~Viewport(void);
		};
	};
};
#endif
