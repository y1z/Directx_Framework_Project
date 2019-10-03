#pragma once 
/*!\brief this is the base class for all shader classes */
#include "utility/Grafics_libs.h"
class cShaderBase
{
public:
  cShaderBase();
  virtual ~cShaderBase();
public:
#if DIRECTX

  ID3DBlob * getInfo();
  ID3DBlob ** getInfoRef();
  
#endif // DIRECTX

private:
#if DIRECTX

  /*! this contains information ,errors , type of shader etc..*/
  ID3DBlob *mptr_infoBlob;

#endif // DIRECTX
};
