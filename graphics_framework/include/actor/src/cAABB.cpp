#include "../include/actor/cAABB.h"
#include "../include/cMesh.h"
#include "../include/cModel.h"
#include <algorithm>//std::numeric_limit

cAABB::cAABB()
  :m_middlePoint(0.0f, 0.0f, 0.0f),
  m_radius(0.0001f)
{}

void
cAABB::init(const cModel & model)
{
  /*set up the lambdas */
  auto findBiggestValuesPerAxis = [](float &xValue, float &yValue, float &zValue, const cModel &model) {
    for (std::size_t i = 0; i < model.getMeshCount() - 1; ++i)
    {
      const cMesh * ptr_mesh = model.getMesh(i);
      auto *vertexData = ptr_mesh->getVertexData();
      for (const auto &value : *vertexData)
      {
        (value.pos.x > xValue) ? xValue = value.pos.x : 0;
        (value.pos.y > yValue) ? yValue = value.pos.y : 0;
        (value.pos.z > zValue) ? zValue = value.pos.z : 0;
      }
    }
  };

  auto findSmallestValuesPerAxis = [](float &xValue, float &yValue, float &zValue, const cModel &model) {
    for (std::size_t i = 0; i < model.getMeshCount() - 1; ++i)
    {
      const cMesh * ptr_mesh = model.getMesh(i);
      auto *vertexData = ptr_mesh->getVertexData();
      for (const auto &value : *vertexData)
      {
        (value.pos.x < xValue) ? xValue = value.pos.x : 0;
        (value.pos.y < yValue) ? yValue = value.pos.y : 0;
        (value.pos.z < zValue) ? zValue = value.pos.z : 0;
      }
    }
  };

  float biggestX = std::numeric_limits<float>::min();
  float biggestY = std::numeric_limits<float>::min();
  float biggestZ = std::numeric_limits<float>::min();

  float smallestX = std::numeric_limits<float>::max();
  float smallestY = std::numeric_limits<float>::max();
  float smallestZ = std::numeric_limits<float>::max();

  findBiggestValuesPerAxis(biggestX, biggestY, biggestZ, model);

  findSmallestValuesPerAxis(smallestX, smallestY, smallestZ, model);

  m_middlePoint = glm::vec3
  (
    (biggestX + smallestX) / 2,
    (biggestY + smallestY) / 2,
    (biggestZ + smallestZ) / 2
  );

  glm::vec3 farthestPosition(biggestX, biggestY, biggestZ);

  m_radius = glm::vec3(biggestX - m_middlePoint.x,
                       biggestY - m_middlePoint.y,
                       biggestZ - m_middlePoint.z);

}

bool cAABB::checkCollision(const cAABB & otherAABB)
{
  if (std::fabsf(this->m_middlePoint.x - otherAABB.m_middlePoint.x) < (this->m_radius.x + otherAABB.m_radius.x))
  { return true; }

  if (std::fabsf(this->m_middlePoint.y - otherAABB.m_middlePoint.y) < (this->m_radius.y + otherAABB.m_radius.y))
  { return true; }

  if (std::fabsf(this->m_middlePoint.z - otherAABB.m_middlePoint.z) < (this->m_radius.z + otherAABB.m_radius.z))
  { return true; }

  return false;
}

void cAABB::moveAABB(const glm::mat4 & newTransform)
{
}

glm::vec3
cAABB::getMiddlePoint() const
{
  return  m_middlePoint;
}

glm::vec3 cAABB::getRadius() const
{
  return m_radius;
}


