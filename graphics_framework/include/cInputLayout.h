#pragma once
#include"utility/enGraphics.h"
#include <string>
#include <cstdint>
#include <vector>
// FORWARD DECLARATIONS
class cVertexShader;

/*! tells the input layout how to interpret the data */
struct sInputDescriptor
{
  std::string Name{ "" };
  uint32_t Index{ 0 };
  uint32_t Format{ 0 };
  uint32_t Slot{ 0 };
  uint32_t Alignment{ 0 };
  uint32_t SlotClass{ 0 };
  uint32_t InstanceData{ 0 };
};

/*!\todo make this class be take in a dynamic layout */
class cInputLayout
{
public:
  cInputLayout();

  ~cInputLayout();

#if DIRECTX
  //! for interfacing with directX 
  ID3D11InputLayout
    *getInputLayout();

    //! for functions that require an address to a pointer 
  ID3D11InputLayout
    **getInputLayoutRef();
#endif // DIRECTX
public:

  bool
    ReadShaderData(cVertexShader& ShaderData, bool isPerVertex = true);

  std::vector<sInputDescriptor>
    getInputDescriptor() const;

private:
#if DIRECTX
  ID3D11InputLayout *mptr_inputLayout;
#endif // DIRECTX
  std::vector<sInputDescriptor> m_InputLayouts;
};

