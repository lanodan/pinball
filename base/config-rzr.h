//#ident "$Id: config-rzr.h,v 1.5 2003/05/24 10:55:06 rzr Exp $"
//#warning "!+rzr: Win32 portability hacks @ www.rzr.online.fr"
#ifndef config_rzr_h_ // !+rzr 
#define config_rzr_h_
/**
 * @author: www.Philippe.COVAL.free.fr - rev: $Author: rzr $
 * Portability issues:
 *
 * - Linux = gcc@linux
 configure && make && make install && make clean
 *
 * - WIN32 = mingw32@linux

 cross-configure.sh --prefix=/pinball --disable-sdltest && \
 cross-make.sh
 cross-make.sh -C src wpinball.exe && \
 \cp -f src/wpinball.exe /pinball/

 * - WIN32 = msvc@windows
  Install SDL libs
  Just open the project file pinball.dsw
  Build and copy dll in windows/system... or build dir (Release or Debug)
  exec Pinball.exe
 
 **/
 
/// default settings customise yourself
/// just define if you dont want to use plugins (dll vs static) 
/// default: undefined

/// if the program file is copied elsewhere it still works
//#undef RZR_PATHRELATIVE  // @ base/Config.cpp
/// mingw32@Linux : Dyn loadling works, but keyboard is too slow w/ dll ???
//#undef RZR_LIBSTATIC  // @ src/Loader.cpp
/// vfat etc
//#undef RZR_LINKS_UNSUPPORTED
///
//#undef RZR_CONFIG_STATIC_DEFAULT
/// cstdlibs random is not definied @msvc + mingw32
//#undef RZR_RANDOM_UNSUPPORTED
///
//#undef RZR_DEBUG
//--------------------------------------------------------------------------
/// GCC @ gnu.org (everything is autoconfigured)
#if ( (defined unix ) && ( defined __GNUC__ ) )
#endif

/// Visual C++ @ MicroSoft.com   (win32@win32)
#if ( (defined WIN32 ) && ( defined _MSC_VER ) )
#undef  HAVE_CONFIG_H 
#undef  HAVE_UNISTD_H
#define RZR_LINKS_UNSUPPORTED 1
#define RZR_RANDOM_UNSUPPORTED 1
#define RZR_WINAPI 1 
#define RZR_CPP_USE_NAMESPACE_STD
#define RZR_LIBSTATIC 1
#define RZR_CONFIG_STATIC_DEFAULT 1
#ifdef _DEBUG // win32 msvc debug
#define RZR_DEBUG 1
#endif
#define RZR_PATHRELATIVE 1
#define EM_DATADIR "../data/" // from ./Release/pinball.exe
#define EM_LIBDIR "." //unused (so far)
#define EM_HIGHSCORE_DIR EM_DATADIR
#endif

/// CodeWarrior  @ MetroWreks.com (xcompiler windows2mac )
#if(  ( defined __MWERKS__ ) ) //(defined WIN32 ) &&
#undef  HAVE_CONFIG_H
#define HAVE_UNISTD_H 1
#define RZR_LINKS_UNSUPPORTED 1
#define RZR_RANDOM_UNSUPPORTED 1
#define RZR_CONFIG_STATIC_DEFAULT 1
#define RZR_CPP_USE_NAMESPACE_STD
#define RZR_LIBSTATIC 1
#undef  RZR_PATHRELATIVE // main(argc,argv) lost @src/Pinball.cpp 
#define EM_HIGHSCORE_DIR EM_DATADIR
#endif

/// gcc(mingw32) @ GNU.org  (gcc for Win32, like cygwin) 
#if ( (defined WIN32 ) && ( defined __GNUC__ ) )
#define HAVE_UNISTD_H 1
#define RZR_LINKS_UNSUPPORTED 1
#define RZR_PATHRELATIVE 1
#define RZR_LIBSTATIC  1
#define RZR_RANDOM_UNSUPPORTED 1
#endif

/// other compilers
#if ( ! ( (defined __GNUC__ ) || ( defined _MSC_VER )  || (defined __MWERKS__ )))
#warning "This compiler has never been tested (so far)"
#define RZR_CONFIG_STATIC_DEFAULT 1
#define RZR_LINKS_UNSUPPORTED 1
#undef  RZR_PATHRELATIVE
#define HAVE_UNISTD_H
#endif

//--------------------------------------------------------------------------

