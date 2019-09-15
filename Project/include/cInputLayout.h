#pragma once

#include"include/utiliy/Grafics_libs.h"

#include <string>
#include <cstdint>

/*! tells the input layout how to interpret the data */
struct sInputDescriptor
{
 std::string Name{""};
  uint32_t Index{0};
  uint32_t Format{0};
  uint32_t Slot{0};
  uint32_t Alignment{0};//alignment
  uint32_t SlotClass{0};
  uint32_t InstanceData{0};
};

/*!\todo make this class be take in a dynamic layout */
class cInputLayout
{
public:
  cInputLayout();

  ~cInputLayout();
  
  ID3D11InputLayout *getInputLayout();
  ID3D11InputLayout **getInputLayoutRef();

private:
  ID3D11InputLayout *mptr_inputLayout;
 };

