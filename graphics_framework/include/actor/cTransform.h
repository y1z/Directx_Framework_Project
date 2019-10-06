#pragma once
#include "actor/baseComponent.h"
#include <utility/CustomStructs.h>

class cTransform : public baseComponent
{
public:
  cTransform();

  ~cTransform() = default;

private:
  glm::vec4 m_pos;
 
  
};

