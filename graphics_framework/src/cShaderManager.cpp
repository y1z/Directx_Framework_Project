#include "cShaderManager.h"
#include "cResourceManager.h"
#include "cInputLayout.h"
#include "utility/HelperFuncs.h"

//std includes 
#include <vector>

cShaderManager::cShaderManager()
  :m_selectedShaders(0)
{
  m_defineOptions =
  {
    {"BLIN 1"},
    {"PIXEL_LIGHT 1"},
    {"VERTEX_LIGHT 1"},
    {"DIR_LIGHT 1"},
    {"CONE_LIGHT 1"},
    {"POINT_LIGHT 1"}
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

  static std::vector<std::string> defineStatements =
  {
    {"BLIN 1"},
    {"PIXEL_LIGHT 1"},
    {"VERTEX_LIGHT 1"},
    {"DIR_LIGHT 1"},
    {"CONE_LIGHT 1"},
    {"POINT_LIGHT 1"}
  };

  //std::shared_ptr<cDevice> device = resourceManager.getPtrDevice();

  std::vector<std::string> vertexShaderStr = addDefinesToShaders(helper::loadFileToString(descriptor.vertexShaderPath),
                                                                 defineStatements);

  std::vector<std::string> pixelShaderStr = addDefinesToShaders(helper::loadFileToString(descriptor.pixelShader),
                                                                defineStatements);

  bool isSucceful = false;
  for (size_t i = 0; i < pixelShaderStr.size() - 1; ++i)
  {
    sShadersPairs shaders;

    isSucceful = shaders.vertShader.compileShaderFromMemory(vertexShaderStr[i],
                                                            descriptor.vertexEntry,
                                                            descriptor.vertexShaderVersion);

    if (!isSucceful)
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::ShaderComplieError);
      return false;
    }

    isSucceful = shaders.pixelShader.compileShaderFromMemory(pixelShaderStr[i],
                                                             descriptor.pixelEntry,
                                                             descriptor.pixelShaderVersion);

    if (!isSucceful)
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::ShaderComplieError);
      return false;
    }

    isSucceful = device.CreateVertexShader(shaders.vertShader);

    if (!isSucceful)
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
      return false;
    }

    isSucceful = device.CreatePixelShader(shaders.pixelShader);

    if (!isSucceful)
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
      return false;
    }

    m_shaders.emplace_back(std::move(shaders));
  }//end for 

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
  return m_shaders[m_selectedShaders].vertShader;
}

cPixelShader &
cShaderManager::getPixelShaderRef()
{
  return m_shaders[m_selectedShaders].pixelShader;
}

cVertexShader *
cShaderManager::getVertexShaderPtr()
{
  return &m_shaders[m_selectedShaders].vertShader;
}

cPixelShader *
cShaderManager::getPixelShaderPtr()
{
  return &m_shaders[m_selectedShaders].pixelShader;
}

void
cShaderManager::setShader(cDeviceContext & deviceContext)
{
  deviceContext.SetShaders(m_shaders[m_selectedShaders].vertShader, m_shaders[m_selectedShaders].pixelShader);
}

std::vector<std::string>
cShaderManager::addDefinesToShaders(const std::string & originalShader,
                                    std::vector<std::string> &defineStatements)
{
  size_t indexAfterNewLine = helper::findIndexAfterFirstNewLine(originalShader);

  if (indexAfterNewLine == size_t(-1))
  {
    return std::vector<std::string>();
  }

  // make sure the first shader does NOT have any defines 
  std::vector<std::string> result{ originalShader };
  for (std::string &define : defineStatements)
  {
    std::string newShader = originalShader;

    //make sure there is a #define in the define statement
    if (define.find("#define ") == std::string::npos)
    {
      define.insert(0, "#define ");
    }

    // make sure there is a new line char in the define
    if (define.find("\n") == std::string::npos)
    {
      define += "\n";
    }

    newShader.insert(indexAfterNewLine, define);

    result.emplace_back(newShader);
  }

  return result;
}


