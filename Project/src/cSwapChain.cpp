#include "../include/cSwapChain.h"
#include "..//include/cTexture2D.h"
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


