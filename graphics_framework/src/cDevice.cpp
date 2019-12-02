
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
#include "../include/cRenderTarget.h"
#include "cApiComponents.h" // for shaderProgram 

#include "utility/HelperFuncs.h"
//std includes 
#include <vector>

cDevice::cDevice()
#if DIRECTX
  :mptr_device(nullptr)
#elif   OPEN_GL
  :mptr_device(0)
#endif // DIRECTX
{}

cDevice::~cDevice()
{
#if DIRECTX
  if (mptr_device != nullptr)
  {
    mptr_device->Release();
    mptr_device = nullptr;
  }
#endif // DIRECTX
}

bool cDevice::CreateRenderTargetView(cTexture2D & texture, cRenderTargetView & renderTraget)
{
#if DIRECTX

  HRESULT hr;
  hr = mptr_device->CreateRenderTargetView(texture.getTexture(), NULL, renderTraget.getRenderTragetViewRef());
  if (SUCCEEDED(hr))
  {
    return true;
  }
#elif OPEN_GL
  //TODO :  set up the frame buffer  http://docs.gl/gl4/glGenFramebuffers
  GlRemoveAllErrors();
  glGenFramebuffers(1, texture.getIDPtr());
  glBindFramebuffer(GL_FRAMEBUFFER, texture.getID());


  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  //if (status != GL_FRAMEBUFFER_COMPLETE)
  //{
  //	 std::string ErrorCode = std::to_string(status);
  //	 assert(true == false && "Frame buffer Errror :");
  //}

  if (!GlCheckForError())
  {
    // un bind the buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return true;
#endif // 0
  return false;
}

bool cDevice::CreateRenderTargetView(cRenderTarget & renderTarget, cRenderTargetView & renderTragetView)
{
#if DIRECTX

  HRESULT hr;
  hr = mptr_device->CreateRenderTargetView(renderTarget.getRenderTarget(), &renderTarget.getDiscriptor()
                                           , renderTragetView.getRenderTragetViewRef());
  if (SUCCEEDED(hr))
  {
    return true;
  }
  return false;
#elif OPEN_GL
  GlRemoveAllErrors();

  glGenFramebuffers(1, renderTarget.getTexture().getIDPtr());
  glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.getTexture().getID());

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  //if (status != GL_FRAMEBUFFER_COMPLETE)
  //{
  //	 std::string ErrorCode = std::to_string(status);
  //	 assert(true == false && "Frame buffer Errror :");
  //}

  if (!GlCheckForError())
  {
    // un bind the buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

#endif // DIRECTX
  return false;
}


bool
cDevice::CreateTexture2D(sTextureDescriptor & Description, cTexture2D & Texture)
{
#if DIRECTX
  HRESULT hr;
  D3D11_TEXTURE2D_DESC descDepth;
  std::memset(&descDepth, 0, sizeof(descDepth));
  descDepth.Width = Description.texWidth;
  descDepth.Height = Description.texHeight;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = static_cast< DXGI_FORMAT > (Description.texFormat);//DXGI_FORMAT_D24_UNORM_S8_UINT;//DXGI_FORMAT_D24_UNORM_S8_UINT
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = static_cast< D3D11_USAGE > (Description.Usage);
  descDepth.BindFlags = Description.BindFlags;
  descDepth.CPUAccessFlags = Description.CpuAccess;
  descDepth.MiscFlags = 0;

  hr = mptr_device->CreateTexture2D(&descDepth, NULL, Texture.getTextureRef());
  if (SUCCEEDED(hr))
  {
    return true;
  }
#elif OPEN_GL
  GlRemoveAllErrors();

  glBindTexture(GL_TEXTURE_2D, Texture.getID());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Description.texWidth, Description.texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glBindTexture(GL_TEXTURE_2D, 0);

  if (!GlCheckForError())
  {
    return true;
  }
#endif // DIRECTX
  return false;
}


bool
cDevice::CreateDepthStencilView(cDepthStencilView & DepthView)
{
#if DIRECTX
  HRESULT hr;
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  SecureZeroMemory(&descDSV, sizeof(descDSV));
  descDSV.Format = static_cast< DXGI_FORMAT >(DepthView.getDescriptor().Format);
  descDSV.ViewDimension = static_cast< D3D11_DSV_DIMENSION >(DepthView.getDescriptor().Dimension);
  descDSV.Texture2D.MipSlice = DepthView.getDescriptor().Mip;

  hr = mptr_device->CreateDepthStencilView(DepthView.getTexture(), &descDSV, DepthView.getDepthStencilViewRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
#elif OPEN_GL
  /* TODO :*/
  GlRemoveAllErrors();
  glEnable(GL_DEPTH_TEST);
  glGenRenderbuffers(1, DepthView.getDepthStencil().getIDPtr());
  glBindRenderbuffer(GL_RENDERBUFFER, DepthView.getDepthStencil().getID());

  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                        DepthView.getWidth(),
                        DepthView.getHeight());

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  if (!GlCheckForError())
  {
    return true;
  }
  else
  {
    return false;
  }

#endif // DIRECTX
  return false;
}

bool
cDevice::CreateVertexShader(cVertexShader & vertexShaderPath)
{
#if DIRECTX
  HRESULT hr;
  hr = mptr_device->CreateVertexShader(vertexShaderPath.getInfo()->GetBufferPointer(),
                                       vertexShaderPath.getInfo()->GetBufferSize(),
                                       NULL,
                                       vertexShaderPath.getVertexShaderRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
#elif OPEN_GL
  GlRemoveAllErrors();
  glAttachShader(*cApiComponents::getShaderProgram(), vertexShaderPath.getID());
  glLinkProgram(*cApiComponents::getShaderProgram());
  glValidateProgram(*cApiComponents::getShaderProgram());

  int Status;
  glGetProgramiv(*cApiComponents::getShaderProgram(), GL_VALIDATE_STATUS, &Status);
  if (Status == GL_FALSE)
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::ShaderLinkError);
  }

  if (GlCheckForError())
  {
    return false;
  }

  return true;
#endif // DIRECTX

  return false;
}

