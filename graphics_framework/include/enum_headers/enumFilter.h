#pragma once
#include "utility/Grafics_libs.h"
enum class Filter
{
#if DIRECTX
  MinMagMip_Point = D3D11_FILTER_MIN_MAG_MIP_POINT,
  MinMagMip_Point_Linear = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
  MinMagMip_Linear_Mip_Point = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
  MinMagMip_MagMip_Linear = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR	,
  MinMagMip_Linear = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
  /**************************************/
  Anisotropic = D3D11_FILTER_ANISOTROPIC,
  Anisotropic_Comperasion = D3D11_FILTER_COMPARISON_ANISOTROPIC,
  Anisotropic_Minimum = D3D11_FILTER_MINIMUM_ANISOTROPIC,
  Anisotropic_Maximum = D3D11_FILTER_MAXIMUM_ANISOTROPIC
#elif OPEN_GL
  MinMagMip_Point ,
  MinMagMip_Point_Linear, 
  MinMagMip_Linear_Mip_Point, 
  MinMagMip_MagMip_Linear, 
  MinMagMip_Linear, 
  /**************************************/
  Anisotropic, 
  Anisotropic_Comperasion, 
  Anisotropic_Minimum, 
  Anisotropic_Maximum
#endif // DIRECTX
};
