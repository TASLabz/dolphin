// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QEvent>
#include <QKeyEvent>
#include <QTextEdit>
#include <QLabel>
#include <QSplitter>
#include <QWidget>
#include <QString>
#include <QListView>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDirIterator>

#include "Common/MsgHandler.h"
#include "Common/FileUtil.h"
#include "DolphinQt/Resources.h"
#include "DolphinQt/Scripting/ScriptingWidget.h"
#include "DolphinQt/Settings.h"

#include "Scripting/ScriptList.h"
#include "Scripting/ScriptingEngine.h"

static QSize ICON_SIZE(16, 16);

ScriptingWidget::ScriptingWidget(QWidget* parent)
{
  setWindowTitle(tr("Scripts"));

  // actions
  m_button_add_new = new QPushButton();
  m_button_reload_selected = new QPushButton();
  m_button_remove_selected = new QPushButton();
  UpdateIcons();

  QHBoxLayout* actions_layout = new QHBoxLayout;
  actions_layout->addWidget(m_button_add_new);
  actions_layout->addWidget(m_button_reload_selected);
  actions_layout->addWidget(m_button_remove_selected);
  QWidget* actions_widget = new QWidget;
  actions_widget->setLayout(actions_layout);

  // table view
  m_scripts_model = new ScriptsListModel();
  m_table_view = new QTableView();
  m_table_view->setModel(m_scripts_model);
  m_table_view->horizontalHeader()->setStretchLastSection(true);
  m_table_view->horizontalHeader()->hide();
  m_table_view->verticalHeader()->hide();
  m_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table_view->installEventFilter(this);

  // put in group box
  QGroupBox* scripts_group = new QGroupBox(tr("Loaded Scripts"));
  QVBoxLayout* scripts_layout = new QVBoxLayout;
  scripts_group->setLayout(scripts_layout);
  scripts_layout->addWidget(m_table_view);

  QVBoxLayout* main_layout = new QVBoxLayout;
  main_layout->addWidget(actions_widget);
  main_layout->addWidget(scripts_group);
  QWidget* main_widget = new QWidget;
  main_widget->setLayout(main_layout);
  this->setWidget(main_widget);

  connect(&Settings::Instance(), &Settings::ScriptingVisibilityChanged, this,
          &ScriptingWidget::setVisible);

  connect(m_button_add_new, &QPushButton::clicked, this, &ScriptingWidget::AddNewScript);
  connect(m_button_reload_selected, &QPushButton::clicked, this,
          &ScriptingWidget::RestartSelectedScripts);
  connect(m_button_remove_selected, &QPushButton::clicked, this,
          &ScriptingWidget::RemoveSelectedScripts);
  connect(&Settings::Instance(), &Settings::ThemeChanged, this, &ScriptingWidget::UpdateIcons);

  PopulateScripts();
}

void ScriptingWidget::UpdateIcons()
{
  m_button_add_new->setIcon(Resources::GetThemeIcon("add"));
  m_button_add_new->setIconSize(ICON_SIZE);

  m_button_reload_selected->setIcon(Resources::GetThemeIcon("refresh"));
  m_button_reload_selected->setIconSize(ICON_SIZE);

  m_button_remove_selected->setIcon(Resources::GetThemeIcon("remove"));
  m_button_remove_selected->setIconSize(ICON_SIZE);
}

// Reads the scripts present in Load/Scripts/ and adds to the widget list
void ScriptingWidget::PopulateScripts()
{
  QString dir = QString::fromStdString(File::GetUserPath(D_SCRIPTS_IDX));
  QStringList fileFilter{QStringLiteral("*.py"), QStringLiteral("*.py3")};
  QDirIterator it = QDirIterator(dir, fileFilter);

  while (it.hasNext())
  {
    QFileInfo file = it.nextFileInfo();
    bool autorun = file.fileName().at(0) == QChar(u'_');
    m_scripts_model->Add(file.filesystemAbsoluteFilePath(), autorun);
  }
};

// Keyboard shortcuts for scripting table control
bool ScriptingWidget::eventFilter(QObject* object, QEvent* event)
{
  if (event->type() == QEvent::KeyPress)
  {
    QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
    if (key_event->key() == Qt::Key_Delete)
    {
      RemoveSelectedScripts();
      return true;
    }
    if (key_event->key() == Qt::Key_Return)
    {
      ToggleSelectedScripts();
      return true;
    }
  }

  return false;
}

void ScriptingWidget::AddNewScript()
{
  if (Scripting::ScriptingBackend::PythonSubinterpretersDisabled() && m_scripts_model->rowCount() == 1)
  {
    CriticalAlertFmt("Can run at most one script at a time because Python subinterpreters are disabled");
    return;
  }

  QString filename = QFileDialog::getOpenFileName(
      this, tr("Add script"), QString(),
      tr("Python scripts (*.py *.py3)"));

  if (!filename.isEmpty())
  {
    m_scripts_model->Add(filename.toStdString());
  }
}

void ScriptingWidget::RestartSelectedScripts()
{
  for (const QModelIndex& q_index : m_table_view->selectionModel()->selectedRows())
  {
    m_scripts_model->Restart(q_index.row());
  }
}

void ScriptingWidget::RemoveSelectedScripts()
{
  int count_deleted = 0;
  for (const QModelIndex& q_index : m_table_view->selectionModel()->selectedRows())
  {
    m_scripts_model->Remove(q_index.row() - count_deleted++);
  }
}

void ScriptingWidget::AddScript(std::string filename, bool enabled /* = false */)
{
  m_scripts_model->Add(filename, enabled);
}

void ScriptingWidget::ToggleSelectedScripts()
{
  QModelIndexList index_list = m_table_view->selectionModel()->selectedRows();
  for (const QModelIndex& q_index : index_list)
  {
    Qt::CheckState prev_state = (Qt::CheckState) m_scripts_model->data(q_index, Qt::CheckStateRole).toUInt();
    m_scripts_model->setData(q_index, prev_state == Qt::Checked ? Qt::Unchecked : Qt::Checked,
                             Qt::CheckStateRole);
  }

  m_scripts_model->dataChanged(index_list.first(), index_list.last(),
                               QList<int>(Qt::CheckStateRole));
}

void ScriptingWidget::closeEvent(QCloseEvent*)
{
  Settings::Instance().SetScriptingVisible(false);
}
