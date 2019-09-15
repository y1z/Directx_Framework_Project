#include "../include/cInputLayout.h"

cInputLayout::cInputLayout()
#if DIRECTX
  :mptr_inputLayout(nullptr)

#endif // DIRECTX
{}

cInputLayout::~cInputLayout()
{
  if (mptr_inputLayout)
  {
    mptr_inputLayout->Release();
  }
}

#if DIRECTX
ID3D11InputLayout * cInputLayout::getInputLayout()
{
  return mptr_inputLayout;
}

ID3D11InputLayout ** cInputLayout::getInputLayoutRef()
{
  return &mptr_inputLayout;
}

#endif // DIRECTX
