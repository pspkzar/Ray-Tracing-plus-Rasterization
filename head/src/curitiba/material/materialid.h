#ifndef MATERIALID_H
#define MATERIALID_H

#include <string>
#include <curitiba/material/material.h>

namespace curitiba
{
	namespace material
	{

		class MaterialID {

		private:
			std::string m_LibName, m_MatName;

		public:
			MaterialID (void);
			MaterialID (std::string libName, std::string matName);
			~MaterialID (void);

			void setMaterialID (std::string libName, std::string matName);
			const std::string& getLibName (void);
			const std::string& getMaterialName (void);
			Material *m_MatPtr;

			Material *getMaterialPtr();
		};
	};
};

#endif //MATERIALID_H
