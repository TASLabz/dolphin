// Copyright 2018 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "DolphinQt/TAS/GCTASInputWindow.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSpinBox>
#include <QVBoxLayout>

#include "Common/CommonTypes.h"

#include "Core/HW/GCPad.h"
#include "Core/HW/GCPadEmu.h"

#include "DolphinQt/TAS/StickWidget.h"
#include "DolphinQt/TAS/TASCheckBox.h"
#include "DolphinQt/TAS/TASStickBox.h"

#include "InputCommon/ControllerEmu/ControllerEmu.h"
#include "InputCommon/InputConfig.h"

GCTASInputWindow::GCTASInputWindow(QWidget* parent, int controller_id)
    : TASInputWindow(parent), m_controller_id(controller_id)
{
  setWindowTitle(tr("GameCube TAS Input %1").arg(controller_id + 1));

  m_main_stick_box = CreateStickInputs(tr("Main Stick"), GCPad::MAIN_STICK_GROUP, &m_overrider, 1,
                                       1, 255, 255, Qt::Key_F, Qt::Key_G);
  m_c_stick_box = CreateStickInputs(tr("C Stick"), GCPad::C_STICK_GROUP, &m_overrider, 1, 1, 255,
                                    255, Qt::Key_H, Qt::Key_J);

  auto* top_layout = new QHBoxLayout;
  top_layout->addWidget(m_main_stick_box);
  top_layout->addWidget(m_c_stick_box);

  CreateTriggersBox();
  CreateButtonsBox();

  // Combine triggers, buttons, and settings into a single row layout
  auto* trigger_and_buttons_layout = new QHBoxLayout();
  trigger_and_buttons_layout->addWidget(m_triggers_box);
  trigger_and_buttons_layout->addWidget(m_buttons_box);
  trigger_and_buttons_layout->addWidget(m_settings_box);
  trigger_and_buttons_layout->setAlignment(m_triggers_box, Qt::AlignTop);
  trigger_and_buttons_layout->setAlignment(m_buttons_box, Qt::AlignTop);
  trigger_and_buttons_layout->setAlignment(m_settings_box, Qt::AlignTop);

  auto* layout = new QVBoxLayout;
  layout->addLayout(top_layout);
  layout->addLayout(trigger_and_buttons_layout);

  setLayout(layout);

  setMinimumWidth(430);
  setMinimumHeight(390);

  connect(m_toggle_lines, &QCheckBox::toggled, m_main_stick_box->GetStickWidget(),
          &StickWidget::SetAxisLines);
  connect(m_toggle_lines, &QCheckBox::toggled, m_c_stick_box->GetStickWidget(),
          &StickWidget::SetAxisLines);
}

void GCTASInputWindow::CreateTriggersBox()
{
  m_triggers_box = new QGroupBox(tr("Triggers"));

  auto* l_trigger_layout = new QVBoxLayout();
  m_l_trigger_value =
      CreateSliderValuePair(GCPad::TRIGGERS_GROUP, GCPad::L_ANALOG, &m_overrider, l_trigger_layout,
                            0, 0, 0, 255, Qt::Key_N, Qt::Vertical, m_triggers_box);

  auto* r_trigger_layout = new QVBoxLayout();
  m_r_trigger_value =
      CreateSliderValuePair(GCPad::TRIGGERS_GROUP, GCPad::R_ANALOG, &m_overrider, r_trigger_layout,
                            0, 0, 0, 255, Qt::Key_M, Qt::Vertical, m_triggers_box);

  auto* triggers_layout = new QHBoxLayout;
  triggers_layout->addLayout(l_trigger_layout);
  triggers_layout->addLayout(r_trigger_layout);
  m_triggers_box->setLayout(triggers_layout);
}

void GCTASInputWindow::CreateButtonsBox()
{
  m_buttons_box = new QGroupBox(tr("Buttons"));

  m_a_button =
      CreateButton(QStringLiteral("&A"), GCPad::BUTTONS_GROUP, GCPad::A_BUTTON, &m_overrider);
  m_b_button =
      CreateButton(QStringLiteral("&B"), GCPad::BUTTONS_GROUP, GCPad::B_BUTTON, &m_overrider);
  m_x_button =
      CreateButton(QStringLiteral("&X"), GCPad::BUTTONS_GROUP, GCPad::X_BUTTON, &m_overrider);
  m_y_button =
      CreateButton(QStringLiteral("&Y"), GCPad::BUTTONS_GROUP, GCPad::Y_BUTTON, &m_overrider);
  m_l_button =
      CreateButton(QStringLiteral("&L"), GCPad::TRIGGERS_GROUP, GCPad::L_DIGITAL, &m_overrider);
  m_r_button =
      CreateButton(QStringLiteral("&R"), GCPad::TRIGGERS_GROUP, GCPad::R_DIGITAL, &m_overrider);
  m_z_button =
      CreateButton(QStringLiteral("&Z"), GCPad::BUTTONS_GROUP, GCPad::Z_BUTTON, &m_overrider);
  m_start_button =
      CreateButton(QStringLiteral("&S"), GCPad::BUTTONS_GROUP, GCPad::START_BUTTON, &m_overrider);

  m_left_button =
      CreateButton(QStringLiteral("L"), GCPad::DPAD_GROUP, DIRECTION_LEFT, &m_overrider);
  m_up_button = CreateButton(QStringLiteral("U"), GCPad::DPAD_GROUP, DIRECTION_UP, &m_overrider);
  m_down_button =
      CreateButton(QStringLiteral("D"), GCPad::DPAD_GROUP, DIRECTION_DOWN, &m_overrider);
  m_right_button =
      CreateButton(QStringLiteral("R"), GCPad::DPAD_GROUP, DIRECTION_RIGHT, &m_overrider);

  auto* buttons_layout = new QGridLayout;
  buttons_layout->addWidget(m_a_button, 0, 0);
  buttons_layout->addWidget(m_b_button, 0, 1);
  buttons_layout->addWidget(m_x_button, 0, 2);
  buttons_layout->addWidget(m_y_button, 0, 3);
  buttons_layout->addWidget(m_l_button, 1, 0);
  buttons_layout->addWidget(m_r_button, 1, 1);
  buttons_layout->addWidget(m_z_button, 1, 2);
  buttons_layout->addWidget(m_start_button, 1, 3);

  auto* dpad_layout = new QGridLayout;
  dpad_layout->addWidget(m_left_button, 1, 0);
  dpad_layout->addWidget(m_up_button, 0, 1);
  dpad_layout->addWidget(m_down_button, 2, 1);
  dpad_layout->addWidget(m_right_button, 1, 2);
  dpad_layout->setVerticalSpacing(0);
  dpad_layout->setHorizontalSpacing(25);

  auto* combined_layout = new QVBoxLayout();
  combined_layout->setAlignment(Qt::AlignTop);
  combined_layout->addLayout(buttons_layout);
  combined_layout->addLayout(dpad_layout);

  m_buttons_box->setLayout(combined_layout);
}

void GCTASInputWindow::hideEvent(QHideEvent* event)
{
  Pad::GetConfig()->GetController(m_controller_id)->ClearInputOverrideFunction();
}

void GCTASInputWindow::showEvent(QShowEvent* event)
{
  Pad::GetConfig()
      ->GetController(m_controller_id)
      ->SetInputOverrideFunction(m_overrider.GetInputOverrideFunction());
}
