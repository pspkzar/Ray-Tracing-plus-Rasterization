#ifndef LISTENERTYPE_H
#define LISTENERTYPE_H

#include <curitiba/event/ilistener.h>
#include <vector>

using namespace std;

namespace curitiba
{
	namespace event_
	{
		class ListenerType
		{
		public:

			// vars
			std::string eventType;
			vector<IListener*> lsts;

			// Contrutors
			ListenerType(std::string eventType, vector<IListener*> *lsts); 
			ListenerType(const ListenerType &e);
			ListenerType(void);
			~ListenerType(void);

			// Methods
			void setEventType(std::string eventType);
			std::string *getEventType(void);
			void setListeners(vector<IListener *> *lsts); 
			vector<IListener *> *getListeners(void);
		};
	};
};
#endif