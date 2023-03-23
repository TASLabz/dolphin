// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <QAbstractListModel>

#include "Scripting/ScriptingEngine.h"

struct Script
{
  std::string filename;
  Scripting::ScriptingBackend* backend;
  bool enabled;
};

class ScriptsListModel : public QAbstractListModel
{
public:
  ScriptsListModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  void Add(std::string filename, bool enabled = false);
  void Restart(int index);
  void Remove(int index);

private:
  std::vector<Script> m_scripts;
};
