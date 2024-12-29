#pragma once

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include "ui_QtMainWindow.h"
#include <QString> 
#include <qlineedit.h>
#include <QDialog>
#include <qmessagebox.h>
class QtInputWindow : public QDialog // 改为 QDialog
{
    Q_OBJECT

public:
    explicit QtInputWindow(QWidget* parent = nullptr);
    ~QtInputWindow();
    QString getPWD(); 
    int getMode();
    void next();// 获取密码的方法

private slots:
    void onCloseButtonClicked(); // 关闭按钮的槽函数

private:
    QLabel* notice;
    QLabel* title; // 主题
    QGroupBox* Group;
    QPushButton* close_b;
    QVBoxLayout* Layout;

    // aes密钥输入框
    QLineEdit* key_txt;

    // aes密钥格式
    QGroupBox* aes_mode_group;
    QRadioButton* aes128_b;
    QRadioButton* aes192_b;
    QRadioButton* aes256_b;
    QHBoxLayout* Layout2;
    int aes_flag3 = 1;
};
