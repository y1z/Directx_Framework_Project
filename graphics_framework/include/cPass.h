#pragma once
#include "cVertexShader.h"
#include "cPixelShader.h"
#include "actor/cActor.h"
#include <deque>

class cPass
{
public:
  cPass();

  ~cPass() = default;

  void init(cVertexShader &vertex, cPixelShader &pixel);

private:
  cVertexShader m_vertexShader;
  cPixelShader m_pixelShader;

  std::deque<cActor> m_actorList;
};

