#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <vector>
#include <map>

//#include <curitiba/render/textureCubeMap.h>
#include <curitiba/render/texture.h>
#include <curitiba/material/teximage.h>


namespace curitiba 
{
	namespace resource
	{

		class TextureManager {
		
		friend class ResourceManager;

		private:
		        std::string m_Path;	
		        std::vector<curitiba::render::Texture*> m_Lib;
				std::map<std::string, curitiba::material::TexImage*> m_TexImageLib;

		public:

			void clear();
			void setPath (std::string path);

			curitiba::render::Texture* addTexture (std::string filename, std::string label, bool mipmap=true);
			curitiba::render::Texture* addTexture (std::vector<std::string> filenames, std::string label, bool mipmap=true);

			//curitiba::render::Texture* createTexture (
			//			std::string label, 
			//			std::string internalFormat, 
			//			std::string aFormat, 
			//			std::string aType, int width, int height);

			curitiba::render::Texture* createTexture (
						std::string label, 
						std::string internalFormat, 
						std::string aFormat, 
						std::string aType, int width, int height,
						unsigned char* data = NULL);

			curitiba::render::Texture* createTextureMS (
						std::string label, 
						std::string internalFormat, 
						int width, int height,
						int samples);

			//curitiba::render::Texture* createTexture ();			
			curitiba::render::Texture* newEmptyTexture(std::string name);
			void removeTexture (std::string name);
			//curitiba::render::Texture* addTexture (int id, char *label);

			bool hasTexture(std::string &name);
			curitiba::render::Texture* getTexture (int id);
			curitiba::render::Texture* getTexture (std::string name);
			std::vector<std::string>* getTextureLabels();

			int getNumTextures ();
			
			curitiba::render::Texture* getTextureOrdered (unsigned int position);
			int getTexturePosition (curitiba::render::Texture *t);


			// TEXIMAGE
			curitiba::material::TexImage* createTexImage(curitiba::render::Texture *t);
			curitiba::material::TexImage* getTexImage(std::string aTextureName);
	//		void deleteTexImage(std::string aTextureName);

		  protected:
			  TextureManager (std::string path);
			  //TextureManager(const CTextureManager&);
			  //TextureManager& operator= (const CTextureManager&);
		};
	};
};

#endif //TEXTUREMANAGER_H