#ifdef RZR_LINKS_UNSUPPORTED
#define lstat( name, opt)  stat(name, opt); //undef on mingw32@Linux
#endif


#ifdef RZR_RANDOM_UNSUPPORTED 
#define random rand // cstdlibs random is not definied @msvc + mingw32
#endif

#ifdef RZR_CPP_USE_NAMESPACE_STD
#include <iostream> 
//namespace std {};
using namespace std;
#endif

#ifdef RZR_WINAPI
#include <windows.h>
#ifndef WINAPI  // what are those ? dll rt ?  @ windows.h 4 GL/gl.h
#define WINAPI __stdcall  // not sure it is good , but it worked for me
// WINGDIAPI
#endif
#endif

#ifdef  RZR_DEBUG
#define EM_DEBUG 1
#undef  RELEASE 
//#include <assert.h>
#include <cassert>
#else // win32 msvc release
#define RELEASE 1
#undef  EM_DEBUG
//#undef  assert
//#define assert(x); {}
#endif

#ifdef RZR_PATHRELATIVE // on upcoming versions
#ifndef EM_DATADIR
#define EM_DATADIR "share/pinball" // single dir and exes in it (!= unix)
#endif
#ifndef EM_LIBDIR
#define EM_LIBDIR "lib/pinball"  // BUT subdirs are like unix
#endif
#else
#ifndef EM_DATADIR
#define EM_DATADIR "/pinball/data/" 
#endif
#ifndef EM_LIBDIR
#define EM_LIBDIR "/pinball/lib/"  
#endif
#endif


//--------------------------------------------------------------------------
// Common WIN32 options tested on mingw32 + msvc6
#ifdef WIN32 //!+rzr MSVC++ , mingw32 (etc not tested so far)
// ----- // configure should check if the install fs supports modes & links
//#include <io.h> // mkdir
//#include <sys/stat.h>
//#define mkdir(dir,modes) mkdir(dir) // @direct.h // autoconf should do that 

#endif // WIN32 // !rzr-


#ifdef _MSC_VER
//#undef WINAPI
//#ifdef Polygon // Polygon Is A Macro So It Is Renamed To Be Used As AClass
//#undef Polygon // other solution is to write "class Polygon" instead of just Polygon
//#define Polygon PolygonClass
// ---- Texture  Engine // undefined reference to `gluErrorString@4
//#undef EM_GLERROR
//#define EM_GLERROR(a) EM_COUT(a,42)
#include <GL/glu.h>
//#define gluErrorString(x) x
//#endif 
#endif // msvc6


#ifdef _MSC_VER
// trunct template 255
#pragma warning (disable:4786)
//Disable bogus conversion warnings.
#pragma warning (disable:4244)
// VC++ 5.0 version of above warning.
#pragma warning (disable:4305)
// ??? // MSVC6.0 dll / ccxx
#pragma warning (disable:4273)
// bool / uc
#pragma warning (disable:4800)
// symb trunc
#pragma warning (disable:4786)
// dominance ?
#pragma warning (disable:4250)
// unreferenced
#pragma warning (disable:4101)
#endif

//--------------------------------------------------------------------------
#ifdef RZR_DELETESAFE
#define deletesafe(x); { delete(x); x=NULL; } //!+rzr @ Group.h
#ifdef delete
//#warning "..."
#define delete_1stdef_hopeitisunused delete
#undef delete
#define delete(x) deletesafe(x) //!-rzr
#endif
#endif
//--------------------------------------------------------------------------
#ifdef  RZR_CONFIG_STATIC_DEFAULT
#define EM_USE_SOURCE_SPECULAR 1
#define EM_USE_QUADTREE 1
#define EM_USE_SDLIMAGE 1
#define EM_USE_SDLMIXER 1
#define EM_USE_SDL 1
#define EM_USE_SHARED_COLOR 1
#define HAVE_MEMORY_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define PACKAGE "pinball"
#define PACKAGE_BUGREPORT ""
#define PACKAGE_NAME ""
#define PACKAGE_STRING ""
#define PACKAGE_TARNAME ""
#define PACKAGE_VERSION ""
#define STDC_HEADERS 1
#define VERSION "cvs-__DATE__"
#endif
#endif //!-rzr ----------------------------------------------------------------
//EOF: $Id: config-rzr.h,v 1.5 2003/05/24 10:55:06 rzr Exp $
