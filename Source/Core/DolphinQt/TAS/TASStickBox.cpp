// Copyright 2023 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinQt/TAS/TASStickBox.h"

TASStickBox::TASStickBox(QWidget* parent) : QGroupBox(parent), m_stick_widget(nullptr)
{
}

TASStickBox::TASStickBox(const QString& title, QWidget* parent)
    : QGroupBox(title, parent), m_stick_widget(nullptr)
{
}

StickWidget* TASStickBox::GetStickWidget() const
{
  return m_stick_widget;
}

void TASStickBox::SetStickWidget(StickWidget* widget)
{
  m_stick_widget = widget;
}
