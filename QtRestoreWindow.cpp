#include "QtRestoreWindow.h"

std::string target_folder2;
std::vector <std::string> cho2;
int file_format = 0;
int aes_flag2 = 1;
int  c = 40;
QtRestoreWindow::QtRestoreWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setFixedSize(600, 500);

	//设置图标
	this->setWindowTitle("学子·文件备份");
	QIcon icon("./加密文件夹.png");
	this->setWindowIcon(icon);

	QPalette palette_qmw;
	palette_qmw.setBrush(this->backgroundRole(), QColor(240, 255, 255));
	this->setPalette(palette_qmw);


	// 文件
	file_b = new QPushButton(this);
	file_b->setText("选择恢复文件");
	file_b->move(125, 180+c+c);

	check_b = new QPushButton(this);
	check_b->setText("进行CRC校验");
	check_b->move(375, 180+c+c);

	// 文件夹
	folder_b = new QPushButton(this);
	folder_b->setText("选择恢复路径");
	folder_b->move(250, 40+c);


	// 执行
	restore_b = new QPushButton(this);
	restore_b->setText("恢复");
	restore_b->move(320, 460);

	// back
	back_b = new QPushButton(this);
	back_b->setText("返回");
	back_b->move(410, 460);

	// close
	close_b = new QPushButton(this);
	close_b->setText("关闭");
	close_b->move(500, 460);

	// 文件
	label2 = new QLabel(this);
	QFont font = label2->font(); // 获取当前字体
	font.setPointSize(10);      // 更改字体大小
	label2->setFont(font);       // 应用新的字体
	label2->setStyleSheet("background-color:#E6E6FA; color: black; border: none; font-weight: bold;");
	label2->setAlignment(Qt::AlignLeft);
	label2->setVisible(true);
	label2->setGeometry(10, 70+c+c, 580, 100);
	label2->setText("我要恢复的文件:当前未选择");
	label2->adjustSize();

	//
	scrolllabel2 = new QScrollArea(this);
	scrolllabel2->setGeometry(10, 70+c+c, 580, 100);
	scrolllabel2->setWidget(label2);
	scrolllabel2->setStyleSheet("background-color:  #E6E6FA ;}");

	// 文件夹
	label = new QLabel(this);
	label->setFont(font);
	label->setStyleSheet("background-color: #FFF0F5 ;color: black; border: none; font-weight: bold;");
	label->setAlignment(Qt::AlignLeft);
	label->setVisible(true);
	label->setGeometry(10, 10+c, 580, 20);
	label->setFrameShape(QFrame::Box);
	label->setText("我要恢复的目标路径:当前未选择");

	// file_b: choose_files
	connect(file_b, &QPushButton::released, this, &QtRestoreWindow::choose_files);
	// folder_b: choose_folder
	connect(folder_b, &QPushButton::released, this, &QtRestoreWindow::choose_folder);

	connect(check_b, &QPushButton::released, this, &QtRestoreWindow::crc);
	// restore_b: 执行
	connect(restore_b, &QPushButton::released, this, &QtRestoreWindow::excute);
	// back_b: 返回
	connect(back_b, &QPushButton::released, this, &QtRestoreWindow::back);
	// close_b: 关闭界面
	connect(close_b, &QPushButton::pressed, this, &QtRestoreWindow::close);

}

void QtRestoreWindow::choose_folder() {  //文件夹

	target_folder2 = CSelectFolderDlg::Show();
	//b2->setText(QString::fromStdString(target_folder2));
	std::string label_txt = target_folder2;
	label->setText(QString::fromLocal8Bit(label_txt));

}

void QtRestoreWindow::choose_files() {  //文件
	cho2 = chooseFiles();
	std::string file_name;
	for (auto path : cho2) {
		file_name += path + "\n";
	}
	std::string label2_txt =  file_name;
	label2->setText(QString::fromLocal8Bit(label2_txt));
	label2->adjustSize();
}

void QtRestoreWindow::excute() {
	const char* key="";
	int flag_aes=0;
	if (target_folder2 != "" && cho2.size() != 0)
	{
		
		//	qDebug() << target_folder2 << "\n";
		for (auto i : cho2)
		{
			if (i.substr(i.find_last_of('.') + 1) == "aes")
			{
				flag_aes = 1;
				break;
			}
		}
		if (flag_aes == 1)
		{
			QtInputWindow* dia = new QtInputWindow(this);
			if (dia->exec() == QDialog::Accepted) {
				// 用户点击了“确定”
				QString password = dia->getPWD(); // 获取密码
				aes_flag2 = dia->getMode(); 
				std::string ts = password.toStdString();
				key = ts.c_str();
				my_restore(target_folder2, cho2, key, aes_flag2);
				this->back();
				QtMsgWindow* pic = new QtMsgWindow();
				QString t_s = "恢复成功!";
				pic->setMessage(t_s);
				pic->show();
				
			}
			else {
				QtMsgWindow* pic = new QtMsgWindow();
				QString t_s = "恢复已中断";
				pic->setMessage(t_s);
				pic->show();
			}
		}
		else {
			my_restore(target_folder2, cho2, key, aes_flag2);
			this->back();
			QtMsgWindow* pic = new QtMsgWindow();
			QString t_s = "恢复成功!";
			pic->setMessage(t_s);
			pic->show();
		}
		
		
	}
	else if (cho2.size() == 0) {
		QtMsgWindow* pic = new QtMsgWindow();
		QString t_s = "未选择文件";
		pic->setMessage(t_s);
		pic->show();
	}
	else {
		QtMsgWindow* pic = new QtMsgWindow();
		QString t_s = "未选择路径";
		pic->setMessage(t_s);
		pic->show();
	}
}

void QtRestoreWindow::back() {
	this->close();
	QtMainWindow* pic = new QtMainWindow();
	pic->show();
}

void QtRestoreWindow::crc() {
	if (cho2.size() != 0)
	{
		for (auto i : cho2)
		{
			std::string t1 = i + ".txt";
			uint32_t crc_value_r = read_from_txt(t1);
			if (crc_value_r == 941871365)
			{
				QtMsgWindow* pic = new QtMsgWindow();
				QString t_s = "校验文件异常";
				pic->setMessage(t_s);
				pic->show();
			}
			else {
				CRC32 crc_calculator;
				uint32_t crc_value = crc_calculator.compute(i);
				if (crc_value_r == crc_value)
				{
					QtMsgWindow* pic = new QtMsgWindow();
					QString t_s = "文件无误!";
					pic->setMessage(t_s);
					pic->setNotice(QString::fromLocal8Bit(get_file_name(i)));
					pic->show();
				}
				else {
					QtMsgWindow* pic = new QtMsgWindow();
					QString t_s = "文件异常!";
					pic->setMessage(t_s);
					pic->setNotice(QString::fromLocal8Bit(get_file_name(i)));
					pic->show();
				}
			}
		}
	}
	else  {
		QtMsgWindow* pic = new QtMsgWindow();
		QString t_s = "未选择文件";
		pic->setMessage(t_s);
		pic->show();
	}

		
	
}

QtRestoreWindow::~QtRestoreWindow()
{}
