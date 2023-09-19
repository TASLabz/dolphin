// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Registers.h"
#include "Core/System.h"
#include "Core/Core.h"
#include "Core/HW/CPU.h"

namespace API::Registers
{

// Register reading

u32 Read_GPR(u32 index)
{
  Core::CPUThreadGuard guard(Core::System::GetInstance());
  return guard.GetSystem().GetPPCState().gpr[index];
}

double Read_FPR(u32 index)
{
  Core::CPUThreadGuard guard(Core::System::GetInstance());
  return guard.GetSystem().GetPPCState().ps[index].PS0AsDouble();
}

// register writing

void Write_GPR(u32 index, u32 value)
{
  Core::CPUThreadGuard guard(Core::System::GetInstance());
  guard.GetSystem().GetPPCState().gpr[index] = value;
}

void Write_FPR(u32 index, double value)
{
  Core::CPUThreadGuard guard(Core::System::GetInstance());
  guard.GetSystem().GetPPCState().ps[index].SetPS0(value);
}

}  // namespace API::Registers
