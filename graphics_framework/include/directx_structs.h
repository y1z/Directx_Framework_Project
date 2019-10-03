#pragma once
#include"../include/utility/Grafics_libs.h"

#if DIRECTX

struct SimpleVertex
{
  dx::XMFLOAT4 Pos;
  dx::XMFLOAT2 Tex;
};

struct CBNeverChanges
{
  dx::XMMATRIX mView;
};

struct CBChangeOnResize
{
  dx::XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
  dx::XMMATRIX mWorld;
  dx::XMFLOAT4 vMeshColor;
};


#endif // DIRECTX