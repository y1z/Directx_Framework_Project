#pragma once
#include "utility/enGraphics.h"

enum class enComparison
{
#if DIRECTX
  Never = D3D11_COMPARISON_NEVER,
  Less = D3D11_COMPARISON_LESS,
  Equal = D3D11_COMPARISON_EQUAL,
  Less_Equal = D3D11_COMPARISON_LESS_EQUAL,
  Greater = D3D11_COMPARISON_GREATER,
#elif OPEN_GL 
  Never = GL_NEVER,
  Less = GL_LESS,
  Equal = GL_EQUAL,
  Less_Equal = GL_LEQUAL
#else 

  Never, 
  Less, 
  Equal, 
  Less_Equal 
#endif // DIRECTX
};
