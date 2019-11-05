#pragma once 
#include "utility/enGraphics.h"
#include "utility/enDefs.h"
#include <string_view>

class cDevice;
class cDeviceContext;


struct sResource2D
{
  uint32 mipsLevel{ 1u};
  uint32 bestMipsLevel{ 0u};

  //! for  when we are dealing with multiple resources aka a texture array
  uint32 size{ 1u};

  //! to know where to start when dealing with multiple  texture arrays 
  uint32 startingIndex{ 0u};
};

/*! intermediate descriptor intended for use
*in both a.p.i's */
struct
  sShaderResourceDesc2D
{
  enFormats format = enFormats::fR32G32B32A32;
  sResource2D resource;
  int32 viewDim;
};



class
  cShaderResourceView
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

  /*! returns a descriptor for the directX a.p.i*/
  D3D11_SHADER_RESOURCE_VIEW_DESC
    getDxDescriptor();

#endif // DIRECTX

  /*! loads an images from a file to create a shader resource*/
  bool
    createShaderResourceFromFile(std::string_view file, cDevice &device, cDeviceContext &deviceContext);

  /*! returns a pointer to the data of the resource*/
  uint8 *
    getResourceData() const;

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
    init(enFormats format,uint32 MipsLevel,uint32 BestMipsLevel , int32 viewDim);
private:
  sShaderResourceDesc2D m_desc;

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
