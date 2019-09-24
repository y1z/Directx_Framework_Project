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

/*! tell the api which topology to use */
enum class Topology
{
  UnDefined = 0,
#if DIRECTX
  TriList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
  PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
  LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST
#else
  TriList = 1,
  PointList = 2,
  LineList = 3
#endif // DIRECTX
};
