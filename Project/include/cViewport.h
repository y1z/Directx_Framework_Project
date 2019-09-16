#pragma once
#include "include/utiliy/Grafics_libs.h"
#include <cinttypes>

struct sViewportDesc
{
  float width{1.0f};
  float height{1.0f};
  float minDepth{0.0f};
  float maxDepth{0.0f};
  uint32_t TopLeftX{0 };
  uint32_t TopLeftY{0 };
};

class cViewport
{
public:
  cViewport();

  ~cViewport();

private:
#if DIRECTX
  D3D11_VIEWPORT m_directxViewport;
#endif // DIRECTX
};
