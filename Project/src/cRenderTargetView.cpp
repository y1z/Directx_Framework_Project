#include "include/cRenderTargetView.h"



cRenderTargetView::cRenderTargetView()
  :mptr_renderTragetView(nullptr)
{}


cRenderTargetView::~cRenderTargetView()
{}

ID3D11RenderTargetView * cRenderTargetView::getRenderTragetView()
{
  return mptr_renderTragetView;
}

ID3D11RenderTargetView ** cRenderTargetView::getRenderTragetViewRef()
{
  return &mptr_renderTragetView;
}
