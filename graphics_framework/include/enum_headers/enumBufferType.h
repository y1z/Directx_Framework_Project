#pragma once
#include "utility/GraficsMacros.h"

enum BufferType
{
  NONE = -1, 
#if DIRECTX
  Vertex = D3D11_BIND_VERTEX_BUFFER,
  Index = D3D11_BIND_INDEX_BUFFER,
  Const = D3D11_BIND_CONSTANT_BUFFER
#else
  Vertex = 0b00'00'00'00'01,
  Index = 0b00'00'00'00'10,
  Const = 0b00'00'00'01'00,
#endif // DIRECTX
};
