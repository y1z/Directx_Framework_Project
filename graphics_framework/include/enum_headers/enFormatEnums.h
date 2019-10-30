#pragma once
#include "utility/Grafics_libs.h"

enum Formats
{
#if DIRECTX
  /* one channel */
  uR8 = DXGI_FORMAT_R8_UINT,
  uR16 = DXGI_FORMAT_R16_UINT,
  fR16 = DXGI_FORMAT_R16_FLOAT,
  uR32 = DXGI_FORMAT_R32_UINT,
  fR32 = DXGI_FORMAT_R32_FLOAT,
/*two channel */
fR16G16 = DXGI_FORMAT_R16G16_FLOAT,
/* three channel */

/*Four channel */
R8G8B8A8_uniform_norm = DXGI_FORMAT_R8G8B8A8_UNORM,
fR16G16B16A16 = DXGI_FORMAT_R16G16B16A16_FLOAT,
fR32G32B32A32 = DXGI_FORMAT_R32G32B32A32_FLOAT,
/* other */
depthStencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT
#elif OPEN_GL//TODO : GL
  /* one channel */
  uR8 = GL_UNSIGNED_BYTE,
  uR16 = GL_UNSIGNED_SHORT,
  fR16,
  uR32 = GL_UNSIGNED_INT,
  fR32 = GL_FLOAT,
/*two channel */
fR16G16,
/* three channel */

/*Four channel */
R8G8B8A8_uniform_norm,
fR16G16B16A16,
fR32G32B32A32,
/* other */
depthStencil_format = GL_DEPTH24_STENCIL8,
#else

  uR8,
  uR16,
  fR16,
  uR32,
  fR32,
/*two channel */
fR16G16,
/* three channel */

/*Four channel */
R8G8B8A8_uniform_norm,
fR16G16B16A16,
fR32G32B32A32,
/* other */
depthStencil_format,
#endif // DIRECTX
};

