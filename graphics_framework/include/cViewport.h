#pragma once
#include "../include/utility/Grafics_libs.h"
#include "utility/enDefs.h"
//! intermediate for using it in multiple API's  
struct sViewportDesc
{
  uint32 width{1u};
  uint32 height{1u};
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
    setViewport(uint32 width, uint32 height,
                float minDepth, float maxDepth,
                float TopLeftX = 0.0f, float TopLeftY = 0.0f);

#if DIRECTX
  //! returns the descriptor for the view-port
  D3D11_VIEWPORT
    getViewport();
#elif OPEN_GL 
  sViewportDesc
  getViewport() const ;
#endif // DIRECTX
  int32
  getWidth()const;

  int32
  getHeight()const;
private:
  sViewportDesc m_viewportDesc;
#if DIRECTX
  D3D11_VIEWPORT m_viewport;
#endif // DIRECTX
};
