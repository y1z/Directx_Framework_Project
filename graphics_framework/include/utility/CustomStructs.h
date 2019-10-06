#pragma once
#include "../include/utility/Grafics_libs.h"
#include "glm/glm.hpp"

struct sMatrix4x4
{
#if DIRECTX
  dx::XMMATRIX matrix;
#else
  glm::mat4x4 matrix;
#endif // DIRECTX
};

struct sVector4
{
#if DIRECTX
  dx::XMVECTOR vector4;
#else 
  glm::vec4 vector4;
#endif // DIRECTX
};

struct sFloat4
{
#if DIRECTX
  dx::XMFLOAT4 vector4;
#else 
  glm::vec4 vector4;
#endif // DIRECTX
};
