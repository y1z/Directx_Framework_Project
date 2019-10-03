#pragma once
#include "../include/utility/Grafics_libs.h"

struct sMatrix4x4
{
#if DIRECTX
  dx::XMMATRIX matrix;
#else
  float matrix[16];
#endif // DIRECTX
};

struct sVector4
{
#if DIRECTX
  dx::XMVECTOR vector4;
#else 
  float vector4[4];
#endif // DIRECTX
};

struct sFloat4
{
#if DIRECTX
  dx::XMFLOAT4 vector4;
#else 
  float vector4[4];
#endif // DIRECTX
};
