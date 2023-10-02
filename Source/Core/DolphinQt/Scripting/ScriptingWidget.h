// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <QDockWidget>
#include <QFileSystemModel>
#include <QPushButton>
#include <QTableView>
#include <QTreeView>

#include "DolphinQt/Scripting/ScriptsListModel.h"

class ScriptingWidget : public QDockWidget
{
public:
  ScriptingWidget(QWidget* parent = nullptr);
  void UpdateIcons();
  void AddNewScript();
  void RestartSelectedScripts();
  void ToggleSelectedScripts();

protected:
  void closeEvent(QCloseEvent*) override;

private:
  void OpenScriptsFolder();

  QPushButton* m_button_add_new;
  QPushButton* m_button_reload_selected;
  QPushButton* m_button_open_folder;

  ScriptsFileSystemModel* m_scripts_model;
  QTreeView* m_tree;
};
