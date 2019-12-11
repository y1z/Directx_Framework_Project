
#include "cDeviceContext.h"
// implementations
#include "../include/cDevice.h"
#include "../include/cRenderTargetView.h"
#include "../include/cDepthStencilView.h"
#include "../include/cTexture2D.h"
#include "../include/cVertexShader.h"
#include "../include/cPixelShader.h"
#include "../include/cInputLayout.h"
#include "../include/cVertexBuffer.h"
#include "../include/cIndexBuffer.h"
#include "../include/cConstBuffer.h"
#include "../include/cSampler.h"
#include "../include/cViewport.h"
#include "../include/cBuffer.h"
#include "../include/cShaderResourceView.h"
#include "../include/cMesh.h"

#include "../include/cApiComponents.h"
#include "utility/HelperFuncs.h"
#include "utility/CustomStructs.h"

#include "glm/gtc/type_ptr.hpp" // for glm::value_ptr
// std includes 
#include <cassert>
#include <vector>
// limits for setting objects 
static constexpr const uint8_t c_MaxRenderTargets = 8;
static constexpr const uint8_t c_MaxViewPorts = 8;
//TODO : make open-gl have the ability to use multiple vertex buffers  
static constexpr const uint8_t c_MaxVertexBuffers = 24;

cDeviceContext::cDeviceContext()
#if DIRECTX
  :mptr_deviceContext(nullptr)
#elif OPEN_GL
#endif // DIRECTX
{}

cDeviceContext::~cDeviceContext()
{
#if DIRECTX
  if (mptr_deviceContext)
  {
    mptr_deviceContext->Release();
    mptr_deviceContext = nullptr;
  }

#endif // DIRECTX
}

#if DIRECTX

ID3D11DeviceContext *
cDeviceContext::getDeviceContext()
{
  return mptr_deviceContext;
}

ID3D11DeviceContext **
cDeviceContext::getDeviceContextRef()
{
  return &mptr_deviceContext;
}

#elif OPEN_GL

#endif // DIRECTX

void
cDeviceContext::ClearState()
{
#if DIRECTX
  mptr_deviceContext->ClearState();
#elif OPEN_GL

#endif // DIRECTX
}
void
cDeviceContext::OMSetRenderTargets(cRenderTargetView renderTargetsViews[],
                                   cDepthStencilView &depthStencilViews,
                                   uint32_t numRenderTargets)
{
#if DIRECTX
  ID3D11RenderTargetView *RenderTempPtrArr[c_MaxRenderTargets];
  if (numRenderTargets <= c_MaxRenderTargets)
  {
    for (uint8_t i = 0; i < numRenderTargets; ++i)
    {
      RenderTempPtrArr[i] = renderTargetsViews[i].getRenderTragetView();
    }

    mptr_deviceContext->OMSetRenderTargets(numRenderTargets,
                                           RenderTempPtrArr,
                                           depthStencilViews.getDepthStencilView());
  }
  else
  {
    assert(("Error two many render targets", numRenderTargets <= c_MaxRenderTargets));
  }
#elif OPEN_GL
#endif // DIRECTX

}


void
cDeviceContext::RSSetViewports(cViewport viewports[], uint8_t numViewports)
{
#if DIRECTX
  if (numViewports <= c_MaxViewPorts)
  {
    D3D11_VIEWPORT viewportsArr[c_MaxViewPorts];
    for (uint8_t i = 0; i < numViewports; ++i)
    {
      viewportsArr[i] = viewports[i].getViewport();
    }
    mptr_deviceContext->RSSetViewports(numViewports, viewportsArr);
  }
  else
  {
    assert(("Error too many view-ports"&& numViewports <= c_MaxViewPorts));
  }
#elif OPEN_GL 
  //TODO : make this have the ability to address multiple view-ports
  glViewport(0, 0, viewports[0].getWidth(), viewports[0].getHeight());

#endif // DIRECTX

}

