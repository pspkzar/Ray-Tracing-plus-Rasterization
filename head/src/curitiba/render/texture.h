#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <math.h>

#include <curitiba/material/textureSampler.h>
#include <curitiba/enums.h>

#include <curitiba/config.h>

#ifdef __SLANGER__
#include <wx/bitmap.h>
#include <wx/image.h>
#include <IL/ilu.h>
#endif

namespace curitiba {

	namespace material {
		class TextureSampler;
	}
}

namespace curitiba
{
	namespace render
	{
		class Texture
		{
		public:
			typedef enum {
				TEXTURE_1D,
				TEXTURE_2D,
				TEXTURE_3D,
				TEXTURE_CUBE_MAP
			} TextureDimension;

			typedef enum { UNIT, 
				COUNT_SAMPLERPROPERTY} SamplerProperty;

			typedef enum { WIDTH, HEIGHT, DEPTH, SAMPLES,
				COUNT_INTPROPERTY} IntProperty;

			static const std::string SamplerString[COUNT_SAMPLERPROPERTY];
			static const std::string IntString[COUNT_INTPROPERTY];
			static void getPropTypeAndId(std::string &s, curitiba::Enums::DataType *dt , int *id);
			std::vector<int> m_IntProps;
			std::vector<int> m_SamplerProps;

			typedef enum {
				S_COORD,
				T_COORD,
				R_COORD,
				Q_COORD
			} TextureCoord;

			typedef enum {TEXTURE0, TEXTURE1, TEXTURE2, TEXTURE3, TEXTURE4, TEXTURE5,
					TEXTURE6,TEXTURE7,COUNT_TEXTUREUNIT} TextureUnit;


		protected:
			unsigned int m_Id;
			//int m_Width;
			//int m_Height;
			//int m_Depth;
			std::string m_StringDimension;
			std::string m_StringInternalFormat;
			std::string m_StringFormat;
			std::string m_StringType;
			std::string m_Label;
			//int m_Unit;

#ifdef __SLANGER__
			wxBitmap *bitmap;
#endif

		public:
			//static Texture* create (std::string label, std::string internalFormat,
			//	std::string aFormat, std::string aType, int width, int height);
			static Texture* create (std::string file, std::string label, bool mipmap=true);
			static Texture* create (std::string label);

			static Texture* create(std::string label, std::string internalFormat,
				std::string aFormat, std::string aType, int width, int height, 
				unsigned char* data = NULL );

			static Texture* createMS(std::string label, std::string internalFormat,
				int width, int height, 
				int samples );

			static std::string *getStringDimensions(void);
			static int *getDimensions(void);

			static std::string *getStringTypes(void);
			static int *getTypes(void);

			static std::string *getStringFormats(void);
			static int *getFormats(void);

			static std::string *getStringInternalFormats(void);
			static int *getInternalFormats(void);

			static bool isValidFormat(std::string aFormat);
			static bool isValidInternalFormat(std::string anInternalFormat);

			static int getArraySize(int IntArray[]);

#ifdef __SLANGER__
			virtual wxBitmap *getBitmap(void);
#endif

			//virtual void setData(std::string internalFormat, std::string aFormat, 
			//	std::string aType, int width, int height) = 0;

			virtual void setData(std::string internalFormat, std::string aFormat, 
				std::string aType, int width, int height, unsigned char * data = NULL) = 0;


			virtual void setId (int id);
			virtual int getId (void);

			virtual std::string& getLabel (void);
			virtual void setLabel (std::string label);

			int getWidth (void);
			int getHeight (void);
			int getDepth (void);

			int getPropi(IntProperty prop);
			int getProps(SamplerProperty prop);

			std::string getStringDimension(void);
			std::string getStringInternalFormat (void);
			std::string getStringFormat (void);
			std::string getStringType (void);
	
			virtual int getNumberOfComponents(void) = 0;
			virtual int getDimension(void) = 0;
			virtual int getInternalFormat (void) = 0;
			virtual int getType (void) = 0;
			virtual int getFormat(void) = 0;
			virtual int getElementSize() = 0;

			virtual bool getMipmap() = 0;

			virtual void prepare(int unit, curitiba::material::TextureSampler *ts) = 0;
			virtual void restore(int unit) = 0;

			void setUnit (int unit);
			int getUnit (void);
		
		protected:
			Texture(std::string label, std::string aDimension, std::string internalFormat, 
				std::string aFormat, std::string aType, int width, int height);
			Texture (std::string label);
		public:
			virtual ~Texture(void);
		};
	};
};

#endif
