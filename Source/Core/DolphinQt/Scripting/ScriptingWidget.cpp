// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <QCheckBox>
#include <QDirIterator>
#include <QEvent>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QSplitter>
#include <QString>
#include <QStringList>
#include <QTextEdit>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

#include "Common/FileUtil.h"
#include "Common/MsgHandler.h"
#include "DolphinQt/QtUtils/ModalMessageBox.h"
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
  UpdateIcons();

  QHBoxLayout* actions_layout = new QHBoxLayout;
  actions_layout->addWidget(m_button_add_new);
  actions_layout->addWidget(m_button_reload_selected);
  QWidget* actions_widget = new QWidget;
  actions_widget->setLayout(actions_layout);

  m_scripts_model = new ScriptsFileSystemModel();
  QModelIndex rootIdx =
      m_scripts_model->setRootPath(QString::fromStdString(File::GetUserPath(D_SCRIPTS_IDX)));

  m_tree = new QTreeView();
  m_tree->setModel(m_scripts_model);
  m_tree->setRootIndex(rootIdx);
  m_tree->setHeaderHidden(true);
  // Hide size/type/date columns
  m_tree->hideColumn(1);
  m_tree->hideColumn(2);
  m_tree->hideColumn(3);

  QGroupBox* scripts_group = new QGroupBox(tr("Loaded Scripts"));
  QVBoxLayout* scripts_layout = new QVBoxLayout;
  scripts_group->setLayout(scripts_layout);
  scripts_layout->addWidget(m_tree);

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
  connect(&Settings::Instance(), &Settings::ThemeChanged, this, &ScriptingWidget::UpdateIcons);
  connect(m_tree, &QTreeView::doubleClicked, this, &ScriptingWidget::ToggleSelectedScripts);
}

void ScriptingWidget::UpdateIcons()
{
  m_button_add_new->setIcon(Resources::GetThemeIcon("add"));
  m_button_add_new->setIconSize(ICON_SIZE);

  m_button_reload_selected->setIcon(Resources::GetThemeIcon("refresh"));
  m_button_reload_selected->setIconSize(ICON_SIZE);
}

void ScriptingWidget::AddNewScript()
{
  QString filename = QFileDialog::getOpenFileName(this, tr("Add script"), QString(),
                                                  tr("Python scripts (*.py *.py3)"));
  QFileInfo finfo(filename);

  QString newPath = QString::fromStdString(File::GetUserPath(D_SCRIPTS_IDX)) + finfo.fileName();
  if (!QFile::copy(finfo.absoluteFilePath(), newPath))
  {
    ModalMessageBox::warning(this, tr("Failed to copy file"),
                             tr("Failed to copy %1 to %2. Another program may have these files "
                                "locked, or you may need elevated permissions.")
                                 .arg(filename, newPath),
                             QMessageBox::Ok);
  }
}

void ScriptingWidget::RestartSelectedScripts()
{
  m_scripts_model->Restart(m_tree->currentIndex());
}

void ScriptingWidget::ToggleSelectedScripts()
{
  QModelIndex index = m_tree->currentIndex();

  if (!index.isValid() || m_scripts_model->hasChildren(index))
    return;

  Qt::CheckState prev_state =
      (Qt::CheckState)m_scripts_model->data(index, Qt::CheckStateRole).toUInt();
  m_scripts_model->setData(index, prev_state == Qt::Checked ? Qt::Unchecked : Qt::Checked,
                           Qt::CheckStateRole);

  m_scripts_model->dataChanged(index, index, QList<int>(Qt::CheckStateRole));
}

void ScriptingWidget::closeEvent(QCloseEvent*)
{
  Settings::Instance().SetScriptingVisible(false);
}
