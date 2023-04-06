// Copyright 2023 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Scripting/ScriptList.h"

namespace Scripts
{
void StartPendingScripts()
{
  for (int i = 0; i < g_scripts.size(); i++)
  {
    if (g_scripts[i].enabled && !g_scripts[i].backend)
      g_scripts[i].backend = new Scripting::ScriptingBackend(g_scripts[i].path);
  }
  g_scripts_started = true;
}

// called on game close
// maintain enabled flag, but delete backends
void StopAllScripts()
{
  for (int i = 0; i < g_scripts.size(); i++)
  {
    if (g_scripts[i].backend)
    {
      delete g_scripts[i].backend;
      g_scripts[i].backend = nullptr;
    }
  }
  g_scripts_started = false;
}

std::vector<Script> g_scripts = {};
bool g_scripts_started = false;
}  // namespace Scripts
