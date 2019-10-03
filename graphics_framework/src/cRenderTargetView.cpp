
#include "../include/cRenderTargetView.h"

cRenderTargetView::cRenderTargetView()
#if DIRECTX
  :mptr_renderTragetView(nullptr)
#endif // DIRECTX
{}


cRenderTargetView::~cRenderTargetView()
{
  if (mptr_renderTragetView)
  {
    mptr_renderTragetView->Release();
  }
}

#if DIRECTX
ID3D11RenderTargetView *
cRenderTargetView::getRenderTragetView()
{
  return mptr_renderTragetView;
}

ID3D11RenderTargetView **
cRenderTargetView::getRenderTragetViewRef()
{
  return &mptr_renderTragetView;
}
#endif // DIRECTX


void cRenderTargetView::Release()
{
#if DIRECTX
  if (mptr_renderTragetView)
  {
    this->mptr_renderTragetView->Release();
    this->mptr_renderTragetView = nullptr;
  }
#endif // DIRECTX
}
