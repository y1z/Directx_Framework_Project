#include "cDeviceContext.h"
// implementations
#include "include/cDevice.h"
#include "include/cRenderTargetView.h"
#include "include/cDepthStencilView.h"
#include "include/cTexture2D.h"
#include "include/cVertexShader.h"
#include "include/cPixelShader.h"
#include "include/cInputLayout.h"
#include "include/cVertexBuffer.h"
#include "include/cIndexBuffer.h"
#include "include/cConstBuffer.h"
#include "include/cSampler.h"
// std includes 
#include <cassert>
constexpr const uint16_t c_MaxRenderTargets = 8;

cDeviceContext::cDeviceContext()
#if DIRECTX
  :mptr_deviceContext(nullptr)

#endif // DIRECTX
{}

cDeviceContext::~cDeviceContext()
{
#if DIRECTX
  if (mptr_deviceContext)
  {
    mptr_deviceContext->Release();
  }

#endif // DIRECTX
}

#if DIRECTX

ID3D11DeviceContext *
cDeviceContext::getDeviceContext()
{
  return mptr_deviceContext;
}

ID3D11DeviceContext **
cDeviceContext::getDeviceContextRef()
{
  return &mptr_deviceContext;
}

void
cDeviceContext::ClearState()
{ mptr_deviceContext->ClearState(); }

#endif // DIRECTX

void
cDeviceContext::OMSetRenderTargets(cRenderTargetView renderTargetsViews[],
                                   cDepthStencilView &depthStencilViews,
                                   uint32_t numRenderTargets)
{
#if DIRECTX
  ID3D11RenderTargetView *RenderTempPtrArr[c_MaxRenderTargets];
  if (numRenderTargets <= c_MaxRenderTargets)
  {
    for (uint8_t i = 0; i < numRenderTargets; ++i)
    {
      RenderTempPtrArr[i] = renderTargetsViews[i].getRenderTragetView();
    }

    mptr_deviceContext->OMSetRenderTargets(numRenderTargets,
                                           RenderTempPtrArr,
                                           depthStencilViews.getDepthStencilView());
  }
  else
  {
    assert("Error two many render targets", numRenderTargets <= c_MaxRenderTargets);
  }
#endif // DIRECTX

}

