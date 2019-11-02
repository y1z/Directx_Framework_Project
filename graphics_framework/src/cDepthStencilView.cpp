
#include "cDepthStencilView.h"

cDepthStencilView::cDepthStencilView()
#if DIRECTX
  :mptr_depthStencilView(nullptr)
#elif OPEN_GL
  :m_depthStencilID(0)
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
#elif OPEN_GL

uint32 
cDepthStencilView::getDepthStencilID() const
{
  return m_depthStencilID;
}
#endif // DIRECTX 

sDepthStencilDescriptor
cDepthStencilView::getDescriptor() const
{
  return this->m_Descriptor;
}

void
cDepthStencilView::init(int Format, int Dimension, int mip)
{
  m_Descriptor.Format = Format;
  m_Descriptor.Dimension = Dimension;
  m_Descriptor.Mip = mip;
#if OPEN_GL
  glGenRenderbuffers(1, &m_depthStencilID);

#endif // OPEN_GL

}

cTexture2D &
cDepthStencilView::getDepthStencil()
{
  return m_depthStencil;
}

void
cDepthStencilView::ReleaseAll()
{
  m_depthStencil.Release();
#if DIRECTX
  mptr_depthStencilView->Release();
  mptr_depthStencilView = nullptr;
#elif OPEN_GL
#endif // DIRECTX
}
