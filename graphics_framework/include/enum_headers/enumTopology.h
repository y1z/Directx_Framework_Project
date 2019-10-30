#pragma once
/*! tell the api which topology to use */
enum class Topology
{
  UnDefined = 0,
#if DIRECTX
  TriList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
  PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
  LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST
#elif OPEN_GL
  TriList = GL_TRIANGLES,
  PointList = GL_POINTS ,
  LineList = GL_LINES
#else
  TriList ,
  PointList,
  LineList,
#endif // DIRECTX
};