bool cDevice::CreatePixelShader(cPixelShader & pixelShader)
{
#if DIRECTX
  HRESULT hr;
  hr = mptr_device->CreatePixelShader(pixelShader.getInfo()->GetBufferPointer(),
                                      pixelShader.getInfo()->GetBufferSize(),
                                      NULL,
                                      pixelShader.getPixelShaderRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
#elif  OPEN_GL
  GlRemoveAllErrors();
  glAttachShader(*cApiComponents::getShaderProgram(), pixelShader.getID());
  glLinkProgram(*cApiComponents::getShaderProgram());
  glValidateProgram(*cApiComponents::getShaderProgram());

  int Status;
  glGetProgramiv(*cApiComponents::getShaderProgram(), GL_VALIDATE_STATUS, &Status);
  if (Status == GL_FALSE)
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::ShaderLinkError);
  }

  if (GlCheckForError())
  {
    return false;
  }

  return true;
#endif // DIRECTX

  return false;
}

bool cDevice::CreateInputLayout(cInputLayout &inputLayout
                                , uint32_t NumElements,
                                sInputDescriptor intermediateDesc[],
                                cVertexShader &vertexShaderPath)
{
#if DIRECTX
  HRESULT hr;
  std::vector<D3D11_INPUT_ELEMENT_DESC> ElementDesc;
  for (int i = 0; i < NumElements; ++i)
  {
    D3D11_INPUT_ELEMENT_DESC Desc;
    Desc.Format = static_cast< DXGI_FORMAT >(intermediateDesc[i].Format);
    Desc.AlignedByteOffset = ( UINT )intermediateDesc[i].Alignment;
    Desc.SemanticIndex = intermediateDesc[i].Index;
    Desc.SemanticName = intermediateDesc[i].Name.c_str();
    Desc.InputSlot = intermediateDesc[i].Slot;
    Desc.InputSlotClass = static_cast< D3D11_INPUT_CLASSIFICATION > (intermediateDesc[i].SlotClass);
    Desc.InstanceDataStepRate = intermediateDesc[i].InstanceData;
    ElementDesc.emplace_back(Desc);
  }
  hr = mptr_device->CreateInputLayout(&ElementDesc[0],
                                      NumElements,
                                      vertexShaderPath.getInfo()->GetBufferPointer(),
                                      vertexShaderPath.getInfo()->GetBufferSize(),
                                      inputLayout.getInputLayoutRef());
  if (SUCCEEDED(hr))
  {
    return true;
  }
#elif OPEN_GL
  GlRemoveAllErrors();

  glEnableVertexAttribArray(0);// position 
  glEnableVertexAttribArray(1);// normals 
  glEnableVertexAttribArray(2);// textures

  if (GlCheckForError())
  {
    EN_LOG_ERROR;
    return false;
  }

  //TODO:  create input layout  
  return true;
#endif // DIRECTX
  return false;
}

