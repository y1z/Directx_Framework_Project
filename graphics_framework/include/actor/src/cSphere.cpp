#include "actor/cSphere.h"
#include "cModel.h"

cSphere::cSphere()
  :m_centerPoint(0.f, 0.f, 0.f, 0.f),
  m_radius(0.f, 0.f, 0.f) 
{
  m_componentType = componentTypes::Sphere;
}

void
cSphere::init(const cModel & Model)
{

  auto findBiggestAndSmallestValuesPerAxis = [](float &xValueBig, float &yValueBig, float &zValueBig
                                                , float &xValueSmall, float &yValueSmall, float &zValueSmall, const cModel &model) {
    // go through each mesh 
    for (std::size_t i = 0; i < model.getMeshCount(); ++i)
    {
      const cMesh * ptr_mesh = model.getMesh(i);
      auto *vertexData = ptr_mesh->getVertexData();
      // go through each vertex 
      for (const auto &value : *vertexData)
      {
        (value.pos.x > xValueBig) ? xValueBig = value.pos.x : 0;
        (value.pos.y > yValueBig) ? yValueBig = value.pos.y : 0;
        (value.pos.z > zValueBig) ? zValueBig = value.pos.z : 0;

        (value.pos.x < xValueSmall) ? xValueSmall = value.pos.x : 0;
        (value.pos.y < yValueSmall) ? yValueSmall = value.pos.y : 0;
        (value.pos.z < zValueSmall) ? zValueSmall = value.pos.z : 0;
      }
    }
  };

  float biggestX = std::numeric_limits<float>::min();
  float biggestY = std::numeric_limits<float>::min();
  float biggestZ = std::numeric_limits<float>::min();

  float smallestX = std::numeric_limits<float>::max();
  float smallestY = std::numeric_limits<float>::max();
  float smallestZ = std::numeric_limits<float>::max();

  findBiggestAndSmallestValuesPerAxis(biggestX, biggestY, biggestZ,
                                      smallestX, smallestY, smallestZ, Model);

  m_centerPoint = glm::vec4(
    (biggestX + smallestX) / 2,
    (biggestY + smallestY) / 2,
    (biggestZ + smallestZ) / 2,
    1.0f);

  m_radius = glm::vec3(biggestX, biggestY, biggestZ) - glm::vec3(m_centerPoint.x, m_centerPoint.y, m_centerPoint.z);
}

bool cSphere::checkCollsion(const cSphere & otherSphere)
{
  if (glm::length(m_centerPoint.x - otherSphere.getCenterX()) < (m_radius.x + otherSphere.getRadiusX()))
  { return true; }

  if (glm::length(m_centerPoint.y - otherSphere.getCenterY()) < (m_radius.y + otherSphere.getRadiusY()))
  { return true; }

  if (glm::length(m_centerPoint.z - otherSphere.getCenterZ()) < (m_radius.z + otherSphere.getRadiusZ()))
  { return true; }

  return false;
}

float cSphere::getCenterX() const
{
  return  this->m_centerPoint.x;
}

float cSphere::getCenterY() const
{
  return this->m_centerPoint.y;
}

float cSphere::getCenterZ() const
{
  return this->m_centerPoint.z;
}

float cSphere::getRadiusX() const
{
  return  m_radius.x;
}

float cSphere::getRadiusY() const
{
  return  m_radius.y;
}

float cSphere::getRadiusZ() const
{
  return  m_radius.z;
}

bool cSphere::isReady() const
{
  return m_Ready;
}

void cSphere::Init(cDevice & device, cDeviceContext & deviceContext)
{}

void cSphere::Draw(cDeviceContext & devContext, std::vector<cConstBuffer*>& constBuffers)
{}

void cSphere::update(cDeviceContext & deviceContext, const sMatrix4x4 & Transform)
{
  m_centerPoint = Transform.matrix * m_centerPoint;
}

void cSphere::Destroy()
{}

