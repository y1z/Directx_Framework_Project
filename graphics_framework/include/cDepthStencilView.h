#pragma once

#include"utility/enGraphics.h"
#include "../include/cTexture2D.h"
#include "utility/enDefs.h"

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

#elif OPEN_GL

  uint32
    getDepthStencilID() const;

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
  ID3D11DepthStencilView *mptr_depthStencilView;
#elif OPEN_GL 

  uint32 m_depthStencilID = 0;
#endif // DIRECTX

  sDepthStencilDescriptor m_Descriptor;
public:
  cTexture2D m_depthStencil;
};
