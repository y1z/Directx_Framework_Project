#pragma once
#include "glm/vec3.hpp"
#include "utility/CustomStructs.h"

class cMesh;
class cModel;


class cAABB
{
public:
  cAABB();
  ~cAABB() = default ;
public: // functions 
/*! requires an model */
  void 
    init(const cModel &model);

  bool
    checkCollision(const cAABB &otherAABB);

  void
    moveAABB(const glm::mat4 &newTransform);

  glm::vec3
    getMiddlePoint() const ;

  glm::vec3
    getRadius()const;
private:

private:
  glm::vec3 m_middlePoint;
  glm::vec3 m_radius;
 // glm::vec3 m_radius;
};

