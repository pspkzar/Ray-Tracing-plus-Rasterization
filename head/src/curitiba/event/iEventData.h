#ifndef IEVENTDATA_H
#define IEVENTDATA_H



namespace curitiba
{
	namespace event_
	{
		class IEventData
		{
		public:
			~IEventData(void){};

			virtual void *getData(void){return 0;};
			virtual void setData(void *data){};
		protected:
			IEventData(void){};

		};

	};
};

#endif