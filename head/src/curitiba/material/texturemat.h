#ifndef TEXTUREMAT_H
#define TEXTUREMAT_H

#include <vector>

#include <curitiba/system/file.h>
#include <curitiba/render/texture.h>
#include <curitiba/render/istate.h>
#include <curitiba/material/texturesampler.h>

namespace curitiba
{
	namespace material
	{

		class TextureMat {

		private:
			curitiba::render::Texture* m_Textures[8];
			curitiba::material::TextureSampler *m_Samplers[8];


		public:
			TextureMat();
			~TextureMat();

			TextureMat *clone();

			void setTexture(int unit, curitiba::render::Texture *t);
			void unset(int unit);

			std::vector<std::string> *getTextureNames();
			std::vector<int> *getTextureUnits();
			curitiba::render::Texture* getTexture(int unit);
			curitiba::material::TextureSampler* getTextureSampler(int unit);

			void prepare(curitiba::render::IState *state);
			void restore(curitiba::render::IState *state);
			void clear();

		};
	};
};

#endif //TEXTUREMAT_H
