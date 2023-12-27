// Copyright 2023 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "debugmodule.h"

#include <Python.h>

#include "Common/Logging/Log.h"
#include "Core/Core.h"
#include "Core/PowerPC/BreakPoints.h"
#include "Core/PowerPC/PowerPC.h"
#include "Core/System.h"
#include "Scripting/Python/Utils/as_py_func.h"
#include "Scripting/Python/Utils/module.h"

namespace PyScripting
{

struct DebugModuleState
{
};

static PyObject* SetBreakpoint(PyObject* self, PyObject* args)
{
  auto args_opt = Py::ParseTuple<u32>(args);
  if (!args_opt.has_value())
    return nullptr;
  u32 addr = std::get<0>(args_opt.value());

  Core::System::GetInstance().GetPowerPC().GetBreakPoints().Add(addr, false);

  // TODO: How can we inform the BreakpointWidget to update the list of breakpoints?
  Py_RETURN_NONE;
}

static PyObject* RemoveBreakpoint(PyObject* self, PyObject* args)
{
  auto args_opt = Py::ParseTuple<u32>(args);
  if (!args_opt.has_value())
    return nullptr;
  u32 addr = std::get<0>(args_opt.value());

  Core::System::GetInstance().GetPowerPC().GetBreakPoints().Remove(addr);

  Py_RETURN_NONE;
}

// Take in a dictionary, because some of these parameters might not be specified
static PyObject* SetMemoryBreakpoint(PyObject* self, PyObject* args)
{
  PyObject* dict = PyTuple_GetItem(args, 0);
  if (!PyDict_Check(dict))
    return nullptr;

  TMemCheck check;

  // Did user specify a single address or do they want to breakpoint a range?
  PyObject* at_obj = PyDict_GetItemString(dict, "At");
  PyObject* start_obj = PyDict_GetItemString(dict, "Start");
  PyObject* end_obj = PyDict_GetItemString(dict, "End");
  if (at_obj)
  {
    u32 addr = PyLong_AsUnsignedLong(at_obj);
    check.start_address = addr;
    check.end_address = addr;
    check.is_ranged = false;
  }
  else if (start_obj && end_obj)
  {
    check.start_address = PyLong_AsUnsignedLong(start_obj);
    check.end_address = PyLong_AsUnsignedLong(end_obj);
    check.is_ranged = true;
  }
  else
  {
    ERROR_LOG_FMT(SCRIPTING, "No \"At\" or \"Start\" and \"End\" addresses provided for Memory Breakpoint.");
    Py_RETURN_NONE;
  }

  PyObject* break_read_obj = PyDict_GetItemString(dict, "BreakOnRead");
  if (break_read_obj)
    check.is_break_on_read = PyObject_IsTrue(break_read_obj);
  else
    check.is_break_on_read = true;

  PyObject* break_write_obj = PyDict_GetItemString(dict, "BreakOnWrite");
  if (break_write_obj)
    check.is_break_on_write = PyObject_IsTrue(break_write_obj);
  else
    check.is_break_on_write = false;

  PyObject* log_hit_obj = PyDict_GetItemString(dict, "LogOnHit");
  if (log_hit_obj)
    check.log_on_hit = PyObject_IsTrue(log_hit_obj);
  else
    check.log_on_hit = true;

  PyObject* break_hit_obj = PyDict_GetItemString(dict, "BreakOnHit");
  if (break_hit_obj)
    check.break_on_hit = PyObject_IsTrue(break_hit_obj);
  else
    check.break_on_hit = true;

  PyObject* expr_obj = PyDict_GetItemString(dict, "Condition");
  if (expr_obj)
  {
    const char* expr = PyUnicode_AsUTF8(expr_obj);
    check.condition = Expression::TryParse(std::string_view(expr));
  }
  else
    check.condition = std::nullopt;

  Core::System::GetInstance().GetPowerPC().GetMemChecks().Add(std::move(check));

  Py_RETURN_NONE;
}

static PyObject* RemoveMemoryBreakpoint(PyObject* self, PyObject* args)
{
  auto args_opt = Py::ParseTuple<u32>(args);
  if (!args_opt.has_value())
    return nullptr;
  u32 addr = std::get<0>(args_opt.value());

  Core::System::GetInstance().GetPowerPC().GetMemChecks().Remove(addr);

  Py_RETURN_NONE;
}

static void SetupDebugModule(PyObject* module, DebugModuleState* state)
{
}

PyMODINIT_FUNC PyInit_debug()
{
  static PyMethodDef methods[] = {
      {"set_breakpoint", SetBreakpoint, METH_VARARGS, ""},
      {"remove_breakpoint", RemoveBreakpoint, METH_VARARGS, ""},
      {"set_memory_breakpoint", SetMemoryBreakpoint, METH_VARARGS, ""},
      {"remove_memory_breakpoint", RemoveMemoryBreakpoint, METH_VARARGS, ""},

      {nullptr, nullptr, 0, nullptr}  // Sentinel
  };
  static PyModuleDef module_def =
      Py::MakeStatefulModuleDef<DebugModuleState, SetupDebugModule>("debug", methods);
  PyObject* def_obj = PyModuleDef_Init(&module_def);
  return def_obj;
}

}  // namespace PyScripting
