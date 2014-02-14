#ifndef IPROGRAM_H
#define IPROGRAM_H

#include <string>


namespace curitiba
{
	namespace render
	{
		class IProgram
		{
		public:
			static IProgram* create ();

			enum SHADER_TYPE {
				VERTEX_SHADER,
				FRAGMENT_SHADER,
				GEOMETRY_SHADER
			 };

			virtual bool loadShader(IProgram::SHADER_TYPE type, const std::string &filename) = 0;
			virtual bool reload (void) = 0;
			
			virtual  bool prepare (void) = 0;
			virtual  bool restore (void) = 0;

			virtual std::string getShaderInfoLog(SHADER_TYPE type) = 0;
			virtual char *getProgramInfoLog() = 0; 
			virtual int programValidate() = 0;

			virtual bool setValueOfUniform (const std::string &name, float *values) = 0; //Use polymorphism on this
			virtual bool setValueOfUniform (const std::string &name, int *values) = 0;

			virtual int getNumberOfUniforms (void) = 0;
			virtual int getNumberOfUserUniforms (void) = 0;

			// FIXME: probably doesn't belong on the
			// interface. It's neede to bind vertex attributes for GLSL programs
			virtual int getAttributeLocation (const std::string &name) = 0;

			virtual const std::string &getShaderFile(SHADER_TYPE type) = 0;
			virtual bool setShaderFile(SHADER_TYPE type, const std::string &name) = 0;

			virtual ~IProgram(void) {};

			virtual void setName(const std::string &name) = 0;
			virtual const std::string &getName() = 0;

			virtual bool isCompiled(SHADER_TYPE type) = 0;
			virtual bool isLinked() = 0;

			virtual bool reloadShaderFile(SHADER_TYPE type) = 0;

			virtual bool compileShader (IProgram::SHADER_TYPE) = 0;
			virtual bool linkProgram (void) = 0;
			virtual void useProgram (void) = 0;


			virtual bool getPropertyb(int query) = 0;
			virtual int getPropertyi(int query) = 0;

		};
	};
};

#endif
