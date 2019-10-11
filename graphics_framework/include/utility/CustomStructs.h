#pragma once
#include "../include/utility/Grafics_libs.h"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

struct sVertexPosTex
{
  glm::vec4 pos;
  glm::vec2 tex;
};

struct sMatrix4x4
{
  glm::mat4x4 matrix;
};

struct sVector4
{
  glm::vec4 vector4;
};

struct sVector3
{
  glm::vec3 vector3;
};

struct sFloat4
{
#if DIRECTX
  dx::XMFLOAT4 vector4;
#else 
  glm::vec4 vector4;
#endif // DIRECTX
};

struct sColorf
{
  float red;
  float green;
  float blue;
  float alpha;
};

struct GlChangeEveryFrame
{
  glm::mat4 world;
  sColorf color;
};

struct GlViewMatrix
{
  glm::mat4 matrix;
};

struct GlProjectionMatrix
{
  glm::mat4 matrix;
};

