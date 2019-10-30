#include "../include/actor/cAABB.h"
#include "../include/cMesh.h"
#include "../include/cModel.h"
#include <algorithm>//std::numeric_limit

cAABB::cAABB()
//:m_middlePoint(0.0f, 0.0f, 0.0f),
//m_radius(0.0001f)
  :m_max(0.0f),
  m_min(0.0f)
{
  m_componentType = (componentTypes::AABB);
}

void
cAABB::init(const cModel & model)
{
  /*set up the lambda */
  auto findBiggestAndSmallestValuesPerAxis = [](float &xValueBig, float &yValueBig, float &zValueBig
                                                , float &xValueSmall, float &yValueSmall, float &zValueSmall, const cModel &model) {
    for (std::size_t i = 0; i < model.getMeshCount(); ++i)
    {
      const cMesh * ptr_mesh = model.getMesh(i);
      auto *vertexData = ptr_mesh->getVertexData();
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
                                      smallestX, smallestY, smallestZ, model);


  glm::vec3 middlePoint = glm::vec3
  (
    (biggestX + smallestX) / 2,
    (biggestY + smallestY) / 2,
    (biggestZ + smallestZ) / 2
  );

  m_min = glm::vec3(smallestX, smallestY, smallestZ);

  m_max = glm::vec3(biggestX, biggestY, biggestZ);

  //m_radius = glm::vec3(biggestX - m_middlePoint.x,
  //                     biggestY - m_middlePoint.y,
  //                     biggestZ - m_middlePoint.z);

}

bool
cAABB::checkCollision(const cAABB & otherAABB)
{
  /* RADIUS IMPLEMENTATION */
  //if (std::fabsf(this->m_middlePoint.x - otherAABB.m_middlePoint.x) < (this->m_radius.x + otherAABB.m_radius.x))
  //{ return true; }

  //if (std::fabsf(this->m_middlePoint.y - otherAABB.m_middlePoint.y) < (this->m_radius.y + otherAABB.m_radius.y))
  //{ return true; }

  //if (std::fabsf(this->m_middlePoint.z - otherAABB.m_middlePoint.z) < (this->m_radius.z + otherAABB.m_radius.z))
  //{ return true; }

  //if (a.max[0] < b.min[0] || a.min[0] > b.max[0]) return false;
  //if (a.max[1] < b.min[1] || a.min[1] > b.max[1]) return false;
  //if (a.max[2] < b.min[2] || a.min[2] > b.max[2]) return false;

  if (m_max.x < otherAABB.m_min.x || m_min.x > otherAABB.m_max.x)
  { return false; }

  if (m_max.y < otherAABB.m_min.y || m_min.y > otherAABB.m_max.y)
  { return false; }

  if (m_max.z < otherAABB.m_min.z || m_min.z > otherAABB.m_max.z)
  { return false; }

  return true; 
}

void
cAABB::moveAABB(const glm::mat4 & newTransform)
{
  //m_middlePoint = newTransform * glm::vec4(m_middlePoint, 1.0f);
  m_max = newTransform * glm::vec4(m_max, 1.0f);
  m_min = newTransform * glm::vec4(m_min, 1.0f);
}

bool cAABB::isReady() const
{
  return m_Ready;
}

void cAABB::Init(cDevice & device, cDeviceContext & deviceContext)
{}

void cAABB::Draw(cDeviceContext & devContext, std::vector<cConstBuffer*>& constBuffers)
{}

void cAABB::update(cDeviceContext & deviceContext, const sMatrix4x4 & Transform)
{
  moveAABB(Transform.matrix);
}

void cAABB::Destroy()
{}

// RADIUS IMPLEMENTATION
//glm::vec3
//cAABB::getMiddlePoint() const
//{
//  return  m_middlePoint;
//}

//glm::vec3 cAABB::getRadius() const
//{
//  return m_radius;
//}


