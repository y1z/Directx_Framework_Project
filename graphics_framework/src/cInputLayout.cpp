
#include "d3d11shader.h"
#include "../include/cInputLayout.h"
#include "../include/cVertexShader.h"

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

ID3D11InputLayout *
cInputLayout::getInputLayout()
{
  return mptr_inputLayout;
}

ID3D11InputLayout **
cInputLayout::getInputLayoutRef()
{
  return &mptr_inputLayout;
}


#endif // DIRECTX

bool cInputLayout::ReadShaderData(cVertexShader & ShaderData, bool isPerVertex)
{	// checking for errors 
#if DIRECTX
  ID3D11ShaderReflection *ReflectorShader = nullptr;

  // to init a shader Reflection
  HRESULT hr = D3DReflect(ShaderData.getInfo()->GetBufferPointer(), ShaderData.getInfo()->GetBufferSize(),
                  IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&ReflectorShader));

  if (FAILED(hr))
  {
    return false;
  }

  ///---------------------------------
  D3D11_SHADER_DESC ShaderDesc;
  // to know how long to make the for loop
  ReflectorShader->GetDesc(&ShaderDesc);
  // helps to get the proper alignment 


  for (uint32_t i = 0; i < ShaderDesc.InputParameters; ++i)
  {
    // where to store the data i need for the input layout 
    D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
    ReflectorShader->GetInputParameterDesc(i, &paramDesc);
    sInputDescriptor InputLayout;

    InputLayout.Name = paramDesc.SemanticName;
    InputLayout.Index = paramDesc.SemanticIndex;
    InputLayout.SlotClass = 0;
    InputLayout.Slot = 0;

    if (isPerVertex)
    { InputLayout.InstanceData = D3D11_INPUT_PER_VERTEX_DATA; }
    else
    { InputLayout.InstanceData = D3D11_INPUT_PER_INSTANCE_DATA; }

    // determine DXGI format 
    //https://docs.microsoft.com/en-us/windows/desktop/api/d3dcommon/ne-d3dcommon-d3d_register_component_type
    // 
    if (paramDesc.Mask == 1)
    {
      // 1 component values
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
      {
        InputLayout.Format = DXGI_FORMAT_R32_UINT;
        InputLayout.Alignment = sizeof(uint32_t);
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
      {
        InputLayout.Format = DXGI_FORMAT_R32_SINT;
        InputLayout.Alignment = sizeof(int32_t);
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
      {
        InputLayout.Format = DXGI_FORMAT_R32_FLOAT;
        InputLayout.Alignment = sizeof(float);
      }
    }
    // 2 component values 
    else if (paramDesc.Mask <= 3)
    {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) { InputLayout.Format = DXGI_FORMAT_R32G32_UINT; }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) { InputLayout.Format = DXGI_FORMAT_R32G32_SINT; }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
      {
        InputLayout.Format = DXGI_FORMAT_R32G32_FLOAT;
        InputLayout.Alignment = sizeof(float) * 2;
      }
    }
    // 3 component values 
    else if (paramDesc.Mask <= 7)
    {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
      {
        InputLayout.Format = DXGI_FORMAT_R32G32B32_UINT;
        InputLayout.Alignment = sizeof(uint32_t) * 3;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
      {
        InputLayout.Format = DXGI_FORMAT_R32G32B32_SINT;
        InputLayout.Alignment = sizeof(int32_t) * 3;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
      {
        InputLayout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        InputLayout.Alignment = sizeof(float) * 3;
      }
    }
    // 4 component values 
    else if (paramDesc.Mask <= 15)
    {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
      {
        InputLayout.Format = DXGI_FORMAT_R32G32B32A32_UINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
      {
        InputLayout.Format = DXGI_FORMAT_R32G32B32A32_SINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
      {
        InputLayout.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        //InputLayout.Alignment = sizeof(float) * 4;
      }
    }
    else// when non of the others are recognized
    {
      // should work on everything
      InputLayout.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    }
    if (i == 0)
    {
      InputLayout.Alignment = 0;
    }
    else
    {
      InputLayout.Alignment = D3D11_APPEND_ALIGNED_ELEMENT;
    }

    m_InputLayouts.emplace_back(InputLayout);
  }
  // no longer needed 
  ReflectorShader->Release();
  return true;

#endif // DIRECTX
  return true;
}

std::vector<sInputDescriptor>
cInputLayout::getInputDescriptor() const
{
  return  m_InputLayouts;
}
