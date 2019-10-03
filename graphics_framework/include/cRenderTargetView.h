#pragma once
#include "../include/utility/Grafics_libs.h"

/**/
class cRenderTargetView
{
public:
  cRenderTargetView();
  ~cRenderTargetView();

#if DIRECTX
  ID3D11RenderTargetView
    * getRenderTragetView();

  ID3D11RenderTargetView
    ** getRenderTragetViewRef();

#endif // DIRECTX
  void Release();

private:
#if DIRECTX
  ID3D11RenderTargetView 
    *mptr_renderTragetView;
#endif // DIRECTX
};

