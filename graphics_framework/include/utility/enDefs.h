#pragma once
#include <utility/enGraphics.h>

#include <cstdint>
#include <cassert>
#include <iostream>


/**
 * @brief This file contains all  typedef, #define and all other data
 * that can be check at compile time
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
/** ENUM'S*/
/*++++++++++++++++++++++++++++++++++++*/
/**
*@brief used to determine the error that was committed
*/
enum class enErrorCode :Byte4
{
  Sucessful = 0b000'0000'0000'0000'0000'0000'0000'0000, //!<  indicates that no error occurred  
  UnClassified = 0b000'0000'0000'0000'0000'0000'0000'0001,//!<  indicates that there is no-know name for the error 
  NotReady = 0b000'0000'0000'0000'0000'0000'0000'0010, //!<  means that the function/class/object needed more set up before it's ready to be used  
  InvalidPath = 0b000'0000'0000'0000'0000'0000'0000'0100,//!< means that a provided path is invalid 
  ShaderComplieError = 0b000'0000'0000'0000'0000'0000'0000'1000,//!< means that an error occurred with compiling the shader
  ShaderLinkError = 0b000'0000'0000'0000'0000'0000'0001'0000, //!< means that a provided path 
  ActorComponentError = 0b000'0000'0000'0000'0000'0000'0010'0000,//!< means that a provided path 
  FailedCreation = 0b000'0000'0000'0000'0000'0000'0100'0000,//!<means that something was not created correctly.
};

/** tells the a.p.i how the information is
formated
*/
enum enFormats
{
#if DIRECTX
  /* one channel */
  uR8 = DXGI_FORMAT_R8_UINT,
  uR16 = DXGI_FORMAT_R16_UINT,
  fR16 = DXGI_FORMAT_R16_FLOAT,
  uR32 = DXGI_FORMAT_R32_UINT,
  fR32 = DXGI_FORMAT_R32_FLOAT,

  /* two channel*/
  fR16G16 = DXGI_FORMAT_R16G16_FLOAT,
  /* three channel */

  /*Four channel */
  R8G8B8A8_uniform_norm = DXGI_FORMAT_R8G8B8A8_UNORM,
  fR16G16B16A16 = DXGI_FORMAT_R16G16B16A16_FLOAT,
  fR32G32B32A32 = DXGI_FORMAT_R32G32B32A32_FLOAT,
  /* other */
  depthStencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT
#elif OPEN_GL//TODO : GL
  /* one channel */
  uR8 = GL_UNSIGNED_BYTE, //GL_R8
  uR16 = GL_UNSIGNED_SHORT,
  fR16 = GL_HALF_FLOAT,
  uR32 = GL_UNSIGNED_INT,
  fR32 = GL_FLOAT,
/*two channel */
fR16G16 = GL_RGB16F,
/* three channel */

/*Four channel */
R8G8B8A8_uniform_norm,
fR16G16B16A16 = GL_RGBA16F,

fR32G32B32A32 = GL_RGBA32F,
/* other */
depthStencil_format = GL_DEPTH24_STENCIL8,
#else

  uR8,
  uR16,
  fR16,
  uR32,
  fR32,
/*two channel */
fR16G16,
/* three channel */

/*Four channel */
R8G8B8A8_uniform_norm,
fR16G16B16A16,
fR32G32B32A32,
/* other */
depthStencil_format,
#endif // DIRECTX
};

/**
*@brief tells the api how the comparison function(for the sampler) is going to work
*/
enum class enComparison
{
#if DIRECTX
  Never = D3D11_COMPARISON_NEVER,
  Less = D3D11_COMPARISON_LESS,
  Equal = D3D11_COMPARISON_EQUAL,
  Less_Equal = D3D11_COMPARISON_LESS_EQUAL,
  Greater = D3D11_COMPARISON_GREATER,
#elif OPEN_GL 
  Never = GL_NEVER,
  Less = GL_LESS,
  Equal = GL_EQUAL,
  Less_Equal = GL_LEQUAL
#else 

  Never,
  Less,
  Equal,
  Less_Equal
#endif // DIRECTX
};

/**
*@brief used to tell the a.p.i which type of buffer we are dealing with.
*/
enum enBufferType
{
  NONE = 0,
#if DIRECTX
  Vertex = D3D11_BIND_VERTEX_BUFFER,
  Index = D3D11_BIND_INDEX_BUFFER,
  Const = D3D11_BIND_CONSTANT_BUFFER,
  ShaderResource = D3D11_BIND_SHADER_RESOURCE

#else
  Vertex = 0b00'00'0000'0001,
  Index = 0b00'00'0000'0010,
  Const = 0b00'00'0000'0100,
  ShaderResource = 0b00'00'0000'1000,
#endif // DIRECTX
};

