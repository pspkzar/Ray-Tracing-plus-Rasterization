#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <string>

#include <curitiba/material/colormaterial.h>
#include <curitiba/material/programvalue.h>
#include <curitiba/material/texturemat.h>

#include <curitiba/render/iprogram.h>
#include <curitiba/render/istate.h> 
#include <curitiba/render/texture.h>
#include <curitiba/clogger.h>



using namespace curitiba::render;

namespace curitiba
{
	namespace material
	{
		class MaterialLibManager;
		class MaterialLib;
		class Material {
			
			friend class MaterialLibManager;
			friend class MaterialLib;

		private:
		   
//			typedef enum  { FRONT_TO_BACK, BACK_TO_FRONT, NONE} orderType;

			curitiba::material::ColorMaterial m_Color;
			curitiba::material::TextureMat *m_Texmat;
			std::string m_Shader;
			curitiba::render::IState *m_State;

			// These are values specified in the material library
			std::map<std::string, curitiba::material::ProgramValue> m_ProgramValues;
			std::map<std::string, curitiba::material::ProgramValue> m_UniformValues;
			
			bool m_Enabled;
			bool m_useShader;
			std::string m_Name;	
			Material();

		public:
			~Material();

			Material *clone();

			void setName (std::string name);
			std::string& getName ();

			void prepare ();
			void prepareNoShaders();
			void setUniformValues();
			void restore();
			void restoreNoShaders();

			// Reset material to defaults
			void clear();
			void enable (void);
			void disable (void);
			bool isEnabled (void);

			
			curitiba::material::TextureMat* getTextures (void);
			bool createTexture (int unit, std::string fn);
			void attachTexture (int unit, std::string label);
			void attachTexture(int unit, Texture *t);
			Texture *getTexture(int unit);
			TextureSampler* getTextureSampler(int unit);
			void unsetTexture(int unit);
			std::vector<std::string> *getTextureNames();
			std::vector<int> *getTextureUnits();

			void attachProgram (std::string shaderName);
			void Material::cloneProgramFromMaterial(Material *mat);
			std::string getProgram();
			bool isInSpecML(std::string programValueName);
			void addProgramValue (std::string name, curitiba::material::ProgramValue progVal);
			void enableShader(bool value);
			bool isShaderEnabled();
			void clearUniformValues(); 

			std::map<std::string, curitiba::material::ProgramValue>& getProgramValues();
			std::map<std::string, curitiba::material::ProgramValue>& getUniformValues();
			ProgramValue *getProgramValue(std::string name);
			void setValueOfUniform(std::string name, float *values);
			std::vector<std::string> *getValidProgramValueNames();
			std::vector<std::string> *getUniformNames();




			curitiba::material::ColorMaterial& getColor (void);
			curitiba::render::IState* getState (void);

			void setState(IState *s);

			//void stateEnable(IState::StateSetting ss, bool value);
			//bool stateGetSetting(IState::StateSetting ss);


			//void stateSetProp(IState::StatePropInt prop, int value);
			//void stateSetProp(IState::StatePropFloat4 prop, float r, float g, float b, float a);
			//void stateSetProp(IState::StatePropFloat4 prop, const vec4& color);
			//void stateSetProp(IState::StatePropBool4 prop, bool r, bool g, bool b, bool a);
			//void stateSetProp(IState::StatePropEnum prop, int value);
			//void stateSetProp(IState::StatePropFloat, float value);

			//int stateGetPropi(IState::StatePropInt prop);
			//float stateGetPropf(IState::StatePropFloat prop);
			//const vec4& stateGetProp4f(IState::StatePropFloat4 prop); // MEMORY LEAK
			//bool* stateGetProp4b(IState::StatePropBool4 prop);

			//SINCE A TEXTURE CAN BE USED IN DIFFERENT MATERIALS
			//WITH DIFFERENT SETTINGS THESE SHOULD BE USED WHENEVER A 
			//TEXTURE IS "PREPARED"
			//virtual void setTexProp(IState::TextureUnit i, IState::TextureEnumProp prop, int value);
			//virtual void setTexProp(IState::TextureUnit i, IState::TextureFloat4Prop prop, float x, float y, float z, float w);
			//virtual void setTexProp(IState::TextureUnit i, IState::TextureFloat4Prop prop, vec4& value);
			//virtual int getTexProp(IState::TextureUnit i, IState::TextureEnumProp prop);
			//virtual vec4* getTexProp(IState::TextureUnit i, IState::TextureFloat4Prop prop); //MEMORY LEAK

			//void prepareFixed();
			//void restoreFixed();

			//void setId (int id);
			//int getId ();

			//void setBlend(std::string src, std::string dst);
			//void setAlphaTest(int value,float alphaValue);

			//void setCull(int value);

			//int getPriority (void);
			//void setPriority (int priority);

			//bool getTransparent (void);
			//void setTransparent (bool value);


			//creates a texture and provides a filename
			//it does not load the texture!

			//void save(std::ofstream &f, std::string path);
			//void load(std::ifstream &f, std::string libPath);
			//void loadTo(std::ifstream &inf, std::string libPath);


		};

		//
		// Struct that holds texture coordinate transforms for the Maya profile. 
		//
		// FIXME: This data should be baked into a generic TEXTURE MATRIX
		// parameter inside each of CMaterial's CTexture objects. Remove this
		// hack when people start caring again about good software design
		// *sigh*
		//

		//struct MayaTextureProfile {
		//   bool HasProfile;
		//   Material *pMaterial;
		//   float CoverageU;
		//   float CoverageV;
		//   float RepeatU;
		//   float RepeatV;
		//};
	};
};
#endif // MATERIAL_H

