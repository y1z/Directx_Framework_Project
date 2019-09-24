#pragma once
#include"../include/utiliy/Grafics_libs.h"

#if DIRECTX

struct SimpleVertex
{
  XMFLOAT4 Pos;
  XMFLOAT2 Tex;
};

struct CBNeverChanges
{
  XMMATRIX mView;
};

struct CBChangeOnResize
{
  XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
  XMMATRIX mWorld;
  XMFLOAT4 vMeshColor;
};


#endif // DIRECTX
