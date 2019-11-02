#pragma once 
#include "../include/utility/Grafics_libs.h"
#include "utility/enDefs.h"
#include "enum_headers/enFormatEnums.h"
#include <string_view>

class cDevice;
class cDeviceContext;

struct sShaderResourceDesc
{
  Formats format = Formats::fR32G32B32A32;
  int viewDim;
};


class cShaderResourceView
{
public:
  cShaderResourceView();

  ~cShaderResourceView();
public:
#if DIRECTX
  //! get the shader Resource 
  ID3D11ShaderResourceView 
    *getShaderResource();
//! for function that ask for pointers to arrays 
  ID3D11ShaderResourceView 
    **getShaderResourceRef();

  D3D11_SHADER_RESOURCE_VIEW_DESC
    getDxDescriptor();

#endif // DIRECTX

  /*! loads an images from a file to create a shader resource*/
  bool 
    createShaderResourceFromFile(std::string_view file,cDevice &device, cDeviceContext &deviceContext);

  /*! returns a pointer to the data of the resource*/
  uint8 *
    getResourceData() const ;

#if OPEN_GL
  /*! returns the ID of the resource*/
  uint32_t
    getResourceID() const;

  uint32_t
    getTextureID() const;

#endif // OPEN_GL

  /*! returns the width of the resource associated 
  with the resource view*/
  int32_t
    getWidth()const;


  /*! returns the height of the resource associated 
  with the resource view*/
  int32_t
    getHeight()const;


  /*! returns the amount of color channels the 
  resource has*/
  int32_t
    getChannelCount()const;

  void
    setDescriptor(Formats format, int viewDim);
private:
 sShaderResourceDesc m_desc;
  uint8 * mptr_resourceData = nullptr;
#if DIRECTX
  ID3D11ShaderResourceView *mptr_shaderResource;
#elif OPEN_GL
  uint32_t m_resourceID{ 0u };
  uint32_t m_imageID{ 0u };
#endif // DIRECTX
  int32_t m_witdth = 0;
  int32_t m_height = 0;
  int32_t m_channalCount = 0;

};
