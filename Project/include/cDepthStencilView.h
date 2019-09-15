#pragma once

#include"utiliy/Grafics_libs.h"
struct sDepthStencilDescriptor
{
  int Format{0};
  int Dimension{0};
  int Mip{0};
};

class cDepthStencilView
{
public:
  cDepthStencilView();
  ~cDepthStencilView();
public:
#if DIRECTX

  ID3D11DepthStencilView* getDepthStencilView();
  ID3D11DepthStencilView** getDepthStencilViewRef();
private:
  ID3D11DepthStencilView*mptr_depthStencilView;

#endif // DIRECTX
};
