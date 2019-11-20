#include "..\include\cShaderBase.h"
#include "utility/HelperFuncs.h"
#include "cApiComponents.h"

cShaderBase::cShaderBase()
#if DIRECTX
  :mptr_infoBlob(nullptr)
#elif OPEN_GL
  :m_shaderID(0)
#endif // DIRECTX
{
  m_shader = { "" };
  m_shaderType = enShaderTypes::NONE;
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
ID3DBlob *
cShaderBase::getInfo()
{
  return mptr_infoBlob;
}

ID3DBlob **
cShaderBase::getInfoRef()
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

bool
cShaderBase::compileShader(std::string_view shaderPath,
                           std::string_view EntryPoint,
                           std::string_view shaderVersion)
{
#if DIRECTX
  uint32 dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

  std::wstring FilePath(helper::convertStringToWString(shaderPath));

  m_shader = (helper::loadFileToString(shaderPath));
  if (!m_shader.compare("Error"))
  {
    EN_LOG_DB(did not managed to load the shader);
    return false;
  }

  m_entryPoint = (EntryPoint);
  m_shaderModel = (shaderVersion);

  ID3DBlob* pErrorBlob = nullptr;

  HRESULT hr = D3DCompile(m_shader.c_str(), m_shader.length(),
                          nullptr, nullptr,
                          nullptr, m_entryPoint.c_str(),
                          m_shaderModel.c_str(), dwShaderFlags,
                          0, this->getInfoRef(),
                          &pErrorBlob);

  if (FAILED(hr))
  {
    if (pErrorBlob != NULL)
      OutputDebugStringA(( char* )pErrorBlob->GetBufferPointer());
    if (pErrorBlob) pErrorBlob->Release();
    return  false;
  }
  if (pErrorBlob) pErrorBlob->Release();

  return true;
#elif   OPEN_GL
  GlRemoveAllErrors();
  unsigned int * shaderProgram = cApiComponents::getShaderProgram();
  this->setShader(helper::loadFileToString(shaderPath.data()));
  uint32_t shaderType{ 0u };

  if (this->m_shaderType == enShaderTypes::vertex)
  {
    shaderType = GL_VERTEX_SHADER;
  }
  else if (this->m_shaderType == enShaderTypes::pixel)
  {
    shaderType = GL_FRAGMENT_SHADER;
  }
  uint32_t TempID = glCreateShader(shaderType);

  if (GlCheckForError())
  {
    EN_LOG_ERROR
      return false;
  }

  this->setID(TempID);

  const char * refToSource = m_shader.c_str();
  glShaderSource(this->getID(), 1, &refToSource, nullptr);
  glCompileShader(this->getID());

  int Result;
  glGetShaderiv(this->getID(), GL_COMPILE_STATUS, &Result);
  // how long is the error message 
  if (Result == GL_FALSE)
  {
    int MessageSize;
    glGetShaderiv(this->getID(), GL_INFO_LOG_LENGTH, &MessageSize);

    char *ptr_message = new char[MessageSize + 1];
    glGetShaderInfoLog(this->getID(), 2048, &MessageSize, ptr_message);

    std::cerr << ptr_message << std::endl;
    delete[] ptr_message;
    return false;
  }


  return true;
#endif // DIRECTX
  return false;
}

