// Copyright 2023 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <QGroupBox>

class StickWidget;

class TASStickBox : public QGroupBox
{
public:
  explicit TASStickBox(QWidget* parent = nullptr);
  explicit TASStickBox(const QString& title, QWidget* parent = nullptr);

  StickWidget* GetStickWidget() const;
  void SetStickWidget(StickWidget* widget);

protected:
  StickWidget* m_stick_widget;
};
