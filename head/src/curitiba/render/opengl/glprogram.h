#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include <vector>
#include <string>

#include <GL/glew.h>

#include <curitiba/render/iprogram.h>
#include <curitiba/render/opengl/gluniform.h>


#define SHADER_COUNT  3

namespace curitiba
{
	namespace render
	{

		class GlProgram : public IProgram 
		{
		public:
			static void FixedFunction (void);

		private:
			std::vector<std::string> m_File; // filenames
			std::vector<std::string> m_Source; // source code
			std::vector<int> m_ID;
			std::vector<bool> m_Compiled;
			GLuint  m_P; // program id
			int m_NumUniforms;
			int m_MaxLength;

			std::string m_Name;
			
			std::vector<GlUniform> m_Uniforms; //list of uniforms
			
			bool m_VCompiled, m_FCompiled, m_GCompiled, m_PLinked;
			bool m_ShowGlobalUniforms;
		
		public:
		
			GlProgram();
			GlProgram (const std::string &vf, const std::string &gf, const std::string &ff); // filenames
			~GlProgram();

			virtual bool loadShader(IProgram::SHADER_TYPE type, const std::string &filename);
			bool reload (void);

			void setName(const std::string &name);
			const std::string &getName();

			virtual const std::string &getShaderFile(SHADER_TYPE type);
			virtual bool setShaderFile(SHADER_TYPE type, const std::string &name);

			virtual bool getPropertyb(int query);
			virtual int getPropertyi(int query);
			
			bool prepare (void);
			bool restore (void);

			bool setValueOfUniform (const std::string &name, float *values);
			bool setValueOfUniform (const std::string &name, int *values); 

			// FIXME: This should be private. It's here
			// until we can find a nice way to bind vertex attributes
			int getAttributeLocation (const std::string &name);

			std::string getShaderInfoLog(SHADER_TYPE type);
			char *getProgramInfoLog(); 
			int programValidate();
	
			virtual bool compileShader (IProgram::SHADER_TYPE);
			virtual bool linkProgram (void);
			virtual void useProgram (void);

			virtual bool isCompiled(SHADER_TYPE type);
			virtual bool areCompiled();
			virtual bool isLinked();

			virtual int getNumberOfUniforms (void);
			virtual int getNumberOfUserUniforms (void);

			virtual const GlUniform& getUniform (int i);
			virtual const GlUniform& getUniform (const std::string &name);
			void updateUniforms ();
			int findUniform (const std::string &name);

		private:

			void init();
			bool reloadShaderFile(IProgram::SHADER_TYPE aType);

			void setUniforms();
			void setValueOfUniform (int i);
			void showGlobalUniforms (void);

			void getAttribsLoc();
		};
	};
};

#endif
