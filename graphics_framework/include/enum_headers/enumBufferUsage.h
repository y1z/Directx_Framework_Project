#pragma once
#include "utility/enGraphics.h"


enum class enBufferUse
{
#if DIRECTX
  renderTragetOut = DXGI_USAGE_RENDER_TARGET_OUTPUT,
  backBuffer = DXGI_USAGE_BACK_BUFFER,
#elif OPEN_GL
renderTragetOut, 
backBuffer, 
#endif // DIRECTX
};
