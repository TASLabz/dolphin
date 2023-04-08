// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "ScriptsListModel.h"

#include "Core/Core.h"

#include "Scripting/ScriptList.h"

int ScriptsListModel::rowCount(const QModelIndex& parent) const
{
  return static_cast<int>(Scripts::g_scripts.size());
}

int ScriptsListModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

Qt::ItemFlags ScriptsListModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags result = QAbstractItemModel::flags(index);
  if (index.column() == 0)
    result |= Qt::ItemIsUserCheckable;

  return result;
}

QVariant ScriptsListModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || (size_t)index.row() >= Scripts::g_scripts.size())
    return QVariant();

  switch (role)
  {
  case Qt::DisplayRole:
  case Qt::EditRole:
  {
    if (index.column() == 0)
      return QVariant(
          QString::fromStdString(Scripts::g_scripts[index.row()].path.filename().string()));
    return QVariant();
  }
  case Qt::CheckStateRole:
  {
    if (index.column() == 0)
      return Scripts::g_scripts[index.row()].enabled ? Qt::Checked : Qt::Unchecked;
    return QVariant();
  }
  default:
    return QVariant();
  }
}

bool ScriptsListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid() || index.column() > 0)
    return false;

  switch (role)
  {
  case Qt::CheckStateRole:
  {
    Scripts::g_scripts[index.row()].enabled =
        ((Qt::CheckState)value.toUInt() == Qt::Checked) ? true : false;
    if (Scripts::g_scripts[index.row()].enabled)
    {
      Scripting::ScriptingBackend* backend = nullptr;
      if (Scripts::g_scripts_started)
        backend = new Scripting::ScriptingBackend(Scripts::g_scripts[index.row()].path);
      Scripts::g_scripts[index.row()].backend = backend;
    }
    else
    {
      delete Scripts::g_scripts[index.row()].backend;
      Scripts::g_scripts[index.row()].backend = nullptr;
    }
    return true;
  }
  default:
    return false;
  }
}

void ScriptsListModel::Add(std::filesystem::path path, bool enabled /* = false */)
{
  beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());
  Scripting::ScriptingBackend* backend = nullptr;
  if (enabled && Scripts::g_scripts_started)
    backend = new Scripting::ScriptingBackend(path);
  Scripts::g_scripts.emplace_back(Scripts::Script{path, backend, enabled});
  endInsertRows();
}

void ScriptsListModel::Restart(int index)
{
  // If script was not enabled, then we don't need to do anything
  if (!Scripts::g_scripts.at(index).enabled)
    return;

  std::filesystem::path path = Scripts::g_scripts.at(index).path;
  delete Scripts::g_scripts.at(index).backend;

  Scripting::ScriptingBackend* backend = nullptr;
  if (Scripts::g_scripts_started)
    backend = new Scripting::ScriptingBackend(path);

  Scripts::g_scripts.at(index) = Scripts::Script{path, backend, true};
}

void ScriptsListModel::Remove(int index)
{
  beginRemoveRows(QModelIndex(), index, index);
  Scripts::g_scripts.erase(Scripts::g_scripts.begin() + index);
  endRemoveRows();
}
