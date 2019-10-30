#pragma once
#include "utility/Grafics_libs.h"


enum class bufferUsage
{
#if DIRECTX
  renderTragetOut = DXGI_USAGE_RENDER_TARGET_OUTPUT,
  backBuffer = DXGI_USAGE_BACK_BUFFER,
#elif OPEN_GL
renderTragetOut, 
backBuffer, 
#endif // DIRECTX
};
