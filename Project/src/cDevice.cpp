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
//std includes 
#include <vector>

cDevice::cDevice()
  :mptr_device(nullptr)
{}

cDevice::~cDevice()
{
#if DIRECTX
  if (mptr_device != nullptr)
  {
    mptr_device->Release();
  }
#endif // DIRECTX
}

bool cDevice::CreateRenderTargetView(cTexture2D & texture, cRenderTargetView & renderTraget)
{
#if DIRECTX

  HRESULT hr;
  hr = mptr_device->CreateRenderTargetView(texture.getTexture(), NULL, renderTraget.getRenderTragetViewRef());
  if (SUCCEEDED(hr))
  {
    return true;
  }

#endif // 0
  return false;
}

bool cDevice::CreateTexture2D(sTextureDescriptor & Description, cTexture2D & Texture)
{
#ifdef DIRECTX
  HRESULT hr;
  D3D11_TEXTURE2D_DESC descDepth;
  SecureZeroMemory(&descDepth, sizeof(descDepth));
  descDepth.Width = Description.texWidth;
  descDepth.Height = Description.texHeight;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = static_cast<DXGI_FORMAT> (Description.texFormat);//DXGI_FORMAT_D24_UNORM_S8_UINT;//DXGI_FORMAT_D24_UNORM_S8_UINT
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = static_cast<D3D11_USAGE> (Description.Usage);
  descDepth.BindFlags = Description.BindFlags;
  descDepth.CPUAccessFlags = Description.CpuAccess;
  descDepth.MiscFlags = 0;
  hr = mptr_device->CreateTexture2D(&descDepth, NULL, Texture.getTextureRef());
  if (SUCCEEDED(hr))
  {
    return true;
  }
#endif // DIRECTX
  return false;
}


bool cDevice::CreateDepthStencilView(cTexture2D & texture, sDepthStencilDescriptor& Desc, cDepthStencilView & DepthView)
{
#if DIRECTX
  HRESULT hr;
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  SecureZeroMemory(&descDSV, sizeof(descDSV));
  descDSV.Format = static_cast<DXGI_FORMAT>(Desc.Format);
  descDSV.ViewDimension = static_cast<D3D11_DSV_DIMENSION>(Desc.Dimension);
  descDSV.Texture2D.MipSlice = Desc.Mip;

  hr = mptr_device->CreateDepthStencilView(texture.getTexture(), &descDSV, DepthView.getDepthStencilViewRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
#endif // DIRECTX
  return false;
}

bool cDevice::CreateVertexShader(cVertexShader & vertexShader)
{
#if DIRECTX
  HRESULT hr;
  hr = mptr_device->CreateVertexShader(vertexShader.getInfo()->GetBufferPointer(),
                                       vertexShader.getInfo()->GetBufferSize(),
                                       NULL,
                                       vertexShader.getVertexShaderRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
  return false;
#endif // DIRECTX
}

bool cDevice::CreatePixelShader(cPixelShader & pixelShader)
{
#if DIRECTX
  HRESULT hr;
  hr = mptr_device->CreatePixelShader(pixelShader.getInfo()->GetBufferPointer(),
                                      pixelShader.getInfo()->GetBufferSize(),
                                      NULL,
                                      pixelShader.getPixelShaderRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
#endif // DIRECTX
  return false;
}

bool cDevice::CreateInputLayout(cInputLayout &inputLayout
                                , uint32_t NumElements,
                                sInputDescriptor intermediateDesc[],
                                cVertexShader &vertexShader)
{
#if DIRECTX
  HRESULT hr;
  std::vector<D3D11_INPUT_ELEMENT_DESC> ElementDesc;
  for (int i = 0; i < NumElements; ++i)
  {
    D3D11_INPUT_ELEMENT_DESC Desc;
    Desc.Format = static_cast<DXGI_FORMAT>(intermediateDesc[i].Format);
    Desc.AlignedByteOffset = (UINT) intermediateDesc[i].Alignment;
    Desc.SemanticIndex = intermediateDesc[i].Index;
    Desc.SemanticName = intermediateDesc[i].Name.c_str();
    Desc.InputSlot = intermediateDesc[i].Slot;
    Desc.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION> (intermediateDesc[i].SlotClass);
    Desc.InstanceDataStepRate = intermediateDesc[i].InstanceData;
    ElementDesc.emplace_back(Desc);
  }
  hr = mptr_device->CreateInputLayout(&ElementDesc[0],
                                      NumElements,
                                      vertexShader.getInfo()->GetBufferPointer(),
                                      vertexShader.getInfo()->GetBufferSize(),
                                      inputLayout.getInputLayoutRef());
  if (SUCCEEDED(hr))
  {
    return true;
  }
#endif // DIRECTX
  return false;
}

bool cDevice::CreateVertexBuffer(cVertexBuffer & VertexBuffer)
{
  if (VertexBuffer.getBufferType() != BufferType::Vertex)
  {
    OutputDebugStringA("inserted the wrong buffer in CreateVertexBuffer function ");
    return false;
  }
#if DIRECTX
  HRESULT hr;
  D3D11_BUFFER_DESC directxDesc = VertexBuffer.getDirectXDesc();
  D3D11_SUBRESOURCE_DATA subresource = VertexBuffer.getSubresource();

  hr = mptr_device->CreateBuffer(&directxDesc,
                                 &subresource,
                                 VertexBuffer.getBufferRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
  return false;
#endif // DIRECTX
}

bool cDevice::CreateIndexBuffer(cIndexBuffer & indexBuffer)
{
  if (indexBuffer.getBufferType() != BufferType::Index)
  {
    OutputDebugStringA("inserted the wrong buffer in CreateIndexBuffer function ");
    return false;
  }
#if DIRECTX
  HRESULT hr;
  D3D11_BUFFER_DESC directxDesc = indexBuffer.getDirectXDesc();
  D3D11_SUBRESOURCE_DATA subresource = indexBuffer.getSubresource();

  hr = mptr_device->CreateBuffer(&directxDesc,
                                 &subresource,
                                 indexBuffer.getBufferRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
  return false;
#endif // DIRECTX
  return true;
}

bool cDevice::CreateConstBuffer(cConstBuffer & constBuffer)
{
  if (constBuffer.getBufferType() != BufferType::Const)
  {
    OutputDebugStringA("inserted the wrong buffer in CreateIndexBuffer function ");
    return false;
  }
#if DIRECTX
  HRESULT hr;
  D3D11_BUFFER_DESC directxDesc = constBuffer.getDirectXDesc();

  hr = mptr_device->CreateBuffer(&directxDesc,
                                 nullptr,
                                 constBuffer.getBufferRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
  return false;
#endif // DIRECTX
  return true;
}

bool cDevice::CreateSamplerState(cSampler & sampler)
{
#if DIRECTX
  HRESULT hr;
  D3D11_SAMPLER_DESC directxSampler = sampler.getDirectxSamplerDesc();
  hr = mptr_device->CreateSamplerState(&directxSampler,
                                       sampler.getSamplerRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
  return false;
#endif // DIRECTX
  return false;
}


#ifdef DIRECTX
ID3D11Device * cDevice::getDevice()
{
  return mptr_device;
}

ID3D11Device ** cDevice::getDeviceRef()
{
  return &mptr_device;
}
#endif // DIRECTX

