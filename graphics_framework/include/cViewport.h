#pragma once
#include "../include/utility/Grafics_libs.h"
#include <cinttypes>
//! intermediate for using it in multiple API's  
struct sViewportDesc
{
  float width{1.0f};
  float height{1.0f};
  float minDepth{0.0f};
  float maxDepth{0.0f};
  float TopLeftX{0.0f};
  float TopLeftY{0.0f};
};

//! will control the view port
class cViewport
{
public:
  cViewport();

  ~cViewport();
public:
  void/*! sets all the values of the viewport*/
    setViewport(float width, float height,
                float minDepth, float maxDepth,
                float TopLeftX = 0.0f, float TopLeftY = 0.0f);

#if DIRECTX
  //! returns the descriptor for the view-port
  D3D11_VIEWPORT
    getViewport();

#endif // DIRECTX
private:
  sViewportDesc m_viewportDesc;
#if DIRECTX
  D3D11_VIEWPORT m_viewport;
#endif // DIRECTX
};
