#pragma once
#include"../include/utiliy/Grafics_libs.h"

struct SimpleVertex
{
  XMFLOAT3 Pos;
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

