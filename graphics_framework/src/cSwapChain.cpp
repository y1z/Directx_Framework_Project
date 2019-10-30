
#include "../include/cSwapChain.h"
#include "..//include/cTexture2D.h"
#include "../include/cDevice.h"
#include "../include/cWindow.h"
#include "utility/HelperFuncs.h"

#include <cassert>
cSwapChain::cSwapChain()
#if DIRECTX
  :mptr_swapChain(nullptr)
#elif OPEN_GL
#endif // DIRECTX
{}

cSwapChain::~cSwapChain()
{
#if DIRECTX
  if (mptr_swapChain)
  {
    mptr_swapChain->Release();
  }
#elif OPEN_GL
#endif // DIRECTX
}

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
  result.BufferDesc.Format = static_cast< DXGI_FORMAT >(m_desc.buffFormat);
  result.BufferDesc.RefreshRate.Numerator = m_desc.buffRefershNumaretor;
  result.BufferDesc.RefreshRate.Denominator = m_desc.buffRefershDenominator;
  result.BufferUsage = m_desc.buffUsage;
  result.OutputWindow = m_desc.outputWindow;
  result.SampleDesc.Count = m_desc.sampCount;
  result.SampleDesc.Quality = m_desc.sampQuality;
  result.Windowed = m_desc.isWindowd;
  return  result;
}
#elif OPEN_GL
#endif // DIRECTX

void
cSwapChain::setSwapChain(uint32_t width, uint32_t height,
                         int format, int usage,
                         cWindow & window, uint8_t bufferCount,
                         uint16_t RefreshNumerator, uint16_t RefreshDenominator,
                         uint8_t SampCount, uint8_t SampQuality, bool isWindowed)
{
  m_desc.buffWidth = width;
  m_desc.buffHeight = height;
  m_desc.buffCount = bufferCount;
  m_desc.buffFormat = format;
  m_desc.buffUsage = usage;
  m_desc.sampCount = SampCount;
  m_desc.sampQuality = SampQuality;
  m_desc.buffRefershNumaretor = RefreshNumerator;
  m_desc.buffRefershDenominator = RefreshDenominator;
  m_desc.isWindowd = isWindowed;
  m_desc.outputWindow = window.getHandle();

  //m_StencilView.set
}

void
cSwapChain::setRenderTarget(uint32 width, uint32 height, Formats format)
{
  this->m_renderTarget.setDescription(width, height, format);
}

#if OPEN_GL
void
cSwapChain::setGlWindow(GLFWwindow * newWindow)
{
  mptr_window = newWindow;
}

#endif // OPEN_GL

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

cRenderTargetView
& cSwapChain::getRenderTargerView()
{
  return this->m_renderTargetView;
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
                                 reinterpret_cast< LPVOID* >(backBuffer.getTextureRef()));
  if (SUCCEEDED(hr))
  {
    return true;
  }
  else
  {
    return false;
  }
#elif OPEN_GL
  GlRemoveAllErrors();

  mptr_backBuffer = backBuffer.getIDPtr();
  if (GlCheckForError())
  {
    EN_LOG_ERROR

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

bool
cSwapChain::Present(uint32_t SycroOption, uint32_t PresentationOption, unsigned int Program)
{
#if DIRECTX
  HRESULT hr;
  hr = mptr_swapChain->Present(SycroOption, PresentationOption);
  if (SUCCEEDED(hr))
  {
    return true;
  }
#elif OPEN_GL
  glfwPollEvents();
  if (mptr_window != nullptr)
  {

    glfwSwapBuffers(mptr_window);
    return true;
  }
  else { return false; }
#endif // DIRECTX
  return false;
}

bool
cSwapChain::Resize(cDevice &device,
                   uint32_t width, uint32_t height)
{
#if DIRECTX
  bool isSuccesful = false;
  this->m_depthStencilView.ReleaseAll();
  //DepthStencilView.ReleaseAll();
  this->m_renderTarget.Release();
  this->m_renderTargetView.Release();
  //this->m_StencilView.getDepthStencil().Release();

  this->m_desc.buffWidth = width;
  this->m_desc.buffHeight = height;
  this->m_depthStencilView.getDepthStencil().setDescriptor(width, height, Formats::depthStencil_format /*45*/, 0, 0x40L);

  mptr_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

  isSuccesful = this->InitBuffer();
  // isSuccesful = this->GetBuffer(renderTraget, 0);
  assert(("error with Getting buffer " && isSuccesful == true));

  isSuccesful = device.CreateRenderTargetView(this->m_renderTarget.getTexture(), this->m_renderTargetView);
  assert(("error with render traget view " && isSuccesful == true));

  isSuccesful = device.CreateTexture2D(m_depthStencilView.m_depthStencil.m_desc, m_depthStencilView.getDepthStencil());

  assert(("error with Texture 2d" && isSuccesful == true));

  //sDepthStencilDescriptor depthDesc;
  //depthDesc.Format = 45;
  //depthDesc.Dimension = 3;// equivalent to D3D11_DSV_DIMENSION_TEXTURE2D 
  //depthDesc.Mip = 0;

  m_depthStencilView.init(45, 3);
  //DepthStencilView.init(45, 3);

  isSuccesful = device.CreateDepthStencilView(m_depthStencilView);
  assert(("error Creating the depth-stencil view" && isSuccesful == true));

  return isSuccesful;
#elif OPEN_GL
#endif // DIRECTX
  return false;
}

