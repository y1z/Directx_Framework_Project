#include "utility/Grafics_libs.h"
#include "HelperFuncs.h"
#include "cDevice.h"
#include "cDeviceContext.h"
#include "cSwapChain.h"
#include "cWindow.h"
#include "cApiComponents.h"
#include "enum_headers/enFormatEnums.h"
#include  "cShaderBase.h"
#include "cVertexShader.h"
#include "cPixelShader.h"
#include "actor/cActor.h"
#include "../include/cModel.h"
#include "../include/cCameraManager.h"
#include "../include/enum_headers/enumBufferUsage.h"
/***************************/
#include <cstdio>
#include <fstream> // for ifstream
#include <sstream>
#include <map>
/////////////////////// MICROSOFT HEADERS 
#include <commdlg.h> // for the open file function 

namespace helper
{

  bool
    CreateDeviceAndSwapchain(cDevice & device, cDeviceContext & deviceContext,
                             cSwapChain &swapChain, cWindow & window,
                             cApiComponents &apiComponent)
  {
  #if DIRECTX
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(window.getHandle(), &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
  #ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  #endif
    D3D_DRIVER_TYPE  driverType;

    D3D_FEATURE_LEVEL  featureLevel;

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
      D3D_FEATURE_LEVEL_12_0	,
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_3	,
      D3D_FEATURE_LEVEL_9_2,
      D3D_FEATURE_LEVEL_9_1	,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    swapChain.setSwapChain(width, height, Formats::R8G8B8A8_uniform_norm,//equivalent to DXGI_FORMAT_R8G8B8A8_UNORM
                           static_cast< int >(bufferUsage::renderTragetOut),/*equivalent to DXGI_USAGE_RENDER_TARGET_OUTPUT*/ window);


    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
      driverType = driverTypes[driverTypeIndex];
      hr = D3D11CreateDeviceAndSwapChain(NULL, driverType,
                                         NULL, createDeviceFlags,
                                         featureLevels, numFeatureLevels,
                                         D3D11_SDK_VERSION, &swapChain.getSwapChainDesc(),
                                         swapChain.getSwapChainRef(), device.getDeviceRef(),
                                         &featureLevel, deviceContext.getDeviceContextRef());
      if (SUCCEEDED(hr))
      {
        apiComponent.setSupportedVersion(static_cast< int >(featureLevel));
        apiComponent.setHardwareVersion(static_cast< int > (driverType));
        return true;
      }
    }
  #elif OPEN_GL
    GlRemoveAllErrors();

    swapChain.setGlWindow(window.getHandle());

    int32 majorVersion{ 0 };
    int32 minorVersion{ 0 };

    glfwGetVersion(&majorVersion, &minorVersion, NULL);

    const unsigned char * OpenglVersion = glGetString(GL_VERSION);
    const unsigned char *GlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const unsigned char *OpenglRenderer = glGetString(GL_RENDERER);

    std::cout << "GLFW version : " << "Major [" << majorVersion << "] Minor [" << minorVersion << "]\n"
      << "Open_gl version : " << OpenglVersion << '\n'
      << "Glsl  Shader version : " << GlslVersion << '\n'
      << "Open_gl renderer : " << OpenglRenderer << '\n' << std::endl;

    apiComponent.setSupportedVersion(majorVersion, minorVersion);

    unsigned int* ptr_vertexArrayObject = cApiComponents::getvertexArrayObject();

    glGenVertexArrays(1, ptr_vertexArrayObject);
    glBindVertexArray(*ptr_vertexArrayObject);

    unsigned int* ptr_ShaderProgram = cApiComponents::getShaderProgram();

    *(ptr_ShaderProgram) = glCreateProgram();

    if (GlCheckForError())
    {
      EN_LOG_ERROR
        return false;
    }

    return true;
  #endif // DIRECTX
    return false;
  } // end function

  /*************/
  bool
    CompileShader(const wchar_t * FileName, const char * shaderModel,
                  const char * entryPoint, cShaderBase & shader)
  {
  #ifdef DIRECTX
    HRESULT hr = S_FALSE;
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  #if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
  #endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile(FileName, NULL,
                            NULL, entryPoint,
                            shaderModel, dwShaderFlags,
                            0, shader.getInfoRef(), &pErrorBlob);

