#pragma once
#include "MacroslessWindow.h"

#include "GraficsMacros.h"
#if DIRECTX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
namespace dx = DirectX;
#elif OPEN_GL

#ifndef __glew_h__
#include <GL/glew.h>
#endif // __glew_h__

#include <GLFW/glfw3.h>

#endif // DIRECTX

#include "utility/Resource.h"
///////////////////////////////////
