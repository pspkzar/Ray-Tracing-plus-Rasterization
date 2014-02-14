#ifndef PASSLOADER_H
#define PASSLOADER_H

#include <string>

namespace curitiba
{
	namespace loader
	{
		class PassLoader
		{
		public:
			static bool load (std::string file);
		private:
			PassLoader(void);
		};
	};
};

#endif
