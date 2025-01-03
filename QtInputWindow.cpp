#include"QtInputWindow.h"

QtInputWindow::QtInputWindow(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setFixedSize(250, 200);
	//设置颜色
	QPalette palette_qmw;
	palette_qmw.setBrush(this->backgroundRole(), Qt::white);
	this->setPalette(palette_qmw);

	//设置图标
	this->setWindowTitle("学子·文件备份");
	QIcon icon("./加密文件夹.png");
	this->setWindowIcon(icon);


	// 创建分组对象
	Group = new QGroupBox(this);
	Group->setTitle("            操作成功           ");


	Group->setGeometry(50, 50, 150, 100);
	Group->setStyleSheet("QGroupBox { background-color: #7DDDFD;  font-weight: bold; color: black; }"); // 更改背景颜色

	notice = new QLabel(this);
	//notice->setStyleSheet("QLabel {margin: 40px; } ");
	notice->setText("");


	// 创建close按钮
	QPushButton* close_b = new QPushButton(this);
	close_b->setText("确定");

	Layout = new QVBoxLayout(Group);
	Layout->addWidget(notice);
	Layout->addWidget(close_b);

	connect(close_b, &QPushButton::released, this, &QtInputWindow::next);

	// 创建key_txt文本框
	key_txt = new QLineEdit(this);
	key_txt->setGeometry(15, 250, 250, 20);
	key_txt->setEchoMode(QLineEdit::Password); // 密码用圆点代替

	// 创建aes密钥格式分组对象
	aes_mode_group = new QGroupBox(this);
	aes_mode_group->setTitle("选择解密模式");
	aes_mode_group->setGeometry(10, 280 + c + c + c, 250, 60);
	aes_mode_group->setStyleSheet("border: 0");

	// 创建三个单选框对象
	aes128_b = new QRadioButton("AES128", aes_mode_group);
	aes192_b = new QRadioButton("AES192", aes_mode_group);
	aes256_b = new QRadioButton("AES256", aes_mode_group);

	// aes128选项为默认值
	aes128_b->setChecked(true);

	// 获取aes_b值
	connect(aes128_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			aes_flag2 = 1;
		});
	connect(aes192_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			aes_flag2 = 2;
		});
	connect(aes256_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			aes_flag2 = 3;
		});

	// 创建布局: 垂直布局, 指定其父对象为分组控件
	Layout2 = new QHBoxLayout(aes_mode_group);
	Layout2->addWidget(aes128_b);
	Layout2->addWidget(aes192_b);
	Layout2->addWidget(aes256_b);

	// 将布局控件添加到组中
	aes_mode_group->setLayout(Layout2);
}

void QtInputWindow::next()
{
	this->close();
	
}