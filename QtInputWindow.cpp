#include"QtInputWindow.h"


QtInputWindow::QtInputWindow(QWidget* parent)
	: QDialog(parent)
{
	this->setFixedSize(400, 280);
	//设置颜色
	QPalette palette_qmw;
	palette_qmw.setBrush(this->backgroundRole(), QColor(240, 255, 255));
	this->setPalette(palette_qmw);

	//设置图标
	this->setWindowTitle("学子·文件备份");
	QIcon icon("./加密文件夹.png");
	this->setWindowIcon(icon);


	// 创建分组对象
	Group = new QGroupBox(this);
	Group->setTitle("      检测到加密文件，请输入密码：     ");
	Group->setAlignment(Qt::AlignCenter);


	Group->setGeometry(60, 60, 280, 160);
	Group->setStyleSheet("QGroupBox { background-color: #7DDDFD;  font-weight: bold; color: black; }"); // 更改背景颜色


	// 创建key_txt文本框
	key_txt = new QLineEdit(this);
	key_txt->setGeometry(80, 90, 250, 20);
	key_txt->setEchoMode(QLineEdit::Password); // 密码用圆点代替

	// 创建aes密钥格式分组对象
	aes_mode_group = new QGroupBox(Group);
	aes_mode_group->setTitle("选择解密模式");
	aes_mode_group->setGeometry(10, 60, 250, 60);
	aes_mode_group->setStyleSheet("border: 0");

	// 创建close按钮
	QPushButton* close_b = new QPushButton(Group);
	close_b->setText("确定");
	
	



	

	

	// 创建三个单选框对象
	aes128_b = new QRadioButton("AES128", aes_mode_group);
	aes192_b = new QRadioButton("AES192", aes_mode_group);
	aes256_b = new QRadioButton("AES256", aes_mode_group);

	// aes128选项为默认值
	aes128_b->setChecked(true);

	// 获取aes_b值
	connect(aes128_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			aes_flag3 = 1;
		});
	connect(aes192_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			aes_flag3 = 2;
		});
	connect(aes256_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			aes_flag3 = 3;
		});
	connect(close_b, &QPushButton::clicked, this, &QtInputWindow::onCloseButtonClicked);
	// 创建布局: 垂直布局, 指定其父对象为分组控件
	Layout2 = new QHBoxLayout(aes_mode_group);
	Layout2->addWidget(aes128_b);
	Layout2->addWidget(aes192_b);
	Layout2->addWidget(aes256_b);

	// 将布局控件添加到组中
	aes_mode_group->setLayout(Layout2);

	Layout = new QVBoxLayout(Group);
	Layout->addLayout(Layout2);
	Layout->addSpacing(60);
	Layout->addWidget(close_b);
	

	//connect(close_b, &QPushButton::released, this, &QtInputWindow::next);
}

void QtInputWindow::next()
{
	this->close();
	
}

QtInputWindow::~QtInputWindow() {
	// 析构函数逻辑，如果有的话
}

QString QtInputWindow::getPWD()
{
	return key_txt->text();
}

int QtInputWindow::getMode()
{
	return aes_flag3;
}

void QtInputWindow::onCloseButtonClicked() {
	// 显示确认对话框，询问用户是否真的要关闭
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "确认",
		"所有加密文件都将使用此密码，是否继续?",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		// 如果用户选择了“是”，则关闭窗口
		this->accept();  // 或者使用 this->close();
	}
	else {
		this->close();
		
	}
}