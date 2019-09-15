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
 #endif // DIRECTX 