#include "utility/enGraphics.h"
#include "HelperFuncs.h"

#include "cDevice.h"
#include "cDeviceContext.h"
#include "cSwapChain.h"
#include "cWindow.h"
#include "cApiComponents.h"
#include  "cShaderBase.h"
#include "cVertexShader.h"
#include "cPixelShader.h"
#include "actor/cActor.h"
#include "../include/cModel.h"
#include "../include/cCameraManager.h"
/***************************/
#include <cstdio>
#include <cstdlib>
#include <fstream> // for ifstream
#include <sstream>
#include <map>
/////////////////////// MICROSOFT HEADERS 

#if WIND_OS
#include <commdlg.h> // for the open file function 
#endif // WIND_OS

namespace helper
{

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
    TextureDesc.texFormat = enFormats::depthStencil_format;// equivalent to DXGI_FORMAT_D24_UNORM_S8_UINT
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
  std::string
    loadFileToString(std::string_view filePath)
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
      EN_LOG_ERROR_WITH_CODE(enErrorCode::InvalidPath);
    }
    return Result;
  }
  /*************/

  std::string
    loadFileToString(std::wstring_view filePath)
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
      EN_LOG_ERROR_WITH_CODE(enErrorCode::InvalidPath);
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
    CameraData ChangeWithViewMatrix;
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


  }

  /*****************/

  void
    handelActorTransforms(cActor & actor, const uint8 chosenAxis, const uint16 pressedKey, float TransformAmount)
  {
    if (pressedKey == (WPARAM)'R')
    {
      actor.m_transform.resetToIdentity();
      //actor.m_transform.rotateInXAxis(180);
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
    std::string Result(wideString.length() + 1, '\0');

    // converts a wide string to a char/multi-byte string
    std::size_t checkForError = std::wcstombs(Result.data(), wideString.data(), wideString.length());

    // how to check for errors  https://en.cppreference.com/w/cpp/string/multibyte/wcstombs
    if (checkForError == static_cast< std::size_t >(-1))
    {
      assert(checkForError != static_cast< std::size_t >(-1) && "invalid string conversion");
    }

    return Result;
  }

  std::wstring
    convertStringToWString(std::string_view String)
  {
    std::wstring Result(String.length() + 1, '\0');

    std::size_t checkForError = std::mbstowcs(Result.data(), String.data(), String.length());

    if (checkForError == static_cast< std::size_t >(-1))
    {
      assert(checkForError != static_cast< std::size_t >(-1) && "invalid string conversion");
    }

    return Result;
  }

  /*****************/
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

  //TODO : repair function later 
#if 0
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

    std::unique_ptr<std::vector<VERTEX_T >>vertexes = std::make_unique<std::vector<VERTEX_T >>(); // std::vector<sVertexPosTex> vertexes; 
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
          VERTEX_T  Pos0{ glm::vec4(currentQuad.triangles[k].positions[0],1.0f),glm::vec3(1.0,0.0,0.0),  glm::vec2(0.5f,0.5f) };

          VERTEX_T   Pos1{ glm::vec4(currentQuad.triangles[k].positions[1],1.0f),glm::vec3(1.0,0.0,0.0) , glm::vec2(0.5f,0.5f) };
          VERTEX_T   Pos2{ glm::vec4(currentQuad.triangles[k].positions[2],1.0f), glm::vec3(1.0,0.0,0.0),glm::vec2(0.5f,0.5f) };
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

#endif // 0

  bool
    loadNewActorModelFromFile(cActor & actor, cWindow &windowToOpen, cDevice &device)
  {
    cModel*ptr_Model = findModelComponent(actor);
    if (ptr_Model == nullptr)
    {
      EN_LOG_ERROR;
      return false;
    }
    ptr_Model->Destroy();

    ptr_Model->setModelPath(openFile(windowToOpen));

    if (!ptr_Model->LoadModelFromFile(device))
    {
      EN_LOG_ERROR;
      return false;

    }

    return true;
  }
/*****************/

#if OPEN_GL
  void
    GlUpdateUniform(sUniformDetails & details)
  {
    if (details.ptr_data == nullptr)
    {
      std::cerr << "uniform data pointer is null ";
      assert(details.ptr_data != nullptr);
    }

    if (details.element == enConstBufferElem::mat4x4)
    {
      glUniformMatrix4fv(details.id, 1, GL_TRUE, static_cast< const GLfloat* > (details.ptr_data));
    }
    else if (details.element == enConstBufferElem::mat3x3)
    {
      glUniformMatrix3fv(details.id, 1, GL_TRUE, static_cast< const  GLfloat* > (details.ptr_data));
    }

    else if (details.element == enConstBufferElem::vec4)
    {
      glUniform4fv(details.id, 1, static_cast< const  GLfloat* > (details.ptr_data));
    }

    else if (details.element == enConstBufferElem::vec3)
    {
      glUniform3fv(details.id, 1, static_cast< const GLfloat* > (details.ptr_data));
    }

    else if (details.element == enConstBufferElem::vec2)
    {
      glUniform2fv(details.id, 1, static_cast< const  GLfloat* > (details.ptr_data));
    }

    else if (details.element == enConstBufferElem::single_float)
    {
      glUniform1fv(details.id, 1, static_cast< const  GLfloat* > (details.ptr_data));
    }



  }

  sUniformDetails
    GlCreateUniformDetail(std::string_view name, enConstBufferElem type)
  {
    sUniformDetails result;
    result.name = name;
    result.element = type;

    return result;
  }

#endif // OPEN_GL
/*****************/

  void
    addUniqueDefines(cShaderBase & originalShader)
  {

    static std::vector<std::string> defineStatements =
    {
      {"BLIN 1"},
      {"VERTEX_LIGHT 1"},
      {"DIR_LIGHT 1"}
    };

    originalShader.addDefines(defineStatements);
    originalShader.printOriginalShader();

  //DEFINE_ENUM_FLAG_OPERATORS
  }

/*****************/

  size_t
    findIndexAfterFirstNewLine(const std::string_view String)
  {
    for (size_t i = 0; i < String.size() - 1; ++i)
    {
      if (String[i] == '\n' || String[i] == '\r')
      {
        return ++i;
      }
    }
    return size_t(-1);
  }

}



