#include "..\include\cShaderBase.h"

cShaderBase::cShaderBase()
#if DIRECTX
  :mptr_infoBlob(nullptr)
#endif // DIRECTX
{}

cShaderBase::~cShaderBase()
{
#if DIRECTX
  if (mptr_infoBlob != nullptr)
  {
    mptr_infoBlob->Release();
  }

#endif // DIRECTX
}

#if DIRECTX
ID3DBlob * cShaderBase::getInfo()
{
  return mptr_infoBlob;
}

ID3DBlob ** cShaderBase::getInfoRef()
{
  return &mptr_infoBlob;
}

#endif // DIRECTX
