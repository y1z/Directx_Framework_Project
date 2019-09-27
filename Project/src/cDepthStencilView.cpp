#include "cDepthStencilView.h"

cDepthStencilView::cDepthStencilView()
#if DIRECTX
  :mptr_depthStencilView(nullptr)

#endif // DIRECTRX
{}


cDepthStencilView::~cDepthStencilView()
{
#if DIRECTX
  if (mptr_depthStencilView != nullptr)
  {
    mptr_depthStencilView->Release();
  }

#endif // DIRECTX
}

#if DIRECTX
ID3D11DepthStencilView * cDepthStencilView::getDepthStencilView()
{
  return mptr_depthStencilView;
}

ID3D11DepthStencilView ** cDepthStencilView::getDepthStencilViewRef()
{
  return &mptr_depthStencilView;
}

ID3D11Texture2D * 
cDepthStencilView::getTexture()
{
  return m_depthStencil.getTexture();
}

ID3D11Texture2D ** 
cDepthStencilView::getTextureRef()
{
  return m_depthStencil.getTextureRef();
}
 #endif // DIRECTX 

cTexture2D & 
cDepthStencilView::getDepthStencil()
{
  return m_depthStencil;
}

void cDepthStencilView::ReleaseAll()
{
  m_depthStencil.Release();
  mptr_depthStencilView->Release();
  mptr_depthStencilView = nullptr;
}
