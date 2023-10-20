// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "ScriptsListModel.h"

#include "Common/FileUtil.h"
#include "Core/Core.h"
#include "Scripting/ScriptList.h"

ScriptsFileSystemModel::ScriptsFileSystemModel(QObject* parent /* = nullptr */)
    : QFileSystemModel(parent)
{
  QStringList filters;
  filters.append(QString::fromStdString("*.py"));
  filters.append(QString::fromStdString("*.py3"));
  setNameFilters(filters);
  setNameFilterDisables(false);
  setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);

  AutoRunScripts();
}

void ScriptsFileSystemModel::AutoRunScripts()
{
  // In scripts dir, look for all files that start with an underscore
  // This is apparently very hard to do via the QFileSystemModel class,
  // because it does not compute subfolder files unless you expand the folder first.
  QString dir = QString::fromStdString(File::GetUserPath(D_SCRIPTS_IDX));
  QStringList nameFilter{QStringLiteral("_*.py"), QStringLiteral("_*.py3")};
  QDirIterator it = QDirIterator(dir, nameFilter, QDir::NoFilter, QDirIterator::Subdirectories);

  while (it.hasNext())
  {
    QFileInfo file = it.nextFileInfo();

    // Ignore __init__.py files, as these are used to make subfolder modules visible
    if (file.fileName() == QStringLiteral("__init__.py"))
      continue;

    Scripts::g_scripts[file.absoluteFilePath().toStdString()] = nullptr;
  }
}

Qt::ItemFlags ScriptsFileSystemModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags result = QAbstractItemModel::flags(index);
  if (index.column() == 0)
    result |= Qt::ItemIsUserCheckable;

  return result;
}

QVariant ScriptsFileSystemModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  // Hide filetype icons except for folders
  if (role == Qt::DecorationRole && !hasChildren(index))
    return QVariant();

  if (role == Qt::CheckStateRole)
  {
    if (hasChildren(index))
      return QVariant();

    QString path = filePath(index);

    bool script_enabled = Scripts::g_scripts.find(path.toStdString()) != Scripts::g_scripts.end();
    return script_enabled ? Qt::Checked : Qt::Unchecked;
  }

  return QFileSystemModel::data(index, role);
}

bool ScriptsFileSystemModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid() || index.column() > 0)
    return false;

  switch (role)
  {
  case Qt::CheckStateRole:
  {
    // Check if the selected script is currently running
    std::string file_path = filePath(index).toStdString();
    if (Scripts::g_scripts.find(file_path) != Scripts::g_scripts.end())
    {
      Q_ASSERT((Qt::CheckState)value.toUInt() == Qt::Unchecked);

      // Disable the script
      delete Scripts::g_scripts[file_path];
      Scripts::g_scripts.erase(file_path);
    }
    else
    {
      Q_ASSERT((Qt::CheckState)value.toUInt() == Qt::Checked);

      // Add the script
      Scripting::ScriptingBackend* backend = nullptr;
      if (Scripts::g_scripts_started)
        backend = new Scripting::ScriptingBackend(file_path);
      Scripts::g_scripts[file_path] = backend;
    }

    emit ScriptsFileSystemModel::dataChanged(index, index, QList<int>{role});

    return true;
  }
  default:
    return false;
  }
}

void ScriptsFileSystemModel::Restart(const QModelIndex& index)
{
  if (!index.isValid())
    return;

  QString path = filePath(index);

  auto iter = Scripts::g_scripts.find(path.toStdString());
  if (iter != Scripts::g_scripts.end())
  {
    if (iter->second != nullptr)
    {
      delete iter->second;
      iter->second = new Scripting::ScriptingBackend(path.toStdString());
    }
  }
}
