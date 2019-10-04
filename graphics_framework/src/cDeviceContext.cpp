
#include "cDeviceContext.h"
// implementations
#include "../include/cDevice.h"
#include "../include/cRenderTargetView.h"
#include "../include/cDepthStencilView.h"
#include "../include/cTexture2D.h"
#include "../include/cVertexShader.h"
#include "../include/cPixelShader.h"
#include "../include/cInputLayout.h"
#include "../include/cVertexBuffer.h"
#include "../include/cIndexBuffer.h"
#include "../include/cConstBuffer.h"
#include "../include/cSampler.h"
#include "../include/cViewport.h"
#include "../include/cBuffer.h"
#include "../include/cShaderResourceView.h"
#include "../include/cMesh.h"
// std includes 
#include <cassert>
// limits for setting objects 
static constexpr const uint8_t c_MaxRenderTargets = 8;
static constexpr const uint8_t c_MaxViewPorts = 8;
static constexpr const uint8_t c_MaxVertexBuffers = 24;

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
    assert(("Error two many render targets", numRenderTargets <= c_MaxRenderTargets));
  }
#endif // DIRECTX

}

void
cDeviceContext::RSSetViewports(cViewport viewports[], uint8_t numViewports)
{
#if DIRECTX
  if (numViewports <= c_MaxViewPorts)
  {
    D3D11_VIEWPORT viewportsArr[c_MaxViewPorts];
    for (uint8_t i = 0; i < numViewports; ++i)
    {
      viewportsArr[i] = viewports[i].getViewport();
    }
    mptr_deviceContext->RSSetViewports(numViewports, viewportsArr);
  }
  else
  {
    assert(("Error too many view-ports"&& numViewports <= c_MaxViewPorts));
  }
#endif // DIRECTX

}

void
cDeviceContext::IASetInputLayout(cInputLayout & inputLayout)
{
#if DIRECTX
  mptr_deviceContext->IASetInputLayout(inputLayout.getInputLayout());
#endif // DIRECTX
}

void
cDeviceContext::IASetVertexBuffers(cVertexBuffer vertexBuffer[], uint32_t numBuffers, uint32_t Slot)
{
#if DIRECTX

  if (numBuffers <= c_MaxVertexBuffers)
  {
    ID3D11Buffer *BufferTempPtrArr[c_MaxVertexBuffers];
    UINT strides[c_MaxVertexBuffers];
    UINT offSets[c_MaxVertexBuffers]{0}; // use later 
    for (uint32_t i = 0; i < numBuffers; ++i)
    {
      BufferTempPtrArr[i] = vertexBuffer[i].getBuffer();
      strides[i] = vertexBuffer->getStride();
    }
    mptr_deviceContext->IASetVertexBuffers(Slot, numBuffers,
                                           BufferTempPtrArr, strides,
                                           offSets);
  }
  else
  {
    assert(("Error too many vertex buffer ", numBuffers <= c_MaxVertexBuffers));
  }

#endif // DIRECTX
}

void cDeviceContext::IASetIndexBuffer(cIndexBuffer & indexBuffer, int Format, int offSet)
{
#if DIRECTX
  mptr_deviceContext->IASetIndexBuffer(indexBuffer.getBuffer(), static_cast<DXGI_FORMAT>(Format), offSet);
#endif // DIRECTX
}

void cDeviceContext::IASetPrimitiveTopology(int Topology)
{
#if DIRECTX
  mptr_deviceContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(Topology));
#endif // DIRECTX
}

void cDeviceContext::UpdateSubresource(cBuffer * Buffer, const void * originOfData)
{
#if DIRECTX
  mptr_deviceContext->UpdateSubresource(Buffer->getBuffer(), 0, nullptr, originOfData, 0, 0);

#endif // DIRECTX
}

void cDeviceContext::ClearRenderTargetView(cRenderTargetView & renderTargetView, float color[4])
{
#if DIRECTX
  if (color == nullptr)
  {
    // han purple https://rgbcolorcode.com/color/6619FF
    float ClearColor[4] = {0.40f,0.10f,1.0f,1.0f};
    mptr_deviceContext->ClearRenderTargetView(renderTargetView.getRenderTragetView(), ClearColor);
  }
  else
  {
    mptr_deviceContext->ClearRenderTargetView(renderTargetView.getRenderTragetView(), color);
  }

#endif // DIRECTX
}