    if (FAILED(hr))
    {
      if (pErrorBlob != NULL)
        OutputDebugStringA(( char* )pErrorBlob->GetBufferPointer());
      if (pErrorBlob) pErrorBlob->Release();
      return  false;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return true;
  #elif OPEN_GL
    GlRemoveAllErrors();
  //unsigned int * shaderProgram = cApiComponents::getShaderProgram();
    shader.setShader(helper::loadFileToString(FileName));
    uint32_t shaderType{ 0u };

    if (dynamic_cast< cVertexShader* >(&shader))
    {
      shaderType = GL_VERTEX_SHADER;
    }
    else if (dynamic_cast< cPixelShader* >(&shader))
    {
      shaderType = GL_FRAGMENT_SHADER;
    }
    uint32_t TempID = glCreateShader(shaderType);

    if (GlCheckForError())
    {
      EN_LOG_ERROR
        return false;
    }

    shader.setID(TempID);
    const std::string *shaderSource = shader.getShader();

    const char * refToSource = shaderSource->c_str();
    glShaderSource(shader.getID(), 1, &refToSource, nullptr);
    glCompileShader(shader.getID());

    int Result;
    glGetShaderiv(shader.getID(), GL_COMPILE_STATUS, &Result);
    // how long is the error message 
    if (Result == GL_FALSE)
    {
      int MessageSize;
      glGetShaderiv(shader.getID(), GL_INFO_LOG_LENGTH, &MessageSize);

      char *ptr_message = new char[MessageSize + 1];
      glGetShaderInfoLog(shader.getID(), 2048, &MessageSize, ptr_message);

      std::cerr << ptr_message << std::endl;
      delete[] ptr_message;
      return false;
    }


    return true;
  #endif // DIRECTX
    return false;
  }

  /*************/
  float
    radiansToDegrees(float radians)
  {
    return radians *= (180.0f / 3.14159f);
  }

  /*************/
  float
    degreesToRadians(float degrees)
  {
    return degrees *= (3.14159f / 180.0f);
  }

  /*************/
  sTextureDescriptor
    createDepthStencilDesc(uint32 width, uint32 height)
  {
    sTextureDescriptor TextureDesc;
    memset(&TextureDesc, 0, sizeof(TextureDesc));
    TextureDesc.texHeight = height;
    TextureDesc.texWidth = width;
    TextureDesc.texFormat = Formats::depthStencil_format;// equivalent to DXGI_FORMAT_D24_UNORM_S8_UINT
  #if DIRECTX
    TextureDesc.BindFlags = 0x40L;// equivalent to DD3D11_BIND_DEPTH_STENCIL
  #elif OPEN_GL

  #endif // DIRECTX
    TextureDesc.Usage = 0;// equivalent to D3D11_USAGE_DEFAULT
    TextureDesc.CpuAccess = 0;
    TextureDesc.arraySize = 1;
    return TextureDesc;
  }

  /*************/
  std::string
    openFile(cWindow &window)
  {
  #if WIND_OS
    OPENFILENAMEA File;
    std::memset(&File, 0, sizeof(File));
    char FileName[4096];

    std::memset(&FileName, 0, sizeof(FileName));
    /* First \0 describes the name of the options
    that the user will be presented with, the second \0
    will discribe the type of files to look for */
    //   static constexpr const char* FileTypes = 
    File.lStructSize = sizeof(OPENFILENAMEA);
  #if DIRECTX
    File.hwndOwner = window.getHandle();
  #elif OPEN_GL
    File.hwndOwner = nullptr;
  #endif // DIRECTX / OPEN_GL
    File.lpstrFile = FileName;
    File.lpstrFile[0] = '\0';
    File.nMaxFile = 4096;
    File.lpstrFilter = "All files\0*.*\0obj files\0*.obj\0fbx files\0*.fbx\0";
    //FileTypes;
    File.nFilterIndex = 1;

    GetOpenFileNameA(&File);
  #endif // WIND_OS

    return std::string(FileName);
  }

  /*************/
  cModel *
    findModelComponent(cActor & actor)
  {
    for (auto iter = actor.getIteratorBegin(); iter != actor.getIteratorEnd(); ++iter)
    {
      baseComponent *ptr_possibleModel = *iter;
      cModel * model = dynamic_cast< cModel* >(ptr_possibleModel);
      if (model == nullptr) { continue; }
      else { return model; }
    }
    return nullptr;
  }

