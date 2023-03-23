// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "ScriptsListModel.h"

int ScriptsListModel::rowCount(const QModelIndex &parent) const
{
  return static_cast<int>(m_scripts.size());
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
  if (!index.isValid() || (size_t)index.row() >= m_scripts.size())
    return QVariant();

  switch (role)
  {
  case Qt::DisplayRole:
  case Qt::EditRole:
  {
    if (index.column() == 0)
      return QVariant(QString::fromStdString(m_scripts[index.row()].filename));
    return QVariant();
  }
  case Qt::CheckStateRole:
  {
    if (index.column() == 0)
      return m_scripts[index.row()].enabled ? Qt::Checked : Qt::Unchecked;
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
    m_scripts[index.row()].enabled = ((Qt::CheckState)value.toUInt() == Qt::Checked) ? true : false;
    if (m_scripts[index.row()].enabled)
    {
      m_scripts[index.row()].backend =
          new Scripting::ScriptingBackend(m_scripts[index.row()].filename);
    }
    else
    {
      delete m_scripts[index.row()].backend;
      m_scripts[index.row()].backend = nullptr;
    }
    return true;
  }
  default:
    return false;
  }
}

void ScriptsListModel::Add(std::string filename, bool enabled /* = false */)
{
  beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());
  m_scripts.emplace_back(Script{filename, nullptr, enabled});
  endInsertRows();
}

void ScriptsListModel::Restart(int index)
{
  // If script was not enabled, then we don't need to do anything
  if (!m_scripts.at(index).enabled)
    return;

  std::string filename = m_scripts.at(index).filename;
  delete m_scripts.at(index).backend;
  m_scripts.at(index) = Script{filename, new Scripting::ScriptingBackend(filename), true};
}

void ScriptsListModel::Remove(int index)
{
  beginRemoveRows(QModelIndex(), index, index);
  m_scripts.erase(m_scripts.begin() + index);
  endRemoveRows();
}
