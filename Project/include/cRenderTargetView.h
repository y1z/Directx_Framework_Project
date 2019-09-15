#pragma once
#include "include/utiliy/Grafics_libs.h"

class cRenderTargetView
{
public:
  cRenderTargetView();
  ~cRenderTargetView();

  ID3D11RenderTargetView * getRenderTragetView();
  ID3D11RenderTargetView ** getRenderTragetViewRef();

private:
  ID3D11RenderTargetView *mptr_renderTragetView;
};

