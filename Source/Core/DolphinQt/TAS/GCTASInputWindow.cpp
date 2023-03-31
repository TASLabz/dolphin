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

#include "DolphinQt/TAS/StickWidget.h"
#include "DolphinQt/TAS/TASCheckBox.h"
#include "DolphinQt/TAS/TASStickBox.h"

#include "InputCommon/GCPadStatus.h"

GCTASInputWindow::GCTASInputWindow(QWidget* parent, int num) : TASInputWindow(parent)
{
  setWindowTitle(tr("GameCube TAS Input %1").arg(num + 1));

  m_main_stick_box = CreateStickInputs(tr("Main Stick"), m_x_main_stick_value, m_y_main_stick_value,
                                       255, 255, Qt::Key_F, Qt::Key_G);
  m_c_stick_box = CreateStickInputs(tr("C Stick"), m_x_c_stick_value, m_y_c_stick_value, 255, 255,
                                    Qt::Key_H, Qt::Key_J);

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
      CreateSliderValuePair(l_trigger_layout, 0, 255, Qt::Key_N, Qt::Vertical, m_triggers_box);

  auto* r_trigger_layout = new QVBoxLayout();
  m_r_trigger_value =
      CreateSliderValuePair(r_trigger_layout, 0, 255, Qt::Key_M, Qt::Vertical, m_triggers_box);

  auto* triggers_layout = new QHBoxLayout;
  triggers_layout->addLayout(l_trigger_layout);
  triggers_layout->addLayout(r_trigger_layout);
  m_triggers_box->setLayout(triggers_layout);
}

void GCTASInputWindow::CreateButtonsBox()
{
  m_buttons_box = new QGroupBox(tr("Buttons"));

  m_a_button = CreateButton(QStringLiteral("&A"));
  m_b_button = CreateButton(QStringLiteral("&B"));
  m_x_button = CreateButton(QStringLiteral("&X"));
  m_y_button = CreateButton(QStringLiteral("&Y"));
  m_l_button = CreateButton(QStringLiteral("&L"));
  m_r_button = CreateButton(QStringLiteral("&R"));
  m_z_button = CreateButton(QStringLiteral("&Z"));
  m_start_button = CreateButton(QStringLiteral("&S"));

  m_left_button = CreateButton(QStringLiteral("L"));
  m_up_button = CreateButton(QStringLiteral("U"));
  m_down_button = CreateButton(QStringLiteral("D"));
  m_right_button = CreateButton(QStringLiteral("R"));

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

void GCTASInputWindow::GetValues(GCPadStatus* pad)
{
  if (!isVisible())
    return;

  GetButton<u16>(m_a_button, pad->button, PAD_BUTTON_A);
  GetButton<u16>(m_b_button, pad->button, PAD_BUTTON_B);
  GetButton<u16>(m_x_button, pad->button, PAD_BUTTON_X);
  GetButton<u16>(m_y_button, pad->button, PAD_BUTTON_Y);
  GetButton<u16>(m_z_button, pad->button, PAD_TRIGGER_Z);
  GetButton<u16>(m_l_button, pad->button, PAD_TRIGGER_L);
  GetButton<u16>(m_r_button, pad->button, PAD_TRIGGER_R);
  GetButton<u16>(m_left_button, pad->button, PAD_BUTTON_LEFT);
  GetButton<u16>(m_up_button, pad->button, PAD_BUTTON_UP);
  GetButton<u16>(m_down_button, pad->button, PAD_BUTTON_DOWN);
  GetButton<u16>(m_right_button, pad->button, PAD_BUTTON_RIGHT);
  GetButton<u16>(m_start_button, pad->button, PAD_BUTTON_START);

  if (m_a_button->isChecked())
    pad->analogA = 0xFF;
  else
    pad->analogA = 0x00;

  if (m_b_button->isChecked())
    pad->analogB = 0xFF;
  else
    pad->analogB = 0x00;

  GetSpinBoxU8(m_l_trigger_value, pad->triggerLeft);
  GetSpinBoxU8(m_r_trigger_value, pad->triggerRight);

  GetSpinBoxU8(m_x_main_stick_value, pad->stickX);
  GetSpinBoxU8(m_y_main_stick_value, pad->stickY);

  GetSpinBoxU8(m_x_c_stick_value, pad->substickX);
  GetSpinBoxU8(m_y_c_stick_value, pad->substickY);
}