bool cDevice::CreateInputLayout(cInputLayout & inputLayout, cVertexShader & vertexShaderPath)
{
#if DIRECTX
  //directX native input layout struct 
  std::vector<D3D11_INPUT_ELEMENT_DESC> directxInputLayout;
  // my input layout struct 
  std::vector<sInputDescriptor> intermidateLayout = inputLayout.getInputDescriptor();
  /*convert my intermediate input layout format to
  directX input layout format */
  for (const sInputDescriptor &intermidate : intermidateLayout)
  {
    D3D11_INPUT_ELEMENT_DESC directxDesc;
    directxDesc.Format = static_cast< DXGI_FORMAT >(intermidate.Format);
    directxDesc.AlignedByteOffset = static_cast< UINT >(intermidate.Alignment);
    directxDesc.SemanticIndex = intermidate.Index;
    directxDesc.SemanticName = intermidate.Name.c_str();
    directxDesc.InputSlot = intermidate.Slot;
    directxDesc.InputSlotClass = static_cast< D3D11_INPUT_CLASSIFICATION > (intermidate.SlotClass);
    directxDesc.InstanceDataStepRate = intermidate.InstanceData;
    directxInputLayout.emplace_back(directxDesc);
  }

  HRESULT  hr = mptr_device->CreateInputLayout(&directxInputLayout[0],
                                               intermidateLayout.size(),
                                               vertexShaderPath.getInfo()->GetBufferPointer(),
                                               vertexShaderPath.getInfo()->GetBufferSize(),
                                               inputLayout.getInputLayoutRef());
  if (SUCCEEDED(hr))
  {
    return true;
  }
#elif OPEN_GL
  GlRemoveAllErrors();
  // TODO : create input layout 
  glEnableVertexAttribArray(0);// position 
  glEnableVertexAttribArray(1);// normals 
  glEnableVertexAttribArray(2);// textures
  //glGetVertexArrayiv()

  if (GlCheckForError())
  {
    EN_LOG_ERROR
      return false;
  }
  return true;
#endif // DIRECTX
  return false;
}

bool cDevice::CreateVertexBuffer(cVertexBuffer & VertexBuffer)
{
  if (VertexBuffer.getBufferType() != enBufferType::Vertex)
  {
    OutputDebugStringA("inserted the wrong buffer in CreateVertexBuffer function ");
    return false;
  }
#if DIRECTX
  HRESULT hr;
  D3D11_BUFFER_DESC directxDesc = VertexBuffer.getDirectXDesc();
  D3D11_SUBRESOURCE_DATA subresource = VertexBuffer.getSubresource();

  hr = mptr_device->CreateBuffer(&directxDesc,
                                 &subresource,
                                 VertexBuffer.getBufferRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
  return false;
#elif OPEN_GL
  GlRemoveAllErrors();

  glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer.getID());

  glBufferData(GL_ARRAY_BUFFER, // buffer type 
               VertexBuffer.getBufferSize(),
               VertexBuffer.getData(), GL_DYNAMIC_DRAW);


  int32 size;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

  if (size != VertexBuffer.getBufferSize())
  {
    EN_LOG_ERROR;
    return false;
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);


  if (GlCheckForError())
  {
    EN_LOG_ERROR;
    return false;
  }
  else
  { return true; }

#endif // DIRECTX
  return false;
}

bool cDevice::CreateIndexBuffer(cIndexBuffer & indexBuffer)
{
  if (indexBuffer.getBufferType() != enBufferType::Index)
  {
    OutputDebugStringA("inserted the wrong buffer in CreateIndexBuffer function ");
    return false;
  }
#if DIRECTX
  HRESULT hr;
  D3D11_BUFFER_DESC directxDesc = indexBuffer.getDirectXDesc();
  D3D11_SUBRESOURCE_DATA subresource = indexBuffer.getSubresource();

  hr = mptr_device->CreateBuffer(&directxDesc,
                                 &subresource,
                                 indexBuffer.getBufferRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
  return false;
#elif OPEN_GL 
  GlRemoveAllErrors();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.getID());

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, // buffer type 
               indexBuffer.getBufferSize(),
               indexBuffer.getData(), GL_STATIC_DRAW);


  int32 size;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

  if (size != indexBuffer.getBufferSize())
  {
    EN_LOG_ERROR;
    return false;
  }


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if (GlCheckForError())
  {
    EN_LOG_ERROR;
    return false;
  }
  else
  { return true; }