void
cDeviceContext::IASetInputLayout(cInputLayout & inputLayout)
{
#if DIRECTX
  mptr_deviceContext->IASetInputLayout(inputLayout.getInputLayout());
#elif OPEN_GL
#endif // DIRECTX
}

void
cDeviceContext::IASetVertexBuffers(cVertexBuffer vertexBuffer[], uint32_t numBuffers, uint32_t Slot)
{
#if DIRECTX

  if (numBuffers <= c_MaxVertexBuffers)
  {
    ID3D11Buffer *BufferTempPtrArr[c_MaxVertexBuffers];
    uint32_t strides[c_MaxVertexBuffers];
    uint32_t offSets[c_MaxVertexBuffers]{ 0 }; // use later 
    for (uint32_t i = 0; i < numBuffers; ++i)
    {
      BufferTempPtrArr[i] = vertexBuffer[i].getBuffer();
      strides[i] = vertexBuffer->getStride();
    }
    mptr_deviceContext->IASetVertexBuffers(Slot, numBuffers,
                                           BufferTempPtrArr, strides,
                                           offSets);
  }
  else
  {
    assert(("Error too many vertex buffer ", numBuffers <= c_MaxVertexBuffers));
  }

#elif OPEN_GL
  m_drawingData.currentVertexBuffer = vertexBuffer[0].getID();
#endif // DIRECTX
}

void
cDeviceContext::IASetIndexBuffer(cIndexBuffer & indexBuffer, int Format, int offSet)
{
#if DIRECTX
  mptr_deviceContext->IASetIndexBuffer(indexBuffer.getBuffer(), static_cast< DXGI_FORMAT >(Format), offSet);
#elif OPEN_GL
  m_drawingData.currentIndexBuffer = indexBuffer.getID();
  m_drawingData.currentFormat = Format;
#endif // DIRECTX
}

void
cDeviceContext::IASetPrimitiveTopology(int enTopology)
{
#if DIRECTX
  mptr_deviceContext->IASetPrimitiveTopology(static_cast< D3D11_PRIMITIVE_TOPOLOGY >(enTopology));
#elif OPEN_GL
  m_drawingData.currentTopology = enTopology;
#endif // DIRECTX
}

