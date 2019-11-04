#pragma once
#include <utility/enGraphics.h>

#include <cstdint>
#include <cassert>
#include <iostream>
/**
 * @brief This file contains all  typedef, #define and all other data
 * that can be check at compile time
 *
 */

 /*++++++++++++++++++++++++++++++++++++*/
 /* TYPE DEF'S  */
 /*++++++++++++++++++++++++++++++++++++*/
  // signed int type's
using int8 = int8_t; /*!<this is a 8-bit integer */
using int16 = int16_t;//!<  this is a 16-bit integer 
using int32 = int32_t;//!<  this is a 32-bit integer 
using int64 = int64_t;//!<  this is a 64-bit integer 

// unsigned int type's 
using uint8 = uint8_t;//!<  this is a 8-bit unsigned integer 
using uint16 = uint16_t;//!<  this is a 16-bit unsigned integer 
using uint32 = uint32_t;//!<  this is a 32-bit unsigned integer 
using uint64 = uint64_t;//!<  this is a 64-bit unsigned integer 

// signed int type's that tell you how much memory they use 
using Byte = int_least8_t;//!<  a single byte 
using Byte2 = int_least16_t;//!<  2 bytes per instance
using Byte4 = int_least32_t;//!<  4 bytes per instance 
using Byte8 = int_least64_t;//!<  8 bytes per instance 

// unsigned int type's that tell you how much memory they use 
using uByte = uint_least8_t;//!<  a single byte 
using uByte2 = uint_least16_t;//!<  2 bytes per instance
using uByte4 = uint_least32_t;//!<  4 bytes per instance 
using uByte8 = uint_least64_t;//!<  8 bytes per instance 

/*++++++++++++++++++++++++++++++++++++*/
/** ENUM'S / error codes  */
/*++++++++++++++++++++++++++++++++++++*/

enum class enErrorCode :Byte4
{
  Sucessful = 0b000'0000'0000'0000'0000'0000'0000'0000, //!<  indicates that no error occurred  
  UnClassified = 0b000'0000'0000'0000'0000'0000'0000'0001,//!<  indicates that there is no-know name for the error 
  NotReady = 0b000'0000'0000'0000'0000'0000'0000'0010, //!<  means that the function/class/object needed more set up before it's ready to be used  
  InvalidPath = 0b000'0000'0000'0000'0000'0000'0000'0100,//!< means that a provided path 
  ShaderComplieError = 0b000'0000'0000'0000'0000'0000'0000'1000,//!< means that an error occurred with compiling the shader
  ShaderLinkError = 0b000'0000'0000'0000'0000'0000'0001'0000, //!< means that a provided path 
  ActorComponentError = 0b000'0000'0000'0000'0000'0000'0010'0000//!< means that a provided path 
};


/**
*@brief tell's each a.p.i how to interpret some piece of data  
*/
//enum enFormats
//{
//#if DIRECTX
//  /* one channel */
//  uR8 = DXGI_FORMAT_R8_UINT,
//  uR16 = DXGI_FORMAT_R16_UINT,
//  fR16 = DXGI_FORMAT_R16_FLOAT,
//  uR32 = DXGI_FORMAT_R32_UINT,
//  fR32 = DXGI_FORMAT_R32_FLOAT,
///*two channel */
//fR16G16 = DXGI_FORMAT_R16G16_FLOAT,
///* three channel */
//
///*Four channel */
//R8G8B8A8_uniform_norm = DXGI_FORMAT_R8G8B8A8_UNORM,
//fR16G16B16A16 = DXGI_FORMAT_R16G16B16A16_FLOAT,
//fR32G32B32A32 = DXGI_FORMAT_R32G32B32A32_FLOAT,
///* other */
//depthStencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT
//#elif OPEN_GL//TODO : GL
//  /* one channel */
//  uR8 = GL_UNSIGNED_BYTE,
//  uR16 = GL_UNSIGNED_SHORT,
//  fR16,
//  uR32 = GL_UNSIGNED_INT,
//  fR32 = GL_FLOAT,
///*two channel */
//fR16G16,
///* three channel */
//
///*Four channel */
//R8G8B8A8_uniform_norm,
//fR16G16B16A16,
//fR32G32B32A32,
///* other */
//depthStencil_format = GL_DEPTH24_STENCIL8,
//#else
//
//  uR8,
//  uR16,
//  fR16,
//  uR32,
//  fR32,
///*two channel */
//fR16G16,
///* three channel */
//
///*Four channel */
//R8G8B8A8_uniform_norm,
//fR16G16B16A16,
//fR32G32B32A32,
///* other */
//depthStencil_format,
//#endif // DIRECTX
//};
//



/*++++++++++++++++++++++++++++++++++++*/
/* Logger functions */
/*++++++++++++++++++++++++++++++++++++*/
namespace enError
{
  /*! Present error message to help with debugging */
  static void ENTROPY_log_error(const char* FunctionName,
                                std::size_t LineNumber,
                                const char* FileName)
  {
    std::cerr << "====================================" <<
      '\n' << "Error occurred in Function :\"" << FunctionName << "\"\n"
      << "On line [" << LineNumber << "]\n"
      << "In File \"" << FileName << '\"' << '\n';
  }

  static void ENTROPY_log_error_code(const char* FunctionName,
                                     std::size_t LineNumber,
                                     const char* FileName,
                                     enErrorCode errorCode = enErrorCode::UnClassified)
  {
    //this is the format all messages will follow 
    auto messageFormat = [](const char* FuncName, const char* ErrorReport) {
      std::clog << '\t' << ErrorReport << '\n' << '\t' << "in Function : '" << FuncName << "'\n\n";
    };

    ENTROPY_log_error(FunctionName,
                      LineNumber,
                      FileName);

    switch (errorCode)
    {
    case enErrorCode::Sucessful:
    messageFormat(FunctionName, "executed successfully");
    break;
    case enErrorCode::UnClassified:
    messageFormat(FunctionName, "Some Un-Classified Error occurred");
    break;
    case enErrorCode::NotReady:
    messageFormat(FunctionName, "Class needs more setup before it can be used");
    break;
    case enErrorCode::InvalidPath:
    messageFormat(FunctionName, "Provided path is invalid");
    break;
    case enErrorCode::ShaderComplieError:
    messageFormat(FunctionName, "failed to compile shader");
    break;
    case enErrorCode::ShaderLinkError:
    messageFormat(FunctionName, "failed to link shader");
    break;
    case enErrorCode::ActorComponentError:
    messageFormat(FunctionName, "Component is one of the following \n"
                  " ==> 1) does not exist in the current actor "
                  " ==> 2) the component needed more set-up in order to do it's job ");
    break;
    default:
    messageFormat(FunctionName, "No error massage set");
    break;
    }

  }


}

/*++++++++++++++++++++++++++++++++++++*/
/* #define's  */
/*++++++++++++++++++++++++++++++++++++*/

#ifndef  NDEBUG 
/*! output that an error occurred
in some function in some file in some line
does not indicate the error only where it possibly
happen */
#define EN_LOG_ERROR enError::ENTROPY_log_error(__func__,__LINE__,__FILE__);


#define EN_LOG_ERROR_WITH_CODE(ErrorCode) enError::ENTROPY_log_error_code(__func__,__LINE__,__FILE__,ErrorCode);
#else
// does nothing 
#define EN_LOG_ERROR
#define EN_LOG_ERROR_WITH_CODE (ErrorCode)  ErrorCode ;
#endif // !NDEBUG
