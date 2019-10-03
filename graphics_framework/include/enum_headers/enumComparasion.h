#pragma once
#include "utility/Grafics_libs.h"

enum class Comparasion{
#if DIRECTX
Never = D3D11_COMPARISON_NEVER,
Less = D3D11_COMPARISON_LESS,
Equal = D3D11_COMPARISON_EQUAL,
Less_Equal = D3D11_COMPARISON_LESS_EQUAL,
Greater = D3D11_COMPARISON_GREATER,
#else 
#endif // DIRECTX

};