void
cDeviceContext::UpdateSubresource(cBuffer * Buffer, const void * originOfData)
{
#if DIRECTX
  mptr_deviceContext->UpdateSubresource(Buffer->getBuffer(), 0, nullptr, originOfData, 0, 0);
#elif OPEN_GL
  GlRemoveAllErrors();

  cConstBuffer * constBuffer = dynamic_cast< cConstBuffer * >(Buffer);
  if (constBuffer != nullptr)
  {
    if (constBuffer->getIndex() == 0)
    {
      auto *ViewMatrixTemp = reinterpret_cast< const CameraData* >(originOfData);

      //glUniformMatrix4fv(constBuffer->getID(), 1, GL_TRUE, glm::value_ptr(ViewMatrixTemp->matrix));

      std::vector<sUniformDetails>* refToContainer = constBuffer->getGlUniforms();

      //update the necessary uniforms  
      for (sUniformDetails & uni : *refToContainer)
      {
        if (!uni.name.compare("uView"))
        { uni.ptr_data = reinterpret_cast< const void* >(&ViewMatrixTemp->matrix); }

        if (!uni.name.compare("uViewDir"))
        { uni.ptr_data = reinterpret_cast< const void* >(&ViewMatrixTemp->viewDir); }

        if (!uni.name.compare("uViewPos"))
        { uni.ptr_data = reinterpret_cast< const void* >(&ViewMatrixTemp->cameraPos); }

        if (uni.ptr_data != nullptr && uni.id != -1)
        {
          helper::GlUpdateUniform(uni);
        }
      }
    }
    else if (constBuffer->getIndex() == 1)
    {
      auto *ProjMatrixTemp = reinterpret_cast< const ProjectionMatrix* >(originOfData);
      glUniformMatrix4fv(constBuffer->getID(), 1, GL_TRUE, glm::value_ptr(ProjMatrixTemp->matrix));
    }
    else if (constBuffer->getIndex() == 2)
    {
      // TODO : update  when new members are added 
      static constexpr const char *uniformBlockVarNameWorld[] = { {"u_world"},{"u_color"} };
      // TODO : update when using a different struct
      const GlChangeEveryFrame *worldMatrix = reinterpret_cast< const GlChangeEveryFrame* >(originOfData);

      // how big is the data i what to pass over 
      int32 uniformBlockSize = 0;
      glGetActiveUniformBlockiv(*cApiComponents::getShaderProgram(), constBuffer->getID(),
                                GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);

      uByte *DataBuffer = static_cast< uByte* > (alloca(uniformBlockSize));

      //TODO: update with the struct bing used 
      uint32 indices[2];
      glGetUniformIndices(*cApiComponents::getShaderProgram(), 2,
                          uniformBlockVarNameWorld, indices);

      //TODO: update with the struct bing used 
      int32  offsets[2];
      glGetActiveUniformsiv(*cApiComponents::getShaderProgram(), 2, indices,
                            GL_UNIFORM_OFFSET, offsets);

      //std::memcpy(DataBuffer, worldMatrix, sizeof(GlChangeEveryFrame));

      std::memcpy(DataBuffer + offsets[0], &worldMatrix->world,
                  sizeof(glm::mat4));

      std::memcpy(DataBuffer + offsets[1], &worldMatrix->color,// worldMatrix->color,
                  sizeof(sColorf));

      glBindBuffer(GL_UNIFORM_BUFFER, constBuffer->getGlUniformBlockID());
      glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize,
                   DataBuffer, GL_DYNAMIC_DRAW);

      glBindBufferBase(GL_UNIFORM_BUFFER, constBuffer->getID(), constBuffer->getGlUniformBlockID());

      glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
/****************************************/

/****************************************/
    else if (constBuffer->getIndex() == 3)
    {
    //   TODO : update  when new members are added 
    //  static constexpr const char *uniformBlockVarNameLight[] =
    //  {
    //    {"c_AmbientColor"},{"c_LightColor"},
    //     {"c_LightPos"}, {"c_LightDir"},
    //    {"c_LightIntensity"},{"c_LightAmbientIntensity"}
    //  };
      const sLightData* LightData = reinterpret_cast< const sLightData* >(originOfData);
      std::vector<sUniformDetails>* refToContainer = constBuffer->getGlUniforms();

      //update the necessary uniforms  
      for (sUniformDetails & uni : *refToContainer)
      {
        if (!uni.name.compare("uAmbientColor"))
        { uni.ptr_data = reinterpret_cast< const void* >(&LightData->ambientColor); }

        else if (!uni.name.compare("uDiffuseColor"))
        { uni.ptr_data = reinterpret_cast< const void* >(&LightData->diffuseColor); }

        else if (!uni.name.compare("uSpecularColor"))
        { uni.ptr_data = reinterpret_cast< const void* >(&LightData->specularColor); }

        else if (!uni.name.compare("uLightPos"))
        { uni.ptr_data = reinterpret_cast< const void* >(&LightData->pos); }

        else if (!uni.name.compare("uPointColor"))
        { uni.ptr_data = reinterpret_cast< const void* >(&LightData->pointColor); }

        else if (!uni.name.compare("uLightDir"))
        { uni.ptr_data = reinterpret_cast< const void* >(&LightData->diffuseDir); }

        else if (!uni.name.compare("uDiffuseIntensity"))
        { uni.ptr_data = reinterpret_cast< const void* >(&LightData->lightIntensity); }

        else if (!uni.name.compare("uAmbientIntensity"))
        { uni.ptr_data = reinterpret_cast< const void* >(&LightData->ambientIntensity); }

        else if (!uni.name.compare("uSpecularIntensity"))
        { uni.ptr_data = reinterpret_cast< const void* >(&LightData->specularIntensity); }

        else if (!uni.name.compare("uPointRadius"))
        { uni.ptr_data = reinterpret_cast< const void* >(&LightData->specularIntensity); }

        if (uni.ptr_data != nullptr && uni.id != -1)
        {
          helper::GlUpdateUniform(uni);
        }

      }// end for 

    }
  }

  if (GlCheckForError())
  {
    EN_LOG_ERROR;
  }

  //TODO: add check for the other buffer when you start using them in this function. 
  assert(constBuffer != nullptr);
#endif // DIRECTX
}

