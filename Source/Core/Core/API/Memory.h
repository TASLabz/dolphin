// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "Common/CommonTypes.h"
#include "Core/HW/Memmap.h"
#include "Core/System.h"
#include "Core/PowerPC/JitInterface.h"

namespace API::Memory
{

// memchecks

void AddMemcheck(u32 addr);
void RemoveMemcheck(u32 addr);

// memory reading: just directly forward to the MMU

inline u8 Read_U8(const Core::CPUThreadGuard &guard, u32 addr)
{
  return PowerPC::HostRead_U8(guard, addr);
}

inline u16 Read_U16(const Core::CPUThreadGuard &guard, u32 addr)
{
  return PowerPC::HostRead_U16(guard, addr);
}

inline u32 Read_U32(const Core::CPUThreadGuard &guard, u32 addr)
{
  return PowerPC::HostRead_U32(guard, addr);
}

inline u64 Read_U64(const Core::CPUThreadGuard &guard, u32 addr)
{
  return PowerPC::HostRead_U64(guard, addr);
}

inline s8 Read_S8(const Core::CPUThreadGuard &guard, u32 addr)
{
  return PowerPC::HostRead_S8(guard, addr);
}

inline s16 Read_S16(const Core::CPUThreadGuard &guard, u32 addr)
{
  return PowerPC::HostRead_S16(guard, addr);
}

inline s32 Read_S32(const Core::CPUThreadGuard &guard, u32 addr)
{
  return PowerPC::HostRead_S32(guard, addr);
}

inline s64 Read_S64(const Core::CPUThreadGuard &guard, u32 addr)
{
  return PowerPC::HostRead_S64(guard, addr);
}

inline float Read_F32(const Core::CPUThreadGuard &guard, u32 addr)
{
  return PowerPC::HostRead_F32(guard, addr);
}

inline double Read_F64(const Core::CPUThreadGuard &guard, u32 addr)
{
  return PowerPC::HostRead_F64(guard, addr);
}

// memory writing: arguments of write functions are swapped (address first) to be consistent with other scripting APIs

inline void InvalidateICache(const Core::CPUThreadGuard &guard, u32 addr, u32 size)
{
  (void)guard;
  Core::System::GetInstance().GetJitInterface().InvalidateICache(addr, size, true);
}

inline void Write_U8(const Core::CPUThreadGuard &guard, u32 addr, u8 val)
{
  PowerPC::HostWrite_U8(guard, val, addr);
}

inline void Write_U16(const Core::CPUThreadGuard &guard, u32 addr, u16 val)
{
  PowerPC::HostWrite_U16(guard, val, addr);
}

inline void Write_U32(const Core::CPUThreadGuard &guard, u32 addr, u32 val)
{
  PowerPC::HostWrite_U32(guard, val, addr);
}

inline void Write_U64(const Core::CPUThreadGuard &guard, u32 addr, u64 val)
{
  PowerPC::HostWrite_U64(guard, val, addr);
}

inline void Write_S8(const Core::CPUThreadGuard &guard, u32 addr, s8 val)
{
  PowerPC::HostWrite_S8(guard, val, addr);
}

inline void Write_S16(const Core::CPUThreadGuard &guard, u32 addr, s16 val)
{
  PowerPC::HostWrite_S16(guard, val, addr);
}

inline void Write_S32(const Core::CPUThreadGuard &guard, u32 addr, s32 val)
{
  PowerPC::HostWrite_S32(guard, val, addr);
}

inline void Write_S64(const Core::CPUThreadGuard &guard, u32 addr, s64 val)
{
  PowerPC::HostWrite_S64(guard, val, addr);
}

inline void Write_F32(const Core::CPUThreadGuard &guard, u32 addr, float val)
{
  PowerPC::HostWrite_F32(guard, val, addr);
}

inline void Write_F64(const Core::CPUThreadGuard &guard, u32 addr, double val)
{
  PowerPC::HostWrite_F64(guard, val, addr);
}

}  // namespace API::Memory