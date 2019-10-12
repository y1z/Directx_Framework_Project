#pragma once
#include "MacroslessWindow.h"
#include "GraficsMacros.h"
#if DIRECTX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
namespace dx = DirectX;
#elif OPEN_GL
#endif // DIRECTX
#include "utility/Resource.h"
///////////////////////////////////
