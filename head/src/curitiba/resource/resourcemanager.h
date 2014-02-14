#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <curitiba/render/irenderable.h>
#include <curitiba/resource/texturemanager.h>
#include <curitiba/scene/sceneobject.h>
#include <curitiba/render/rendertarget.h>
#include <curitiba/render/texture.h>

#include <curitiba/material/material.h>

namespace curitiba 
{
	namespace resource
	{
		class ResourceManager
		{

		private:
			TextureManager* m_pTextureManager;
			std::string m_Path;
			
			std::map<std::string, curitiba::render::RenderTarget*> m_RenderTargets; /***MARK***/ //Replace with a manager?
			std::map<std::string, curitiba::render::IRenderable*> m_Meshes;
			std::map<std::string, curitiba::render::IProgram*> m_Programs;
			std::map<std::string, curitiba::render::IState*> m_States;

			static int renderableCount;

		public:
			ResourceManager (std::string path);
			~ResourceManager (void);

			void clear();
			//ISceneObject* addSceneObject (void);


			/***Textures***/
			bool hasTexture(std::string name);
			curitiba::render::Texture* newEmptyTexture(std::string &name);
			int getNumTextures();
			curitiba::render::Texture* getTexture(unsigned int i);
			curitiba::render::Texture* getTexture (std::string name);
			curitiba::render::Texture* addTexture (std::string fn, std::string label = "", bool mipmap = 1);
			curitiba::render::Texture* addTexture (std::vector<std::string> fn, std::string label, bool mipmap = 1);
			void removeTexture (std::string name);
			//curitiba::render::Texture* createTexture (std::string label, 
			//	std::string internalFormat, 
			//	std::string aFormat, 
			//	std::string aType, int width, int height);
			
			curitiba::render::Texture* createTexture (std::string label, 
				std::string internalFormat, 
				std::string aFormat, 
				std::string aType, int width, int height,
				unsigned char* data = NULL);

			curitiba::render::Texture* createTextureMS (std::string label, 
				std::string internalFormat, 
				int width, int height,
				int samples);

			// TEXIMAGE
			curitiba::material::TexImage* createTexImage(curitiba::render::Texture *t);
			curitiba::material::TexImage* getTexImage(std::string aTextureName);
			//void deleteTexImage(std::string aTextureName);


			/***Rendertargets***/
			curitiba::render::RenderTarget* createRenderTarget (std::string name, int width, int height);
			void removeRenderTarget (std::string name);
			curitiba::render::RenderTarget *getRenderTarget(const std::string &name);
			int getNumRenderTargets();
			std::vector<std::string>* ResourceManager::getRenderTargetNames();

			/***Renderables***/
			curitiba::render::IRenderable* createRenderable(std::string type, std::string name="", std::string filename = "");
			bool hasRenderable (std::string meshName, std::string filename);
			curitiba::render::IRenderable* getRenderable (std::string meshName, std::string filename);
			curitiba::render::IRenderable* addRenderable (curitiba::render::IRenderable* aMesh, std::string filename);
			void removeRenderable(std::string name);

			/***States***/
			bool hasState (std::string stateName);
			curitiba::render::IState* getState (std::string stateName);
			void addState (curitiba::render::IState* aState);

			/***SHADERS***/
			bool hasProgram (std::string programName);
			curitiba::render::IProgram* getProgram (std::string programName);
			unsigned int getNumPrograms();
			std::vector<std::string> *getProgramNames();

			/***Materials***/
//			curitiba::material::Material* addMaterial (curitiba::material::Material* aMaterial);
		};
	};
};
#endif //RESOURCEMANAGER_H
