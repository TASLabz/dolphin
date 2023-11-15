// Copyright 2023 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Scripting/ScriptList.h"

namespace Scripts
{
void StartPendingScripts()
{
  // NOTE: This may enable scripts in an arbitrary order
  for (auto it = g_scripts.begin(); it != g_scripts.end(); it++)
  {
    if (!it->second)
      it->second = new Scripting::ScriptingBackend(it->first);
  }
  g_scripts_started = true;
}

// called on game close
// maintain enabled flag, but delete backends
void StopAllScripts()
{
  for (auto it = g_scripts.begin(); it != g_scripts.end(); it++)
  {
    if (it->second)
    {
      delete it->second;
      it->second = nullptr;
    }
  }
  g_scripts_started = false;
}

std::unordered_map<std::string, Scripting::ScriptingBackend*> g_scripts = {};
bool g_scripts_started = false;
}  // namespace Scripts