  /*************/
  std::string loadFileToString(std::string_view filePath)
  {
    std::string Result{ "Error" };
    std::ifstream File(filePath);
    if (File.is_open())
    {
      std::stringstream SStream;
      SStream << File.rdbuf();
      Result = SStream.str();

      File.close();
      return Result;
    }
    else
    {
      En_LOG_ERROR_WITH_CODE(enErrorCode::InvalidPath);
    }
    return Result;
  }
  /*************/

  std::string loadFileToString(std::wstring_view filePath)
  {
    std::string Result{ "Error" };
    std::ifstream File(filePath);
    if (File.is_open())
    {
      std::stringstream SStream;
      SStream << File.rdbuf();
      Result = SStream.str();

      File.close();
      return Result;
    }
    else
    {
      En_LOG_ERROR_WITH_CODE(enErrorCode::InvalidPath);
    }
    return Result;
  }
  /*************/

  void
    handelCameraKeyInput(const uint16 pressedKey, cCameraManager & currentCamera,
                         cWindow & window, cDeviceContext &deviceContext,
                         cConstBuffer *neverChange, cConstBuffer *resizeChange,
                         float deltaTime)
  {
    // used to alter the view matrix 
    ViewMatrix ChangeWithViewMatrix;
    ProjectionMatrix  ChangeOnProjectionChange;
    // going forwards 
    if (pressedKey == (WPARAM)'W')
    {
      currentCamera.moveFront(1.0f, window, deltaTime);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(neverChange),
                                      &ChangeWithViewMatrix);
    }//going backwards
    else if (pressedKey == (WPARAM)'S')
    {
      currentCamera.moveFront(-1.0f, window, deltaTime);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(neverChange),
                                      &ChangeWithViewMatrix);
    }//going right
    else if (pressedKey == (WPARAM)'D')
    {
      currentCamera.moveRight(1.0f, window, deltaTime);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(neverChange),
                                      &ChangeWithViewMatrix);
    }//going left 
    else if (pressedKey == (WPARAM)'A')
    {
      currentCamera.moveRight(-1.0f, window, deltaTime);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(neverChange),
                                      &ChangeWithViewMatrix);
    }
    //going up 
    else if (pressedKey == (WPARAM)'E')
    {
      currentCamera.moveUp(1.0f, window, deltaTime);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(neverChange),
                                      &ChangeWithViewMatrix);
    }//going down 
    else if (pressedKey == (WPARAM)'Q')
    {
      currentCamera.moveUp(-1.0f, window, deltaTime);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(neverChange),
                                      &ChangeWithViewMatrix);
    }
    else if (pressedKey == (WPARAM)'1')
    {
      currentCamera.switchCamera(0);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(neverChange),
                                      &ChangeWithViewMatrix);

      ChangeOnProjectionChange.matrix = currentCamera.getProjectionMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(resizeChange),
                                      &ChangeOnProjectionChange);
    }
    else if (pressedKey == (WPARAM)'2')
    {
      currentCamera.switchCamera(1);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(neverChange),
                                      &ChangeWithViewMatrix);

      //for changing the projection
      ChangeOnProjectionChange.matrix = currentCamera.getProjectionMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(resizeChange),
                                      &ChangeOnProjectionChange);
    }
  //#elif OPEN_GL 
  //#endif // DIRECTX

  }

  /*****************/

  void
    handelActorTransforms(cActor & actor, const uint8 chosenAxis, const uint16 pressedKey, float TransformAmount)
  {
    if (pressedKey == (WPARAM)'R')
    {
      actor.m_transform.resetToIdentity();
    }

    else if (pressedKey == VK_RIGHT)
    {
      switch (chosenAxis)
      {
      case 0:
      actor.m_transform.rotateInXAxis(TransformAmount);
      break;

      case 1:
      actor.m_transform.rotateInYAxis(TransformAmount);
      break;

      case 2:
      actor.m_transform.rotateInZAxis(TransformAmount);
      break;
      }
    }

    else if (pressedKey == VK_LEFT)
    {
      switch (chosenAxis)
      {
      case 0:
      actor.m_transform.rotateInXAxis(-TransformAmount);
      break;
      case 1:
      actor.m_transform.rotateInYAxis(-TransformAmount);
      break;
      case 2:
      actor.m_transform.rotateInZAxis(-TransformAmount);
      break;
      }
    }

    if (pressedKey == (WPARAM)'U')
    {
      switch (chosenAxis)
      {
      case 0:
      actor.m_transform.shearTransformInXAxis(TransformAmount);
      break;
      case 1:
      actor.m_transform.shearTransformInYAxis(TransformAmount);
      break;
      case 2:
      actor.m_transform.shearTransformInZAxis(TransformAmount);
      break;
      }
    }

    else if (pressedKey == (WPARAM)'I')
    {
      switch (chosenAxis)
      {
      case 0:
      actor.m_transform.shearTransformInXAxis(-TransformAmount);
      break;
      case 1:
      actor.m_transform.shearTransformInYAxis(-TransformAmount);
      break;
      case 2:
      actor.m_transform.shearTransformInZAxis(-TransformAmount);
      break;
      }
    }

    else if (pressedKey == (WPARAM)'O')
    {
      switch (chosenAxis)
      {
      case 0:
      actor.m_transform.reflectTransfromInXAxis(TransformAmount);
      break;
      case 1:
      actor.m_transform.reflectTransfromInYAxis(TransformAmount);
      break;
      case 2:
      actor.m_transform.reflectTransfromInZAxis(TransformAmount);
      break;
      }
    }

    if (pressedKey == (WPARAM)'P')
    {
      switch (chosenAxis)
      {
      case 0:
      actor.m_transform.reflectTransfromInXAxis(-TransformAmount);
      break;
      case 1:
      actor.m_transform.reflectTransfromInYAxis(-TransformAmount);
      break;
      case 2:
      actor.m_transform.reflectTransfromInZAxis(-TransformAmount);
      break;
      }
    }

    //move forwards 
    if (pressedKey == (WPARAM)'T')
    {
      actor.m_transform.moveTransform(0.0f, 0.0f, TransformAmount, 0.0f);
    }
    //move backwards 
    else if (pressedKey == (WPARAM)'G')
    {
      actor.m_transform.moveTransform(0.0f, 0.0f, -TransformAmount, 0.0f);
    }
    //move left 
    else if (pressedKey == (WPARAM)'F')
    {
      actor.m_transform.moveTransform(-TransformAmount, 0.0f, 0.0f, 0.0f);
    }
    // move right 
    else if (pressedKey == (WPARAM)'H')
    {
      actor.m_transform.moveTransform(TransformAmount, 0.0f, 0.0f, 0.0f);
    }

    else if (pressedKey == (WPARAM)'V')
    {
      actor.m_transform.moveTransform(0.f, TransformAmount, 0.0f, 0.f);
    }

    else if (pressedKey == (WPARAM)'N')
    {
      actor.m_transform.moveTransform(0.f, -TransformAmount, 0.f, 0.f);
    }
  }

  /*************/
  sWindowSize
    getWindowSize(cWindow & window)
  {
    int32 windowWidth;
    int32 windowHeight;
  #if DIRECTX
    RECT widowDimensions;
    GetClientRect(window.getHandle(), &widowDimensions);
    windowWidth = widowDimensions.right - widowDimensions.left;
    windowHeight = widowDimensions.bottom - widowDimensions.top;

  #elif OPEN_GL
    glfwGetWindowSize(window.getHandle(), ( int* )&windowWidth, ( int* )&windowHeight);

  #endif // DIRECTX

    return sWindowSize(windowWidth, windowHeight);
  }

  std::string
    convertWStringToString(std::wstring_view wideString)
  {
    mbstate_t mbs;
    std::memset(&mbs, 0, sizeof(mbs));

    std::string Result(wideString.length(), '\0');

    uint64 length = 0;
    size_t i = 0;
    for (const wchar_t wideChar : wideString)
    {
      length = wcrtomb(&Result[i], wideChar, &mbs);
      if (length == 0 || length > MB_CUR_MAX)
      { break; }
      i++;
    }

    return Result;
  }

  void
    arrangeForApi(sMatrix4x4 & Original)
  {
  #if DIRECTX
    Original.matrix = glm::transpose(Original.matrix);
  #elif OPEN_GL
  //  Original.matrix = glm::transpose(Original.matrix);
  #endif // DIRECTX
  }
