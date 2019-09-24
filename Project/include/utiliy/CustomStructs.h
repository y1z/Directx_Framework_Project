#pragma once
#include "../include/utiliy/Grafics_libs.h"

struct sMatrix4x4
{
#if DIRECTX
  XMMATRIX matrix;
#else
#endif // DIRECTX
};

struct Vector4
{
#if DIRECTX
  XMVECTOR vector4;
#else 
#endif // DIRECTX
};


