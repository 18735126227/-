#include "QtMsgWindow.h"


QtMsgWindow::QtMsgWindow(QWidget* parent)
	: QWidget(parent)
{

	ui.setupUi(this);
	this->setFixedSize(250, 200);
	//设置颜色
	QPalette palette_qmw;
	palette_qmw.setBrush(this->backgroundRole(), Qt::white);
	this->setPalette(palette_qmw);

	//设置背景图片
	//QPixmap pixmap = QPixmap(":/qm/safe").scaled(this->size());
	//QPalette palette(this->palette());
	//palette.setBrush(QPalette::Background, QBrush(pixmap));
	//this->setPalette(palette);

	//设置图标
	this->setWindowTitle("学子·文件备份");
	QIcon icon("./加密文件夹.png");
	this->setWindowIcon(icon);


	// 创建分组对象
	Group = new QGroupBox(this);
	Group->setTitle("            操作成功           ");


	Group->setGeometry(50, 50, 150, 100);
	Group->setStyleSheet("QGroupBox { background-color: #7DDDFD;  font-weight: bold; color: black; }"); // 更改背景颜色


	// 创建close按钮
	QPushButton* close_b = new QPushButton(this);
	close_b->setText("确定");
	close_b->move(90, 90);
	connect(close_b, &QPushButton::released, this, &QtMsgWindow::close);
}

QtMsgWindow::~QtMsgWindow()
{}

void QtMsgWindow::close()
{
		this->close();
		qDebug() << "restore";
}