void cDeviceContext::ClearDepthStencilView(cDepthStencilView & depthStencilView, bool ClearStencil,
                                           bool ClearDepth, uint8_t StencilClearValues,
                                           float DepthClearValue)
{
#if DIRECTX
  int clearFlags = 0x00;
  // adding the flags for the directX api
  if (ClearStencil) { clearFlags |= D3D11_CLEAR_STENCIL; }
  if (ClearDepth) { clearFlags |= D3D11_CLEAR_DEPTH; }

  mptr_deviceContext->ClearDepthStencilView(depthStencilView.getDepthStencilView(), clearFlags,
                                            DepthClearValue, StencilClearValues);

#endif // DIRECTX
}

void cDeviceContext::VSSetShader(cVertexShader & vertexShader)
{
#if DIRECTX
  mptr_deviceContext->VSSetShader(vertexShader.getVertexShader(), nullptr,
                                  0);
#endif // DIRECTX
}

void cDeviceContext::VSSetConstantBuffers(cConstBuffer & Buffer, uint8_t Slot)
{
#if DIRECTX
  // make sure i don't use more slot than directX has 
  if (Slot <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
  {
    this->mptr_deviceContext->VSSetConstantBuffers(Slot, 1, Buffer.getBufferRef());
  }
  else
  {
    assert("Error used too many slot " && Slot <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1);
  }
#endif // DIRECTX

}

void cDeviceContext::PSSetShader(cPixelShader & pixelShader)
{
#if DIRECTX
  mptr_deviceContext->PSSetShader(pixelShader.getPixelShader(), nullptr, 0);
#endif // DIRECTX
}

void cDeviceContext::PSSetShaderResources(cShaderResourceView shaderResources[], uint32_t numResources, uint32_t Slots)
{
#if DIRECTX
  // make sure i don't use too many slots 
  if (Slots <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 &&
      numResources <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)
  {

    ID3D11ShaderResourceView* ShaderPtrArr[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
    for (uint8_t i = 0; i < numResources; ++i)
    {
      ShaderPtrArr[i] = shaderResources[i].getShaderResource();
    }

    mptr_deviceContext->PSSetShaderResources(Slots,
                                             numResources,
                                             ShaderPtrArr);

  }
  else
  {
    assert(("Error asking for too many slots", Slots <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1));
  }

#endif // DIRECTX
}

void cDeviceContext::PSSetShaderResources(cMesh & ShaderResources, uint32_t Slot)
{
#if DIRECTX
  mptr_deviceContext->PSSetShaderResources(Slot, 1, ShaderResources.getResourceRef());

#endif // DIRECTX
}

void cDeviceContext::PSSetConstantBuffers(cConstBuffer & Buffer, uint32_t Slots)
{
#if DIRECTX
  if (Slots <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
  {
    mptr_deviceContext->PSSetConstantBuffers(Slots,
                                             1,
                                             Buffer.getBufferRef());
  }
  else
  {
    assert(("Error setting PSSetConstantBuffer ", Slots <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1));
  }

#endif // DIRECTX
}

void cDeviceContext::PSSetSamplers(cSampler samplers[], uint32_t numSamplers, uint32_t slot)
{
#if DIRECTX
  if (slot <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 &&
      numSamplers <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)
  {
    ID3D11SamplerState *SamplerPtrArr[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
    for (uint8_t i = 0; i < numSamplers; ++i)
    {
      SamplerPtrArr[i] = samplers[i].getSampler();
    }
    mptr_deviceContext->PSSetSamplers(slot, numSamplers, SamplerPtrArr);
  }
  else
  {
    assert(("Error setting Sampler PSSetSamplers", slot <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1));
  }
#endif // DIRECTX
}

void cDeviceContext::DrawIndexed(uint32_t indexCount, uint32_t indexOffset, unsigned int Program)
{
#if DIRECTX
  mptr_deviceContext->DrawIndexed(indexCount, 0, 0);
#endif // DIRECTX
}