void
cDeviceContext::ClearRenderTargetView(cRenderTargetView & renderTargetView, sColorf* color)
{
    // han purple https://rgbcolorcode.com/color/6619FF
  static constexpr float defaultClearColor[4] = { 0.40f,0.10f,1.0f,1.0f };
  static constexpr sColorf directXColor{ 0.30f,0.30f,1.0f,1.0f };
//rgb(0.30,0.30,1.00)
#if DIRECTX
  if (color == nullptr)
  {
    mptr_deviceContext->ClearRenderTargetView(renderTargetView.getRenderTragetView(), directXColor.allColor);
  }
  else
  {
    mptr_deviceContext->ClearRenderTargetView(renderTargetView.getRenderTragetView(), color->allColor);
  }
#elif OPEN_GL
  static constexpr sColorf OpenGlColor{ 1.0f,0.34f,0.20f,1.0f };
    //rgb(1.00, 0.73, 0.20) https://rgbcolorcode.com/color/FFBB33

  if (color != nullptr)
  {
    glClearColor(color->red, color->green, color->blue, color->alpha);
  }
  else
  {
    glClearColor(OpenGlColor.red, OpenGlColor.green, OpenGlColor.blue, OpenGlColor.alpha);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif // DIRECTX 
}

void
cDeviceContext::ClearDepthStencilView(cDepthStencilView & depthStencilView, bool ClearStencil,
                                      bool ClearDepth, uint8_t StencilClearValues,
                                      float DepthClearValue)
{
#if DIRECTX
  int clearFlags = 0x00;
  // adding the flags for the directX api
  if (ClearStencil) { clearFlags |= D3D11_CLEAR_STENCIL; }
  if (ClearDepth) { clearFlags |= D3D11_CLEAR_DEPTH; }

  mptr_deviceContext->ClearDepthStencilView(depthStencilView.getDepthStencilView(), clearFlags,
                                            DepthClearValue, StencilClearValues);
#elif OPEN_GL
#endif // DIRECTX
}

void cDeviceContext::VSSetShader(cVertexShader & vertexShaderPath)
{
#if DIRECTX
  mptr_deviceContext->VSSetShader(vertexShaderPath.getVertexShader(),
                                  nullptr,
                                  0);
#elif OPEN_GL

#endif // DIRECTX
}

void cDeviceContext::VSSetConstantBuffers(cConstBuffer & Buffer, uint8_t Slot)
{
#if DIRECTX
  // make sure i don't use more slot than directX has 
  if (Slot <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
  {
    this->mptr_deviceContext->VSSetConstantBuffers(Slot, 1, Buffer.getBufferRef());
  }
  else
  {
    assert("Error used too many slot " && Slot <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1);
  }
#elif OPEN_GL
#endif // DIRECTX

}

void cDeviceContext::PSSetShader(cPixelShader & pixelShader)
{
#if DIRECTX
  mptr_deviceContext->PSSetShader(pixelShader.getPixelShader(),
                                  nullptr,
                                  0);
#elif OPEN_GL

#endif // DIRECTX
}

void
cDeviceContext::PSSetShaderResources(cShaderResourceView shaderResources[], uint32_t numResources, uint32_t Slots)
{
#if DIRECTX
  // make sure i don't use too many slots 
  if (Slots <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 &&
      numResources <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)
  {

    ID3D11ShaderResourceView* ShaderPtrArr[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
    for (uint16_t i = 0; i < numResources; ++i)
    {
      ShaderPtrArr[i] = shaderResources[i].getShaderResource();
    }

    mptr_deviceContext->PSSetShaderResources(Slots,
                                             numResources,
                                             ShaderPtrArr);

  }
  else
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::UnClassified);
    assert(("Error asking for too many slots", Slots <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1));
  }
#elif OPEN_GL
  GlRemoveAllErrors();
  // should be the index for shader program 
  glUseProgram(*cApiComponents::getShaderProgram());

  auto Location = glGetUniformLocation(*cApiComponents::getShaderProgram(), "uTextureSampler");

  int32_t Count;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &Count);

  glActiveTexture(shaderResources[0].getTextureID());//+ shaderResources->getResourceID());// + shaderResources->getResourceID());

  glBindTexture(GL_TEXTURE_2D, shaderResources[0].getResourceID());
  // NEEDS TO BE ONE OR EVERYTHING GOES TO SHIT 
  glUniform1i(Location, 0);

  if (GlCheckForError())
  {
    assert(true == false, "Error with Texture Sample");
  }

#endif // DIRECTX
}

