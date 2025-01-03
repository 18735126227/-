#include "QtMainWindow.h"

int RadioButton_flag = 0;

QtMainWindow::QtMainWindow(QWidget *parent)
	: QWidget(parent)
{

	ui.setupUi(this);
	this->setFixedSize(600, 600);
	//设置颜色
	QPalette palette_qmw;
	palette_qmw.setBrush(this->backgroundRole(), QColor(240, 255, 255));
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
	
	RadioButton_flag = 0;

	// 创建分组对象
	Group = new QGroupBox(this);
	Group->setTitle("                      请选择你要进行的操作                      ");


	Group->setGeometry(140, 100, 300, 200);
	Group->setStyleSheet("QGroupBox { background-color: #7DDDFD;  font-weight: bold; color: black; }"); // 更改背景颜色


	
	// 创建两个单选框对象
	backup_b = new QRadioButton("我要备份", Group);
	backup_b->setStyleSheet("QRadioButton {margin: 100px; } ");
	

	restore_b = new QRadioButton("我要恢复", Group);
	restore_b->setStyleSheet("QRadioButton {margin: 100px; } ");

	// 备份选项为默认值
	backup_b->setChecked(true);

	// 获取RadioButton值
	connect(backup_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			RadioButton_flag = 0;
		else if (isChecked == false)
			RadioButton_flag = 1;
		});

	// 创建布局: 垂直布局, 指定其父对象为分组控件
	Layout = new QVBoxLayout(Group);
	Layout->addWidget(backup_b);
	Layout->addWidget(restore_b);
	

	// 将布局控件添加到组中
	Group->setLayout(Layout);

	// 创建next按钮
	QPushButton* next_b = new QPushButton(this);
	next_b->setText("确定");
	next_b->move(400, 565);
	connect(next_b, &QPushButton::released, this, &QtMainWindow::next);

	// 创建close按钮
	QPushButton* close_b = new QPushButton(this);
	close_b->setText("关闭");
	close_b->move(500, 565);
	connect(close_b, &QPushButton::released, this, &QtMainWindow::close);
}

QtMainWindow::~QtMainWindow()
{}

void QtMainWindow::next()
{
	if (RadioButton_flag == 0) {
		qDebug() << "backup";
		this->close();
		QtBackupWindow* pic = new QtBackupWindow();
		pic->show();
	}

	else if (RadioButton_flag == 1) {
		this->close();
		QtRestoreWindow* pic = new QtRestoreWindow();
		pic->show();
		qDebug() << "restore";
	}

	else qDebug() << "error";
}

