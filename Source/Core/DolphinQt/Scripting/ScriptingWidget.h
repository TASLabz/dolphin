// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <QDockWidget>
#include <QPushButton>
#include <QTableView>

#include "DolphinQt/Scripting/ScriptsListModel.h"

class ScriptingWidget : public QDockWidget
{
  Q_OBJECT
public:
  ScriptingWidget(QWidget* parent = nullptr);
  void UpdateIcons();
  void PopulateScripts();
  void AddNewScript();
  void RestartSelectedScripts();
  void RemoveSelectedScripts();
  void ToggleSelectedScripts();
  void AddScript(std::string filename, bool enabled = false);

protected:
  void closeEvent(QCloseEvent*) override;

private:
  bool eventFilter(QObject* object, QEvent* event);

  QPushButton* m_button_add_new;
  QPushButton* m_button_reload_selected;
  QPushButton* m_button_remove_selected;

  ScriptsListModel* m_scripts_model;
  QTableView* m_table_view;
};
