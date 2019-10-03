#pragma once
#include "../include/utility/Grafics_libs.h"
#include <cstdint>
/*!
\todo Finish add all methods used in the tutorial 
\brief this class will be used to create resources
*/

// Forward decelerations 
class cTexture2D;
class cRenderTargetView;
class cDepthStencilView;
class cVertexShader;
class cPixelShader;
class cInputLayout;
class cVertexBuffer;
class cIndexBuffer;
class cConstBuffer;
class cSampler;
class cRenderTarget;
struct sRenderTarget;
struct sSamplerDesc;
struct sTextureDescriptor;
struct sDepthStencilDescriptor;
struct sInputDescriptor;

class cDevice
{
public:
  cDevice();
  ~cDevice();
public:
//! return true if it succeeds
 bool CreateRenderTargetView(cTexture2D &texture, cRenderTargetView &renderTraget);

 bool CreateRenderTargetView(cRenderTarget &renderTarget, cRenderTargetView &renderTraget);
 //! creates a 2D texture 
 bool CreateTexture2D(sTextureDescriptor &Description, cTexture2D &Texture);
 //!  Creates a depth view using a texture as the buffer 
 bool CreateDepthStencilView(cDepthStencilView &Depth);
//! creates a vertex shader 
 bool CreateVertexShader(cVertexShader &vertexShader);
//! creates a pixel shader 
 bool CreatePixelShader(cPixelShader &vertexShader);
/*!Creates an input layout ( meant for when making the input layout manually)
\param inputLayout contains the input layout 
\param NumDesc how many arguments the input layout will take 
\param Desc tell the input layout how the data is organized 
*/ 
 bool CreateInputLayout(cInputLayout &inputLayout
                        ,uint32_t NumDesc
                        , sInputDescriptor Desc[]
                        ,cVertexShader &vertexShader);
/*!*/ 
 bool CreateInputLayout(cInputLayout &inputLayout, cVertexShader &vertexShader);

//! Creates a vertex buffer 
 bool CreateVertexBuffer(cVertexBuffer &VertexBuffer);
 //! Creates a index buffer 
 bool CreateIndexBuffer(cIndexBuffer &indexBuffer);
//! Creates a Const buffer 
 bool CreateConstBuffer(cConstBuffer &constBuffer);
 //! Creates a sampler 
 bool CreateSamplerState(cSampler &sampler);

#ifdef DIRECTX
  ID3D11Device * getDevice();
  ID3D11Device **getDeviceRef();
private:
  ID3D11Device *mptr_device = nullptr;
#else 
#endif 
};

