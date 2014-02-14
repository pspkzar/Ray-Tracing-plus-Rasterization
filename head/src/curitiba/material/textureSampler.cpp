#include <curitiba/material/textureSampler.h>
#include <curitiba/render/opengl/gltexturesampler.h>
#include <curitiba/config.h>
#include <string>

using namespace curitiba::material;
using namespace curitiba::render;


TextureSampler*
TextureSampler::create(Texture *t) {

#ifdef CURITIBA_OPENGL
	return new GLTextureSampler(t) ;
#elif CURITIBA_DIRECTX
	return new DXTextureSampler;
#endif
}


TextureSampler::TextureSampler() {

	// Texture State
	m_TexEnumProps.resize(COUNT_TEXTUREENUMPROPS);
	m_TexFloat4Props.resize(COUNT_TEXTUREFLOAT4PROPS);
}


unsigned int
TextureSampler::getSamplerID() {

	return m_SamplerID;
}





// ---------------------------------------------------------------------
//
//								TRANSLATE STRINGS
//
// ---------------------------------------------------------------------


int 
TextureSampler::translateStringToTexCompareModeEnum(std::string s) {

	if (s == "NONE")
		return(TEXTURE_COMPARE_NONE);
	if (s == "COMPARE_REF_TO_TEXTURE")
		return(TEXTURE_COMPARE_REF_TO_TEXTURE);

	return(-1);
}


int 
TextureSampler::translateStringToTexWrapModeEnum(std::string s) {

	if (s == "CLAMP_TO_EDGE")
		return(TEXTURE_WRAP_CLAMP_TO_EDGE);
	if (s == "CLAMP_TO_BORDER")
		return(TEXTURE_WRAP_CLAMP_TO_BORDER);
	if (s == "REPEAT")
		return(TEXTURE_WRAP_REPEAT);
	if (s == "MIRRORED_REPEAT")
		return(TEXTURE_WRAP_MIRRORED_REPEAT);
	return(-1);
}


int 
TextureSampler::translateStringToTexMinModeEnum(std::string s) {

	if (s == "NEAREST")
		return(TEXTURE_MIN_NEAREST);
	if (s == "LINEAR")
		return(TEXTURE_MIN_LINEAR);
	if (s == "NEAREST_MIPMAP_NEAREST")
		return(TEXTURE_MIN_NEAREST_MIPMAP_NEAREST);
	if (s == "NEAREST_MIPMAP_LINEAR")
		return(TEXTURE_MIN_NEAREST_MIPMAP_LINEAR);
	if (s == "LINEAR_MIPMAP_NEAREST")
		return(TEXTURE_MIN_LINEAR_MIPMAP_NEAREST);
	if (s == "LINEAR_MIPMAP_LINEAR")
		return(TEXTURE_MIN_LINEAR_MIPMAP_LINEAR);
	return(-1);
}


int 
TextureSampler::translateStringToTexMagModeEnum(std::string s) {

	if (s == "NEAREST")
		return(TEXTURE_MAG_NEAREST);
	if (s == "LINEAR")
		return(TEXTURE_MAG_LINEAR);
	return(-1);
}


int 
TextureSampler::translateStringToTexCompareFuncEnum(std::string s) {

	if (s == "NONE")
		return(COMPARE_NONE);
	if (s == "LEQUAL")
		return(COMPARE_LEQUAL);
	if (s == "GEQUAL")
		return(COMPARE_GEQUAL);
	if (s == "LESS")
		return(COMPARE_LESS);
	if (s == "GREATER")
		return(COMPARE_GREATER);
	if (s == "EQUAL")
		return(COMPARE_EQUAL);
	if (s == "NOTEQUAL")
		return(COMPARE_NOTEQUAL);
	if (s == "ALWAYS")
		return(COMPARE_ALWAYS);
	if (s == "NEVER")
		return(COMPARE_NEVER);

	return(-1);
}


