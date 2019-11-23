#pragma once
/*! this header is used to switch api's 
*
*/

/*! this define is used to select the directx api */
#define DIRECTX 1

#define OPEN_GL 0
//#ifdef OPEN_GL
//#undef DIRECTX

//#endif // OPEN_GL

#if( _WIN32) || (_WIN64)
/*! this is for the window only function  */
#define WIND_OS 1
#endif // _WIN32 || _WIN64

//! this is to be able to load model or load the cubes from the tutorial
#define MODEL_LOAD 1
