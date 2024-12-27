#pragma once

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include "ui_QtMainWindow.h"
#include <QString> 


class QtMsgWindow : public QWidget
{
	Q_OBJECT

public:
	QtMsgWindow(QWidget* parent = nullptr);
	~QtMsgWindow();
	//void next();
	void close();

private:
	Ui::QtMainWindowClass ui;
	QLabel* title; // Ö÷Ìâ
	QGroupBox* Group;

	QPushButton* close_b;
};