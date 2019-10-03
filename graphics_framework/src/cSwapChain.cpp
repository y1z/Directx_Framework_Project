
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
cSwapChain::setSwapChain(uint32_t width, uint32_t height,
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

  //m_StencilView.set
}

void
cSwapChain::setRenderTarget(uint32 width, uint32 height, Formats format)
{
  this->m_renderTarget.setDescription(width, height, format);
}

cRenderTarget &
cSwapChain::getRenderTarget()
{
  return m_renderTarget;
}

cDepthStencilView
& cSwapChain::getDepthStencilView()
{
  return this->m_depthStencilView;
}

cTexture2D &
cSwapChain::getDepthStencil()
{
  return this->m_depthStencilView.getDepthStencil();
}

void
cSwapChain::setDepthStencilView(Formats format, int Dimension, int Mip)
{
  this->m_depthStencilView.init(format, Dimension, Mip);
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

bool cSwapChain::InitBuffer()
{
  bool isSucceful = this->GetBuffer(this->m_renderTarget.getTexture(), 0);
  return isSucceful;
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

bool
cSwapChain::Resize(cDevice &device,
                   cRenderTargetView &renderTragetView,
                   HWND handle, uint32_t width, uint32_t height)
{
#if DIRECTX
  bool isSuccesful = false;
  this->m_depthStencilView.ReleaseAll();
  //DepthStencilView.ReleaseAll();
  this->m_renderTarget.Release();
  renderTragetView.Release();
  //this->m_StencilView.getDepthStencil().Release();

  this->m_desc.buffWidth = width;
  this->m_desc.buffHeight = height;

  this->m_depthStencilView.getDepthStencil().setDescriptor(width, height, 45, 0, 0x40L);
  // renderTraget.setDescriptor(width, height, 45, 0, 0x40L);

  mptr_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

  isSuccesful = this->InitBuffer();
  // isSuccesful = this->GetBuffer(renderTraget, 0);
  assert(("error with Getting buffer " && isSuccesful == true));

  isSuccesful = device.CreateRenderTargetView(this->m_renderTarget.getTexture(), renderTragetView);
  assert(("error with render traget view " && isSuccesful == true));

  sTextureDescriptor TextureDesc;
  memset(&TextureDesc, 0, sizeof(TextureDesc));
  TextureDesc.texHeight = height;
  TextureDesc.texWidth = width;
  TextureDesc.texFormat = Formats::depthStencil_format;// equivalent to DXGI_FORMAT_D24_UNORM_S8_UINT
  TextureDesc.BindFlags = 0x40L;// equivalent to DD3D11_BIND_DEPTH_STENCIL
  TextureDesc.Usage = 0;// equivalent to D3D11_USAGE_DEFAULT
  TextureDesc.CpuAccess = 0;
  TextureDesc.arraySize = 1;

  isSuccesful = device.CreateTexture2D(TextureDesc, m_depthStencilView.getDepthStencil());
  assert(("error with Texture 2d" && isSuccesful == true));

  //sDepthStencilDescriptor depthDesc;
  //depthDesc.Format = 45;
  //depthDesc.Dimension = 3;// equivalent to D3D11_DSV_DIMENSION_TEXTURE2D 
  //depthDesc.Mip = 0;

  m_depthStencilView.init(45, 3);
  //DepthStencilView.init(45, 3);

  isSuccesful = device.CreateDepthStencilView(m_depthStencilView);
  assert(("error Creating the depth-stencil view" && isSuccesful == true));

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
