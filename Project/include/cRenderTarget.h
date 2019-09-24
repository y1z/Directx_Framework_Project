#pragma once
#include "../include/utiliy/Grafics_libs.h"
#include "cTexture2D.h"
#include <cstdint>

/*! describe's how the render target interprets data*/
struct sRenderTarget
{
  int format{0};
  int dimension{0};
};

/*! it's the texture where the window is going to render to */
class cRenderTarget
{
public:
  cRenderTarget();
  ~cRenderTarget() ;

public:
#if DIRECTX
  //! the
  ID3D11Texture2D *
    getRenderTarget();

  ID3D11Texture2D **
    getRenderTargetRef();

  //! for interfacing with directX 
  D3D11_RENDER_TARGET_VIEW_DESC
    getDiscriptor();
#endif // DIRECTX

  //! set up a intermediate struct for the description of how to interpret the data
  //\param type the default value is equivalent to D3D11_RTV_DIMENSION_TEXTURE2D aka a 2D texture
  void

    setDescription(uint32_t width, uint32_t height,int Format, int Type = 4);

  /*! to interface with my own functions */
  cTexture2D 
    &getTexture();

private:
#if DIRECTX
  cTexture2D m_target;
#endif // DIRECTX
  sRenderTarget m_desc;
};