void
cDeviceContext::PSSetShaderResources(std::vector<cShaderResourceView*>& shaderResources, uint32_t Slots)
{
#if DIRECTX

  // make sure i don't use too many slots 
  if (Slots <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 &&
      shaderResources.size() <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)
  {

    ID3D11ShaderResourceView* ShaderPtrArr[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{ 0 };
    uint32_t i = 0;
    for (cShaderResourceView * Resource : shaderResources)
    {
      ShaderPtrArr[i] = Resource->getShaderResource();
      ++i;
    }

    mptr_deviceContext->PSSetShaderResources(Slots,
                                             shaderResources.size() - 1,
                                             ShaderPtrArr);

  }
  else
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::UnClassified);
    assert(("Error asking for too many slots", Slots <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1));
  }
#elif OPEN_GL
  GlRemoveAllErrors();
  // should be the index for shader program 
  glUseProgram(*cApiComponents::getShaderProgram());

  auto Location = glGetUniformLocation(*cApiComponents::getShaderProgram(), "uTextureSampler");

  int32_t Count;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &Count);

  glActiveTexture(shaderResources[0]->getTextureID());//+ shaderResources->getResourceID());// + shaderResources->getResourceID());

  glBindTexture(GL_TEXTURE_2D, shaderResources[0]->getResourceID());
  // NEEDS TO BE ONE OR EVERYTHING GOES TO SHIT 
  glUniform1i(Location, 0);

  if (GlCheckForError())
  {
    assert(true == false, "Error with Texture Sample");
  }
#endif // DIRECTX

}

void cDeviceContext::PSSetShaderResources(cMesh & ShaderResources, uint32_t Slot)
{
#if DIRECTX
  mptr_deviceContext->PSSetShaderResources(Slot, 1, ShaderResources.getResourceRef());
#elif OPEN_GL

#endif // DIRECTX
}