/**
*@ this enum tells the current a.p.i how too use a buffer
*/
enum class enBufferUse
{
#if DIRECTX
  shaderInput = DXGI_USAGE_SHADER_INPUT,
  renderTragetOut = DXGI_USAGE_RENDER_TARGET_OUTPUT,
  backBuffer = DXGI_USAGE_BACK_BUFFER,
#else
  shaderInput,
  renderTragetOut,
  backBuffer,
#endif // DIRECTX
};

//! used to know which type of component the actor has attached
enum class enComponentTypes
{
  NONE = -1,
  Transform,
  AABB,
  Sphere,
  Model
};


/**
*@brief tells the sampler how to filter the images it was given
*/
enum class enFilter
{
#if DIRECTX
  MinMagMip_Point = D3D11_FILTER_MIN_MAG_MIP_POINT,
  MinMagMip_Point_Linear = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
  MinMagMip_Linear_Mip_Point = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
  MinMagMip_MagMip_Linear = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
  MinMagMip_Linear = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
  /**************************************/
  Anisotropic = D3D11_FILTER_ANISOTROPIC,
  Anisotropic_Comperasion = D3D11_FILTER_COMPARISON_ANISOTROPIC,
  Anisotropic_Minimum = D3D11_FILTER_MINIMUM_ANISOTROPIC,
  Anisotropic_Maximum = D3D11_FILTER_MAXIMUM_ANISOTROPIC
#elif OPEN_GL
  MinMagMip_Point,
  MinMagMip_Point_Linear,
  MinMagMip_Linear_Mip_Point,
  MinMagMip_MagMip_Linear,
  MinMagMip_Linear,
  /**************************************/
  Anisotropic,
  Anisotropic_Comperasion,
  Anisotropic_Minimum,
  Anisotropic_Maximum
#else
  MinMagMip_Point,
  MinMagMip_Point_Linear,
  MinMagMip_Linear_Mip_Point,
  MinMagMip_MagMip_Linear,
  MinMagMip_Linear,
    /**************************************/
    Anisotropic,
    Anisotropic_Comperasion,
    Anisotropic_Minimum,
    Anisotropic_Maximum,
  #endif // DIRECTX
};

/*! tell the api which topology to use */
enum class enTopology
{
  UnDefined = 0,
#if DIRECTX
  TriList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
  PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
  LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST
#elif OPEN_GL
  TriList = GL_TRIANGLES,
  PointList = GL_POINTS,
  LineList = GL_LINES
#else
  TriList,
  PointList,
  LineList,
#endif // DIRECTX
};

/**
*@brief tells the a.p.i how to use the x,y,z(aka u,v,w) of a given texture
*/
enum class enTextureAddress : int32_t
{
#if DIRECTX
  Wrap = D3D11_TEXTURE_ADDRESS_WRAP,
  Mirror = D3D11_TEXTURE_ADDRESS_MIRROR,
  Clamp = D3D11_TEXTURE_ADDRESS_CLAMP,
  Border = D3D11_TEXTURE_ADDRESS_BORDER,
  Mirror_once = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
#elif OPEN_GL
  Wrap,
  Mirror,
  Clamp,
  Border,
  Mirror_once
#else
  Wrap,
  Mirror,
  Clamp,
  Border,
  Mirror_once
#endif // DIRECTXe
};

/**
*@brief  this is used to tell me which type of element is used by the uniform
*/
enum class enConstBufferElem :int32
{
  NONE = 0,
  mat4x4,
  mat3x3,

  vec4,
  vec3,
  vec2,

  single_float,
  // integer types  
  imat4x4,
  imat3x3,

  ivec4,
  ivec3,
  ivec2,

  single_int,

};

/*!
*@brief used to determine which shader is being used
*/
enum class enShaderTypes
{
  NONE = -1,
  vertex,
  pixel
};

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
                  " ==> 1) does not exist in the current actor \n"
                  " ==> 2) the component needed more set-up in order to do it's job ");
    break;
    case enErrorCode::FailedCreation:
    messageFormat(FunctionName,
                  R"(the creation of some resource has failed, check if more setup is needed
or if the setup wrong.)");

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
#define EN_LOG_DB(message) std::cerr << #message << '\n';
#else
// does nothing 
#define EN_LOG_ERROR
#define EN_LOG_ERROR_WITH_CODE (ErrorCode)  ErrorCode ;
#define EN_LOG_DB(message);
#endif // !NDEBUG
