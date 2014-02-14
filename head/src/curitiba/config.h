#ifndef CURITIBA_CONFIG_H
#define CURITIBA_CONFIG_H      
      
// use OpenGL
#define CURITIBA_OPENGL 1

// enable and disable functionalities 
// based on OpenGL version
#define CURITIBA_OPENGL_VERSION 300

// use atomic counters
#define CURITIBA_USE_ATOMICS 1

// use only core features (1 implies no fixed function)
#define CURITIBA_CORE_OPENGL 1

// use Optix
//#define CURITIBA_OPTIX 1

#define CURITIBA_PLATFORM_WIN32 1

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#define __SLANGER__ 1

#if _MSC_VER >= 1400
#ifndef _CRT_SECURE_NO_DEPRECATE
    #define _CRT_SECURE_NO_DEPRECATE
    #define _CRT_NONSTDC_NO_DEPRECATE
#endif
#endif

#define CURITIBA_RENDER_FLAGS

//#define COMPATIBILITY_FLAG_OPENGL 

#endif // CURITIBA_CONFIG_H
