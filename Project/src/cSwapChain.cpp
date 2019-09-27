#include "../include/cSwapChain.h"
#include "..//include/cTexture2D.h"
#include "../include/cDevice.h"
#include <cassert>
cSwapChain::cSwapChain()
#if DIRECTX
  :mptr_swapChain(nullptr)
#endif // DIRECTX
{}

cSwapChain::~cSwapChain()
{
#if DIRECTX
  if (mptr_swapChain)
  {
    mptr_swapChain->Release();
  }

}
#endif // DIRECTX

#if DIRECTX

IDXGISwapChain
* cSwapChain::getSwapChain()
{
  return mptr_swapChain;
}

IDXGISwapChain
** cSwapChain::getSwapChainRef()
{
  return &mptr_swapChain;
}

DXGI_SWAP_CHAIN_DESC
cSwapChain::getSwapChainDesc()
{
  DXGI_SWAP_CHAIN_DESC result;
  SecureZeroMemory(&result, sizeof(result));
  result.BufferCount = m_desc.buffCount;
  result.BufferDesc.Width = m_desc.buffWidth;
  result.BufferDesc.Height = m_desc.buffHeight;
  result.BufferDesc.Format = static_cast<DXGI_FORMAT>(m_desc.buffFormat);
  result.BufferDesc.RefreshRate.Numerator = m_desc.buffRefershNumaretor;
  result.BufferDesc.RefreshRate.Denominator = m_desc.buffRefershDenominator;
  result.BufferUsage = m_desc.buffUsage;
  result.OutputWindow = m_desc.outputWindow;
  result.SampleDesc.Count = m_desc.sampCount;
  result.SampleDesc.Quality = m_desc.sampQuality;
  result.Windowed = m_desc.isWindowd;
  return  result;
}

#endif // DIRECTX

void
cSwapChain::setDescription(uint32_t width, uint32_t height,
                           int format, int usage,
                           HWND handle, uint8_t bufferCount,
                           uint16_t RefershNumerator, uint16_t RefershDenominator,
                           uint8_t SampCount, uint8_t SampQuality,
                           bool isWindowed)
{
  m_desc.buffWidth = width;
  m_desc.buffHeight = height;
  m_desc.buffCount = bufferCount;
  m_desc.buffFormat = format;
  m_desc.buffUsage = usage;
  m_desc.sampCount = SampCount;
  m_desc.sampQuality = SampQuality;
  m_desc.buffRefershNumaretor = RefershNumerator;
  m_desc.buffRefershDenominator = RefershDenominator;
  m_desc.isWindowd = isWindowed;
  m_desc.outputWindow = handle;
}

bool
cSwapChain::GetBuffer(cTexture2D & backBuffer, uint32_t bufferIndex)
{
#if DIRECTX
  HRESULT hr;
  hr = mptr_swapChain->GetBuffer(bufferIndex,
                                 __uuidof(ID3D11Texture2D),
                                 reinterpret_cast<LPVOID*>(backBuffer.getTextureRef()));
  if (SUCCEEDED(hr))
  {
    return true;
  }
  else
  {
    return false;
  }
#endif // DIRECTX 
  return true;
}

bool cSwapChain::Present(uint32_t SycroOption, uint32_t PresentationOption, unsigned int Program)
{
#if DIRECTX
  HRESULT hr;
  hr = mptr_swapChain->Present(SycroOption, PresentationOption);
  if (SUCCEEDED(hr))
  {
    return true;
  }

#endif // DIRECTX
  return false;
}

bool cSwapChain::Resize(cDevice &device, cDepthStencilView &DepthStencilView
                        , cTexture2D &renderTraget, cRenderTargetView &renderTragetView,
                        HWND &handle, uint32_t width, uint32_t height)
{
#if DIRECTX
  bool isSuccesful = false;
  DepthStencilView.ReleaseAll();
  renderTragetView.getRenderTragetView()->Release();
  renderTraget.Release();

  this->m_desc.buffWidth = width;
  this->m_desc.buffHeight = height;

  renderTraget.setDescriptor(width, height, 45, 0, 0x40L);

  mptr_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

  isSuccesful = this->GetBuffer(renderTraget, 0);
  assert(("error with Getting buffer " && isSuccesful == true));

  isSuccesful = device.CreateRenderTargetView(renderTraget, renderTragetView);
  assert(("error with render traget view " && isSuccesful == true));

  isSuccesful = device.CreateTexture2D(renderTraget.getDescriptor(), renderTraget);
  assert(("error with Texture 2d" && isSuccesful == true));

  sDepthStencilDescriptor depthDesc;
  depthDesc.Format = 45;
  depthDesc.Dimension = 3;// equivalent to D3D11_DSV_DIMENSION_TEXTURE2D 
  depthDesc.Mip = 0;

  isSuccesful = device.CreateDepthStencilView(DepthStencilView.getDepthStencil(), depthDesc,
                                              DepthStencilView);
  assert(("error Creating the depth-stencil view" && isSuccesful == true));
  // device.getDevice()->CreateDepth;

   //DXGI_SWAP_CHAIN_DESC sd;
   //SecureZeroMemory(&sd, sizeof(sd));
   //sd.BufferCount = 1;
   //sd.BufferDesc.Width = width;
   //sd.BufferDesc.Height = height;
   //sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   //sd.BufferDesc.RefreshRate.Numerator = 60;
   //sd.BufferDesc.RefreshRate.Denominator = 1;
   //sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   //sd.OutputWindow = handle;
   //sd.SampleDesc.Count = 1;
   //sd.SampleDesc.Quality = 0;
   //sd.Windowed = TRUE;
  return isSuccesful;
#endif // DIRECTX
  return false;
}


