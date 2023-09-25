// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <QFileSystemModel>

class ScriptsFileSystemModel : public QFileSystemModel
{
public:
  ScriptsFileSystemModel(QObject* parent = nullptr);

  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  void Restart(const QModelIndex& index);
  void AutoRunScripts();
};
