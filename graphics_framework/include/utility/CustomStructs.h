#pragma once
#include "../include/utility/Grafics_libs.h"
#include "utility/enDefs.h"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

/******************************/
// Vector type struct's 
/******************************/

struct sVertexPosTex
{

  glm::vec4 pos;
  glm::vec2 tex;
  sVertexPosTex()
  {
    pos = glm::vec4(0.0, 0.0, 0.0, 1.0f);
    tex = glm::vec2(0.5f, 0.5f);
  }

  sVertexPosTex(const glm::vec4  newPos)
  {
    pos = newPos;
    tex = glm::vec2(0.5f, 0.5f);
  }

  sVertexPosTex(const glm::vec4 &newPos)
  {
    pos = newPos;
    tex = glm::vec2(0.5f, 0.5f);
  }

  sVertexPosTex(const glm::vec4 &newPos, glm::vec2 newTex)
  {
    pos = newPos;
    tex = newTex;
  }
};

/**
* 4D VECTOR'S OR struct's with amount of components
*/
struct sVector4
{
  glm::vec4 vector4;
};

struct sColorf
{
  union
  {
    struct
    {
      float red;
      float green;
      float blue;
      float alpha;
    };
    float allColor[4]{ 0.0f,0.0f,0.0f,1.0f };
  };
};

/**
* 3D VECTOR'S OR struct's with amount of components
*/
struct sVector3
{
  glm::vec3 vector3;
};

/**
* 2D VECTOR'S OR struct's with amount of components
*/
struct sVector2
{
  glm::vec2 vector2;
};


struct sWindowSize
{
  int32 width;
  int32 height;
public:// constructor 
  sWindowSize(int32 Width, int32 Height)
    :width(Width),
    height(Height)
  {}

  sWindowSize()
    :width(-1337),
    height(-1337)
  {}

};

/******************************/
// matrix type struct's 
/******************************/

struct GlChangeEveryFrame
{
  glm::mat4 world;
  sColorf color;
};

/**
* 4 by 4 matrices OR struct's with similar amount of data.
*/
struct sMatrix4x4
{
  //! starts as the identity  matrix 
  glm::mat4x4 matrix{ 1.0f };
};

////////////////////////////////////////////////
// type defs of sMatrix4x4 
////////////////////////////////////////////////
typedef sMatrix4x4 ViewMatrix;
typedef sMatrix4x4 ProjectionMatrix;

/******************************/
// Geometric type struct's 
/******************************/
struct sTri
{
  glm::vec3 positions[3];
  uint16 indices[3]{ 0u,0u,0u };
};

struct sQuad
{
  sTri triangles[2];
};

