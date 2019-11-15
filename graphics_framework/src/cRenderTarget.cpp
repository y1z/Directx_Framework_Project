
#include "../include/cRenderTarget.h"
#include <memory>

cRenderTarget::cRenderTarget()
{}

cRenderTarget::~cRenderTarget()
{}

#if DIRECTX

ID3D11Texture2D *
cRenderTarget::getRenderTarget()
{
  return m_target.getTexture();
}

ID3D11Texture2D **
cRenderTarget::getRenderTargetRef()
{
  return  m_target.getTextureRef();
}

D3D11_RENDER_TARGET_VIEW_DESC
cRenderTarget::getDiscriptor()
{
  D3D11_RENDER_TARGET_VIEW_DESC result;
  std::memset(&result, 0, sizeof(result));
  result.Format = static_cast< DXGI_FORMAT >(this->m_desc.format);//DXGI_USAGE_RENDER_TARGET_OUTPUT
  result.ViewDimension = static_cast< D3D11_RTV_DIMENSION >(m_desc.dimension);
  return  result;
}
#elif OPEN_GL

sRenderTarget 
cRenderTarget::getDiscriptor() const
{
  return  m_desc;
}

#endif // DIRECTX

void
cRenderTarget::init(uint32_t width, uint32_t height,
                              int Format, int Type)
{
  m_desc.format = Format;
  m_desc.dimension = Type;

  m_target.init(width, height, Format, 0, 0x40);
}

cTexture2D &
cRenderTarget::getTexture()
{
  return m_target;
}

void
cRenderTarget::Release()
{
#if DIRECTX
  this->m_target.Release();
#endif // DIRECTX
}

