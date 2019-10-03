#pragma once
#include "utility/Grafics_libs.h"

enum Formats
{
#if DIRECTX
  /* one channel */
  R16 = DXGI_FORMAT_R16_UINT,
  fR16 = DXGI_FORMAT_R16_FLOAT,
  R32 = DXGI_FORMAT_R32_UINT,
  fR32 = DXGI_FORMAT_R32_FLOAT,
/*two channel */
  fR16G16 = DXGI_FORMAT_R16G16_FLOAT,
/* three channel */

/*Four channel */
  R8G8B8A8_uniform_norm = DXGI_FORMAT_R8G8B8A8_UNORM,
  fR16G16B16A16 = DXGI_FORMAT_R16G16B16A16_FLOAT,
  fR32G32B32A32 = DXGI_FORMAT_R32G32B32A32_FLOAT,
/* other */
  depthStencil_format= DXGI_FORMAT_D24_UNORM_S8_UINT
#else//TODO : GL
  /* one channel */
  R16,
  fR16,
  R32,
  fR32,
/*two channel */
  fR16G16,
/* three channel */

/*Four channel */
  R8G8B8A8_uniform_norm,
  fR16G16B16A16,
  fR32G32B32A32,
/* other */
  D24_uniform_S8_UINT,
#endif // DIRECTX
};

