#pragma once
//假的头文件，从QtRestoreWindow.h里ctrl点击QtMsgWindow的include找到真的头文件
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
	void next();
	void setMessage(QString msg);
	void setNotice(QString msg);
	//void close();
	QLabel* notice;

private:
	Ui::QtMainWindowClass ui;
	QLabel* title; // 主题
	QGroupBox* Group;
	QPushButton* close_b;

	QVBoxLayout* Layout;
};
