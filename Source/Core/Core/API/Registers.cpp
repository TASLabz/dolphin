// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Registers.h"

namespace API::Registers
{

// Register reading

u32 Read_GPR(u32 index)
{
  return PowerPC::ppcState.gpr[index];
}

double Read_FPR(u32 index)
{
  return PowerPC::ppcState.ps[index].PS0AsDouble();
}

// register writing

void Write_GPR(u32 index, u32 value)
{
  PowerPC::ppcState.gpr[index] = value;
}

void Write_FPR(u32 index, double value)
{
  PowerPC::ppcState.ps[index].SetPS0(value);
}

}  // namespace API::Registers
