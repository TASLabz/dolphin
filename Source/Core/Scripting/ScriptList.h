// Copyright 2023 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "Scripting/ScriptingEngine.h"

#include <vector>

namespace Scripts
{
struct Script
{
  std::filesystem::path path;
  Scripting::ScriptingBackend* backend;
  bool enabled;
};

void StartPendingScripts();
void StopAllScripts();

// extern so that different translation units can access a global instance of these vars
// i.e. DolphinLib needs to access these variables even though they're housed in the Scripting unit
extern std::vector<Script> g_scripts;
extern bool g_scripts_started;
}