void cDeviceContext::PSSetConstantBuffers(cConstBuffer & Buffer, uint32_t Slots)
{
#if DIRECTX
  if (Slots <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
  {
    mptr_deviceContext->PSSetConstantBuffers(Slots,
                                             1,
                                             Buffer.getBufferRef());
  }
  else
  {
    assert(("Error setting PSSetConstantBuffer ", Slots <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1));
  }
#elif OPEN_GL

#endif // DIRECTX
}

void cDeviceContext::PSSetSamplers(cSampler samplers[], uint32_t numSamplers, uint32_t slot)
{
#if DIRECTX
  if (slot <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 &&
      numSamplers <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)
  {
    ID3D11SamplerState *SamplerPtrArr[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
    for (uint8_t i = 0; i < numSamplers; ++i)
    {
      SamplerPtrArr[i] = samplers[i].getSampler();
    }
    mptr_deviceContext->PSSetSamplers(slot, numSamplers, SamplerPtrArr);
  }
  else
  {
    assert(("Error setting Sampler PSSetSamplers", slot <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1));
  }
#elif OPEN_GL

#endif // DIRECTX
}

void
cDeviceContext::DrawIndexed(uint32_t indexCount, uint32_t indexOffset)
{
#if DIRECTX
  mptr_deviceContext->DrawIndexed(indexCount, 0, 0);
 // mptr_deviceContext->Draw(indexCount,0);
#elif OPEN_GL
  GlRemoveAllErrors();

  glUseProgram(*cApiComponents::getShaderProgram());

  glEnableVertexAttribArray(0);// position 
  glEnableVertexAttribArray(1);// normals 
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);// textures

  glEnableVertexAttribArray(0);// position 
  glBindBuffer(GL_ARRAY_BUFFER, m_drawingData.currentVertexBuffer);// m_drawingData.currentVertexBuffer);
  uint32 OffSetOfFirst = offsetof(VERTEX_T, pos);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_T), reinterpret_cast< const void* >(OffSetOfFirst));

  glEnableVertexAttribArray(1);// normals 
  glBindBuffer(GL_ARRAY_BUFFER, m_drawingData.currentVertexBuffer);
  uint32 OffSetOfSecond = offsetof(VERTEX_T, norm);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_T), reinterpret_cast< const void * >(OffSetOfSecond));

  glEnableVertexAttribArray(2);// textures
  glBindBuffer(GL_ARRAY_BUFFER, m_drawingData.currentVertexBuffer);
  uint32 OffSetOfThird = offsetof(VERTEX_T, tan);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_T), reinterpret_cast< const void * > (OffSetOfThird));

  glEnableVertexAttribArray(3);// textures
  glBindBuffer(GL_ARRAY_BUFFER, m_drawingData.currentVertexBuffer);
  uint32 OffSetOfTexCoord = offsetof(VERTEX_T, tex);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_T), reinterpret_cast< const void * > ( OffSetOfTexCoord));

  if (GlCheckForError())
  {
    std::cout << " Error ";
  }
  //glEnableVertexAttribArray(2);
  //glBindBuffer(GL_ARRAY_BUFFER, this->m_VertexBuffer.GetBufferID());
  //unsigned int OffSetOfThird = offsetof(VertexPosNormTex, TexCoord);
  //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormTex), reinterpret_cast< const void * >(OffSetOfThird));

  // UN-BIND FROM THE VERTEX BUFFER 
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // draw the indices 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_drawingData.currentIndexBuffer);

  glDrawElements(m_drawingData.currentTopology, indexCount, m_drawingData.currentFormat, reinterpret_cast< const void* >(0));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if (GlCheckForError())
  {
    std::cout << " Error ";
  }

#endif // DIRECTX
}

bool
cDeviceContext::SetShaders(cVertexShader & vertexShader, cPixelShader & pixelShader)
{
#if DIRECTX
  this->VSSetShader(vertexShader);
  this->PSSetShader(pixelShader);

  return true;
#elif OPEN_GL
  /*TODO : make this function detach the current shader and attach the new ones*/
  uint32 * ShaderProgram = cApiComponents::getShaderProgram();
  glLinkProgram(*(ShaderProgram));
  glValidateProgram(*(ShaderProgram));

  int Status;
  glGetProgramiv(*(ShaderProgram), GL_VALIDATE_STATUS, &Status);
  if (Status == GL_FALSE)
  {
    assert(true == false && "Error with Shader Program ");
  }

  int Link;
  glGetProgramiv(*ShaderProgram, GL_LINK_STATUS, &Link);
  if (Link == GL_FALSE)
  {
    assert(true == false && "Error with linking ");
  }

  if (GlCheckForError())
  {
    return false;
  }
  else
  {
    return true;
  }

#endif // DIRECTX
  return false;
}

