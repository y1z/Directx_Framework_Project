#pragma once
#include "../utiliy/Grafics_libs.h"

//! this is used to know which type of buffer is being used 
enum class BufferType
{
#if DIRECTX
  Vertex = D3D11_BIND_VERTEX_BUFFER,
  Index = D3D11_BIND_INDEX_BUFFER,
  Const = D3D11_BIND_CONSTANT_BUFFER
#else 
  Vertex = 0b00000001,
  Index = 0b00000010,
  Const = 0b00000100
#endif // DIRECTX
};
