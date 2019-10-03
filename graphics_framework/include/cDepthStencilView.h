#pragma once

#include"utility/Grafics_libs.h"
#include "../include/cTexture2D.h"

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

  ID3D11Texture2D * 
    getTexture();

  ID3D11Texture2D ** 
    getTextureRef();
#endif // DIRECTX
  sDepthStencilDescriptor 
    getDescriptor() const;

  void 
    init(int Format,int Dimension = 3, int mip = 0);

  cTexture2D 
    &getDepthStencil();

  void 
    ReleaseAll();
private:
#if DIRECTX
  ID3D11DepthStencilView*mptr_depthStencilView;
#endif // DIRECTX
  sDepthStencilDescriptor m_Descriptor;
  cTexture2D m_depthStencil;
};
