#pragma once 
#include "..//include/utiliy/Grafics_libs.h"
// std includes 
#include <cstdint>
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
struct sSamplerDesc;
struct sTextureDescriptor;
struct sDepthStencilDescriptor;
struct sInputDescriptor;

class cDeviceContext
{
public:
  cDeviceContext();

  ~cDeviceContext();

public:// functions 

#if DIRECTX
  //! for interfacing with directX 
  ID3D11DeviceContext *
    getDeviceContext();
  //! for function that need 2 pointers to pointers 
  ID3D11DeviceContext **
    getDeviceContextRef();
  //! resets all things set by device context to default settings 
  void
    ClearState();
#endif // DIRECTX
  /*! set render-targets and can set multiple render-targets
      \param renderTragetsViews [out] the set render-targets 
      \param  depthStencilView [out] the set depth-stencil view(note there can only be 1)
      \param numRenderTargets [in] to tell how many render target there are */
  void
    OMSetRenderTargets(cRenderTargetView renderTragetsViews[],
                       cDepthStencilView &depthStencilView,
                       uint32_t numRenderTragets = 1);
private:
#if DIRECTX
  ID3D11DeviceContext* mptr_deviceContext = nullptr;

#endif // DIRECTX
};