#endif // DIRECTX
  return true;
}

bool cDevice::CreateConstBuffer(cConstBuffer & constBuffer)
{
#if DIRECTX
  HRESULT hr;
  D3D11_BUFFER_DESC directxDesc = constBuffer.getDirectXDesc();

  hr = mptr_device->CreateBuffer(&directxDesc,
                                 nullptr,
                                 constBuffer.getBufferRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
  return false;
#elif OPEN_GL
  GlRemoveAllErrors();

  int ActiveUniformCount;
  uint32_t * shaderProgram = cApiComponents::getShaderProgram();

  glUseProgram(*shaderProgram);

  glGetProgramiv(*shaderProgram, GL_ACTIVE_UNIFORMS, &ActiveUniformCount);

  // the view matrix 
  if (constBuffer.getIndex() == 0)
  {
    auto location = glGetUniformLocation(*shaderProgram, "uView");
    *constBuffer.getIDPtr() = location;
  }
  // the projection matrix 
  else if (constBuffer.getIndex() == 1)
  {
    auto location = glGetUniformLocation(*shaderProgram, "uProjection");
    *constBuffer.getIDPtr() = location;
  }
  // the world matrix 
  else if (constBuffer.getIndex() == 2)
  {
    //glUniformBlockBinding()
    *constBuffer.getIDPtr() = glGetUniformBlockIndex(*shaderProgram, "u_worldAndColor");
    glGenBuffers(1, constBuffer.getGlUniformBlockIDPtr());
    std::cout << "uniform block id for u_worldAndColor [" << constBuffer.getGlUniformBlockID() << ']' << '\n';
    std::cout << "regular buffer id for u_worldAndColor [" << constBuffer.getID() << ']' << '\n';
    glBindBuffer(GL_UNIFORM_BUFFER, constBuffer.getGlUniformBlockID());

    //constBuffer.setData(glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
  else if (constBuffer.getIndex() == 3)
  {
    auto refToContainer = constBuffer.getGlUniforms();
    refToContainer->push_back(helper::GlCreateUniformDetail("uAmbientColor", enConstBufferElem::vec4));
    refToContainer->push_back(helper::GlCreateUniformDetail("uDiffuseColor", enConstBufferElem::vec4));
    refToContainer->push_back(helper::GlCreateUniformDetail("uSpecularColor", enConstBufferElem::vec4));
    refToContainer->push_back(helper::GlCreateUniformDetail("uLightPos", enConstBufferElem::vec4));
    refToContainer->push_back(helper::GlCreateUniformDetail("uLightDir", enConstBufferElem::vec3));
    refToContainer->push_back(helper::GlCreateUniformDetail("uLightIntensity", enConstBufferElem::single_float));
    refToContainer->push_back(helper::GlCreateUniformDetail("uAmbientIntensity", enConstBufferElem::single_float));
    refToContainer->push_back(helper::GlCreateUniformDetail("uSpecularIntensity", enConstBufferElem::single_float));


    for (sUniformDetails &uni : *refToContainer)
    {
      uni.id = glGetUniformLocation(*shaderProgram, uni.name.c_str());
    }
  }

  if (GlCheckForError())
  {
    EN_LOG_ERROR;
    return false;
  }
  return true;

#endif // DIRECTX
  return true;
}

bool cDevice::CreateSamplerState(cSampler & sampler)
{
#if DIRECTX
  HRESULT hr;
  D3D11_SAMPLER_DESC directxSampler = sampler.getDirectxSamplerDesc();
  hr = mptr_device->CreateSamplerState(&directxSampler,
                                       sampler.getSamplerRef());

  if (SUCCEEDED(hr))
  {
    return true;
  }
  return false;
#elif OPEN_GL
  //TODO : add sampler function 
  GlRemoveAllErrors();

  if (GlCheckForError())
  {
    EN_LOG_ERROR;
    return false;
  }
  else
  {
    return true;
  }
#endif // DIRECTX
  return false;
}


#if DIRECTX
ID3D11Device * cDevice::getDevice()
{
  return mptr_device;
}

ID3D11Device ** cDevice::getDeviceRef()
{
  return &mptr_device;
}
#endif // DIRECTX

