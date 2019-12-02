#include "..\include\cShaderBase.h"
#include "utility/HelperFuncs.h"
#include "cApiComponents.h"
//std includes 
#include <iostream>

cShaderBase::cShaderBase()
#if DIRECTX
  :mptr_infoBlob(nullptr)
#elif OPEN_GL
  :m_shaderID(0)
#endif // DIRECTX
{
  m_originalShader = { "" };
  m_shaderType = enShaderTypes::NONE;
  m_alteredShaders.emplace_back("");
}

cShaderBase::cShaderBase(cShaderBase && other)
{
#if DIRECTX
  this->mptr_infoBlob = other.mptr_infoBlob;
  other.mptr_infoBlob = nullptr;
#elif OPEN_GL
  this->m_shaderID = other.m_shaderID;
  other.m_shaderID = -1;
#endif // DIRECTX

  m_originalShader = std::move(other.m_originalShader);
  m_entryPoint = std::move(other.m_entryPoint);
  m_shaderModel = std::move(other.m_shaderModel);
  m_alteredShaders = std::move(other.m_alteredShaders);
}


cShaderBase::~cShaderBase()
{
#if DIRECTX
  if (mptr_infoBlob != nullptr)
  {
    mptr_infoBlob->Release();
    mptr_infoBlob = nullptr;
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

void
cShaderBase::setDxPointer(void * ptr)
{
  this->mptr_infoBlob = static_cast< ID3DBlob * >(ptr);
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
cShaderBase::getOriginalShaderPtr()
{
  return &m_originalShader;
}

const std::string &
cShaderBase::getOriginalShader()
{
  return m_originalShader;
}

const std::vector<std::string>&
cShaderBase::getAlteredShaders()
{
  return m_alteredShaders;
}

void
cShaderBase::setShader(std::string_view newShader)
{
  m_originalShader = newShader;
}

bool
cShaderBase::compileShader(std::string_view shaderPath,
                           std::string_view EntryPoint,
                           std::string_view vertexShaderVersion)
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

  m_originalShader = (helper::loadFileToString(shaderPath));
  if (!m_originalShader.compare("Error"))
  {
    EN_LOG_DB(did not managed to load the shader);
    return false;
  }

  m_entryPoint = (EntryPoint);
  m_shaderModel = (vertexShaderVersion);

  ID3DBlob* pErrorBlob = nullptr;

  HRESULT hr = D3DCompile(m_originalShader.c_str(), m_originalShader.length(),
                          nullptr, nullptr,
                          nullptr, m_entryPoint.c_str(),
                          m_shaderModel.c_str(), dwShaderFlags,
                          0, this->getInfoRef(),
                          &pErrorBlob);

  if (FAILED(hr))
  {
    if (pErrorBlob != NULL)
    {
      std::cerr << ( char* )pErrorBlob->GetBufferPointer();
    }
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

  const char * refToSource = m_originalShader.c_str();
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

bool
cShaderBase::compileShaderFromMemory(std::string_view  shader,
                                     std::string_view EntryPoint,
                                     std::string_view vertexShaderVersion)
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

  // does not load the shader from a file 
  m_originalShader = shader;

  if (!m_originalShader.compare("Error"))
  {
    EN_LOG_DB(did not managed to load the shader);
    return false;
  }

  m_entryPoint = (EntryPoint);
  m_shaderModel = (vertexShaderVersion);

  ID3DBlob* pErrorBlob = nullptr;

  HRESULT hr = D3DCompile(m_originalShader.c_str(), m_originalShader.length(),
                          nullptr, nullptr,
                          nullptr, m_entryPoint.c_str(),
                          m_shaderModel.c_str(), dwShaderFlags,
                          0, this->getInfoRef(),
                          &pErrorBlob);

  if (FAILED(hr))
  {
    if (pErrorBlob != NULL)
    {
      std::cerr << ( char* )pErrorBlob->GetBufferPointer();
    }
    if (pErrorBlob) pErrorBlob->Release();
    return  false;
  }
  if (pErrorBlob)
  {
    pErrorBlob->Release();
    pErrorBlob = nullptr;
  };

  return true;
  // TODO : make open_gl NOT load the file 
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

  const char * refToSource = m_originalShader.c_str();
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


void
cShaderBase::addDefines(std::vector<std::string>& defines)
{
  size_t indexAfterNewLine = this->getIndexAfterFirstNewLine();
  size_t filledSpots = 0;
  resizeVector(defines.size());

  for (std::string &defs : defines)
  {
    m_alteredShaders[filledSpots] = m_originalShader;
    std::string temp = defs;

    //make sure there is a #define in the define
    if (temp.find("#define ") == std::string::npos)
    {
      temp.insert(0, "#define ");
    }
    // make sure there is a new line char

    temp += '\n';
    m_alteredShaders[filledSpots].insert(indexAfterNewLine, temp);
    //indexAfterNewLine += temp.size();
    std::cout << m_alteredShaders[filledSpots];
    ++filledSpots;
  }
}

void
cShaderBase::printOriginalShader() const
{
  std::cout << "[[[[[[[[[[[ Original shader ]]]]]]]]]] \n\n===================================\n\n" << m_originalShader << '\n';

}

void
cShaderBase::printAlteredShader() const
{
  for (const std::string &shader : m_alteredShaders)
  {
    std::cout << "\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^n\n" << shader << std::endl;
  }
}

size_t
cShaderBase::getIndexAfterFirstNewLine() const
{
  for (size_t i = 0; i < m_originalShader.size() - 1; ++i)
  {
    if (m_originalShader[i] == '\n' || m_originalShader[i] == '\r')
    {
      return ++i;
    }
  }

  return size_t(0);
}

void
cShaderBase::resizeVector(size_t newSize)
{
  m_alteredShaders.resize(newSize);
}


