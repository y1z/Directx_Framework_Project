#include "..\include\cShaderBase.h"

cShaderBase::cShaderBase()
#if DIRECTX
  :mptr_infoBlob(nullptr)
#elif OPEN_GL
  :m_shaderID(0) 
#endif // DIRECTX
{
  m_shader = {""};
}

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
#elif OPEN_GL

uint32_t 
cShaderBase::getID() const
{
  return m_shaderID;
}

void cShaderBase::setID(uint32_t newID)
{ m_shaderID = newID; }
#endif // DIRECTX

const std::string *
cShaderBase::getShader()
{
  return &m_shader;
}

void 
cShaderBase::setShader(std::string_view newShader)
{
  m_shader = newShader;
}

