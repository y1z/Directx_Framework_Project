#pragma once
#include "utility/enGraphics.h"
#include "utility/enDefs.h"
//#include "utility/HelperFuncs.h"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"



/******************************/
// Vector type struct's 
/******************************/

/*! this is used to determine which of all the vertex types are being used*/
#define VERTEX_T sVertexPosNormTanTex

struct sVertexPosNormTanTex
{
  glm::vec4 pos;
  glm::vec3 norm;
  glm::vec3 tan;
  glm::vec2 tex;
};

struct sVertexPosNormTex
{
  glm::vec4 pos;
  glm::vec3 norm;
  glm::vec2 tex;
};

struct sVertexPosTex
{
  glm::vec4 pos;
  glm::vec2 tex;
};

/**
* 4D VECTOR'S OR struct's with same amount of components
*/
struct sVector4
{
  glm::vec4 vector4{ 0.0f,0.0f,0.0f,1.0f };
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

  sVector3(const sVector3 &other) = default;
  sVector3(sVector3 &&other) = default;
  sVector3()
  {
    this->vector3.x = 0.0f;
    this->vector3.y = 0.0f;
    this->vector3.z = 0.0f;
  }

  sVector3(float x, float y, float z)
  {
    this->vector3.x = x;
    this->vector3.y = y;
    this->vector3.z = z;
  }

  sVector3(const glm::vec3 &otherVector)
  {
    this->vector3 = otherVector;
  }

  sVector3& operator =(const sVector3 &other) = default;
  sVector3& operator =(sVector3 &&other) = default;
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

using ViewMatrix = sMatrix4x4;
using ProjectionMatrix = sMatrix4x4;

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

/******************************/
// Other struct type 
/******************************/

struct alignas(16) sLightData
{
  // color of the lights 
  sColorf ambientColor = { 0.0f,0.0f,0.0f,1.0f };
  sColorf diffuseColor = { 0.0f,0.7f,0.7f,1.0f };
  sColorf specularColor = { 0.5f,0.5f,0.5f,1.0f };
  sColorf pointColor = { 0.7f,0.0f,0.0f,1.0f };
  sColorf spotColor = { 0.0f,0.7f,0.0f,1.0f };
  //position of the lights 
  sVector4 pos;
  sVector4 spotPos;

  // directions of the lights
  sVector3 diffuseDir = sVector3(0.0f, 0.0f, 1.0f);
  sVector3 spotDir = sVector3(-1.0f, 0.0f, 0.0f);

  float lightIntensity{ 0.5f };
  float specularIntensity{ 0.5f };
  float ambientIntensity{ 0.5f };
  float pointRadius{ 10.0f };
  float specularPower{ 0.5f };
  float spotBeta{ 1.0f };
  float spotAlpha{ 10.0f };
  float spotRadius{ 100.0f };
};

struct alignas(16)GlChangeEveryFrame
{
  glm::mat4 world;
  sColorf color;
  sVector3 viewDir;
};


struct alignas(16) CameraData
{
  glm::mat4x4 matrix;
  glm::vec4 cameraPos;
  glm::vec3 viewDir;
};


/*!
* @brief used to keep track of the necessary data used in a uniform
*/
struct sUniformDetails
{
  std::string name{ "\0" };
  const void *ptr_data{ nullptr };
  int32 id{ -1 };
  enConstBufferElem element = enConstBufferElem::NONE;
};


