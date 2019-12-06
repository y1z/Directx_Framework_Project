#include "cShaderManager.h"
#include "cResourceManager.h"
#include "cInputLayout.h"
#include "utility/HelperFuncs.h"

//std includes 
#include <vector>

// used to keep track of id values
static size_t s_idCount = 0u;

cShaderManager::cShaderManager()
  :m_selectedShaders(0)
{
  m_defineOptions =
  {
    {"DIR_LIGHT 1"},
    {"POINT_LIGHT 1"},
    {"CONE_LIGHT 1"},
    {"ALL_SHADER 1"},
  };
}

cShaderManager::~cShaderManager()
{
  m_shaders.clear();
}

bool
cShaderManager::init(cDevice &device,
                     sShadersManagerDesc &descriptor)

{
  static std::vector<std::string> globalDefines =
  {
    {"PIXEL_LIGHT 1"},
    {"BLIN 1"},
  };

  std::vector<std::string> vertexShaderStr = addUniqueDefines(helper::loadFileToString(descriptor.vertexShaderPath));
  if (vertexShaderStr.empty())
  {
    EN_LOG_ERROR;
    return false;
  }

  vertexShaderStr = createShaderWithGlobalDefines(vertexShaderStr, globalDefines);


  std::vector<std::string> pixelShaderStr = addUniqueDefines(helper::loadFileToString(descriptor.pixelShaderPath));
  if (pixelShaderStr.empty())
  {
    EN_LOG_ERROR;
    return false;
  }

  pixelShaderStr = createShaderWithGlobalDefines(pixelShaderStr, globalDefines);


  bool isSuccessful = false;
  for (size_t i = 0; i < pixelShaderStr.size() - 1; ++i)
  {
    sShadersPairs shaders;
    shaders.id = s_idCount++;

    isSuccessful = shaders.vertexShader.compileShaderFromMemory(vertexShaderStr[i],
                                                                descriptor.vertexEntry,
                                                                descriptor.vertexShaderVersion);

    if (!isSuccessful)
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::ShaderComplieError);
      return false;
    }

    isSuccessful = shaders.pixelShader.compileShaderFromMemory(pixelShaderStr[i],
                                                               descriptor.pixelEntry,
                                                               descriptor.pixelShaderVersion);

    if (!isSuccessful)
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::ShaderComplieError);
      return false;
    }

    isSuccessful = device.CreateVertexShader(shaders.vertexShader);

    if (!isSuccessful)
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
      return false;
    }

    isSuccessful = device.CreatePixelShader(shaders.pixelShader);

    if (!isSuccessful)
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
      return false;
    }

    m_shaders.emplace_back(std::move(shaders));
  }//end for 

  this->generateNamesForShader();

  return true;
}

bool
cShaderManager::swichShader(size_t index)
{
  if (index > m_shaders.size() - 1)
  {
    return false;
  }
  else
  {
    m_selectedShaders = index;
    return true;
  }
}

size_t
cShaderManager::getShaderCount() const
{
  return m_shaders.size();
}

cVertexShader&
cShaderManager::getVertexShaderRef()
{
  return m_shaders[m_selectedShaders].vertexShader;
}

cPixelShader &
cShaderManager::getPixelShaderRef()
{
  return m_shaders[m_selectedShaders].pixelShader;
}

cVertexShader *
cShaderManager::getVertexShaderPtr()
{
  return &m_shaders[m_selectedShaders].vertexShader;
}

cPixelShader *
cShaderManager::getPixelShaderPtr()
{
  return &m_shaders[m_selectedShaders].pixelShader;
}

const std::string &
cShaderManager::getShaderNameRef()
{
  return m_shaders[m_selectedShaders].name;
}

void
cShaderManager::setShader(cDeviceContext & deviceContext)
{
  deviceContext.SetShaders(m_shaders[m_selectedShaders].vertexShader, m_shaders[m_selectedShaders].pixelShader);
}

std::vector<std::string>
cShaderManager::addUniqueDefines(const std::string & originalShader)
{
  size_t indexAfterNewLine = helper::findIndexAfterFirstNewLine(originalShader);

  if (indexAfterNewLine == size_t(-1))
  {
    return std::vector<std::string>();
  }

  // make sure the first shader does NOT have any defines 
  std::vector<std::string> result{ originalShader };
  for (std::string &define : m_defineOptions)
  {
    std::string newShader = originalShader;

    this->makeDefineValid(define);

    newShader.insert(indexAfterNewLine, define);

    result.emplace_back(newShader);
  }

  return result;
}


std::vector<std::string>
cShaderManager::createShaderWithGlobalDefines(std::vector<std::string>& containerOfShaders,
                                              std::vector< std::string> &globalDefines)
{
  std::vector<std::string> result;

  for (std::string& shader : containerOfShaders)
  {
    size_t indexAfterNewLine = helper::findIndexAfterFirstNewLine(shader);

    for (std::string& define : globalDefines)
    {
      //make sure that the define is valid
      this->makeDefineValid(define);
      // insert the respective defines 
      shader.insert(indexAfterNewLine, define);
      result.emplace_back(shader);
      // change the value of the define(if it's 1 then it becomes 0 and vice-versa)
      this->changeValueOfDefine(shader, indexAfterNewLine);
      result.emplace_back(shader);
      this->changeValueOfDefine(shader, indexAfterNewLine);
    }
  }

  return result;
}

void
cShaderManager::makeDefineValid(std::string & define)
{
  //make sure that the "#define " statement is in the define
  if (define.find("#define ") == std::string::npos)
  {
    define.insert(0, "#define ");
  }
  //make sure the define ends in a 1 or 0
  bool hasNumericValue = false;
  for (const char& character : define)
  {
    if (character == '0' || character == '1')
    { hasNumericValue = true; }
  }

  if (!hasNumericValue)
  {
    define.append(" 1");
  }

 //make sure that the define end in a newline
  if (define[define.size() - 1] != '\n')
  {
    define.append("\n");
  }

}

void
cShaderManager::changeValueOfDefine(std::string & shader, size_t definePos)
{
  while (shader.size() > definePos)
  {
    // find the value then replace them with the appropriate counter part
    if (shader[definePos] == '0' || shader[definePos] == '1')
    {
      (shader[definePos] == '0') ? shader[definePos] = '1' : shader[definePos] = '0';
      break;
    }
    ++definePos;
  }

}

void
cShaderManager::generateNamesForShader()
{
  std::vector<size_t> positionsOfDefines;

  for (sShadersPairs &shader : m_shaders)
  {
    const std::string *shaderPtr = shader.pixelShader.getOriginalShaderPtr();
    size_t posInShader = 0u;

    // store all the positions for later use 
    while (posInShader != std::string::npos)
    {
      posInShader = shaderPtr->find("#define ", posInShader);
      if (posInShader == std::string::npos)
      {
        break;
      }
      positionsOfDefines.emplace_back(posInShader);
      posInShader += 8;
    }

    // generate all the names 
    for (size_t Pos : positionsOfDefines)
    {
      shader.name += shaderPtr->substr(Pos,
        (helper::findNewLineAfterGivenPosition(*shaderPtr, Pos) - (Pos)));

      shader.name += " ";

      std::cout << "shader name [[" << shader.name << "]]\n\n";
    }

    positionsOfDefines.clear();
  }

}


