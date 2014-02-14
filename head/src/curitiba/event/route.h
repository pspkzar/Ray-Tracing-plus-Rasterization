#ifndef ROUTE_H
#define ROUTE_H

#include <curitiba/event/iEventData.h>
#include <curitiba/event/ilistener.h>
#include <string>

namespace curitiba
{
	namespace event_
	{
		class Route: public IListener
		{
		protected:
			std::string m_Name;
			std::string sender;
			std::string receiver;
			std::string eventIn;
			std::string eventOut;

		public:
			Route(std::string name, std::string sender, std::string receiver, std::string eventIn, std::string eventOut); 
			Route();
			~Route(void);

			std::string &getEventIn(void);
			std::string &getEventOut(void);
			std::string &getSender(void);
			std::string &getReceiver(void);
			std::string &getName(void);

			void eventReceived(const std::string &sender, const std::string &eventType, curitiba::event_::IEventData *evt);
			void init(std::string name, std::string sender, std::string receiver, std::string eventIn, std::string eventOut);
		};
	};
};
#endif