/*****************/

  int32
    convertKeyValue(const int32 originalValue)
  {
    static const std::map<int32, int32> OriginalAndEquivalent =
    {
  /* GLFW  WINDOW */
     {262 ,VK_RIGHT},
     {263, VK_LEFT} ,
     {264, VK_DOWN},
     {265, VK_UP}
    };

    auto possibleValue = OriginalAndEquivalent.find(originalValue);

    if (possibleValue != OriginalAndEquivalent.end())
    {
      return  possibleValue->second;
    }

    return originalValue;
  }

/*****************/
  cModel *
    createHelicoid(float lowerMultiplier, float upperMultiplier,
                   float lowerAngle, float upperAngle,
                   uint32_t division, cDevice &device)
  {
    float inverseValue = 1.0f / ( float )division;

    float incrementBetweenMultiplier = std::fabs(upperMultiplier - lowerMultiplier) * inverseValue;
    float incrementBetweenAngles = (lowerAngle - upperAngle) * inverseValue;

    float currentAngle = lowerAngle;
    float currentMultiplier = lowerMultiplier;

    std::unique_ptr<std::vector<sVertexPosTex>>vertexes = std::make_unique<std::vector<sVertexPosTex>>(); // std::vector<sVertexPosTex> vertexes; 
    std::unique_ptr<std::vector< uint16 >>indices = std::make_unique<std::vector<uint16>>(); // std::vector<sVertexPosTex> vertexes; 

    auto calculateQuad = [](glm::vec3 &bottomRight, uint32_t Iteration)->sQuad
    {
      sQuad result;
      uint16 firstIndice = Iteration * 6;
      for (uint8 i = 0; i < 2; ++i)
      {
        result.triangles[i].indices[0] = firstIndice++;
        result.triangles[i].indices[1] = firstIndice++;
        result.triangles[i].indices[2] = firstIndice++;

        result.triangles[i].positions[0] = bottomRight;
        if (i > 0)
        { result.triangles[i].positions[1] = bottomRight + glm::vec3(-1.0f, 0.0f, 0.0f); }
        else
        { result.triangles[i].positions[1] = bottomRight + glm::vec3(0.0f, 1.0f, 0.0f); }

        result.triangles[i].positions[2] = bottomRight + glm::vec3(-1.0f, 1.0f, 0.0f);
      }

      return result;
    };


    uint32_t Iteration = 0;
    for (uint32 i = 0; i < division; ++i)
    {
      currentAngle = lowerAngle;

      for (uint32 j = 0; j < division; ++j)
      {
        // indicates the bottom right position of my quad 
        glm::vec3 position(currentMultiplier * sinf(currentAngle),
                           currentAngle * currentMultiplier,
                           currentMultiplier * cosf(currentAngle));

        sQuad currentQuad = calculateQuad(position, Iteration++);
        for (uint32 k = 0; k < 2; ++k)
        {// get the indices 
          indices->emplace_back(currentQuad.triangles[k].indices[0]);
          indices->emplace_back(currentQuad.triangles[k].indices[1]);
          indices->emplace_back(currentQuad.triangles[k].indices[2]);
          //fill the vertex 
          sVertexPosTex Pos0{ glm::vec4(currentQuad.triangles[k].positions[0],1.0f), glm::vec2(0.5f,0.5f) };
          sVertexPosTex Pos1{ glm::vec4(currentQuad.triangles[k].positions[1],1.0f), glm::vec2(0.5f,0.5f) };
          sVertexPosTex Pos2{ glm::vec4(currentQuad.triangles[k].positions[2],1.0f), glm::vec2(0.5f,0.5f) };
          //get the vertexes 
          vertexes->emplace_back(Pos0);
          vertexes->emplace_back(Pos1);
          vertexes->emplace_back(Pos2);
        }
        currentAngle += incrementBetweenAngles;
      }
      currentMultiplier += incrementBetweenMultiplier;
    }
    cModel* result = new cModel();
    cMesh mesh;
    mesh.initIndexBuffer(indices);
    mesh.initVertexBuffer(vertexes);
    mesh.createIndexBuffer(device);
    mesh.createVertexBuffer(device);
    result->AddMesh(std::move(mesh));

    return result;
  }

/*****************/
}


