#pragma once
#include "utility/enGraphics.h"

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

