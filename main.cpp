#include "QtMainWindow.h"
#include <QtWidgets/QApplication>
//#include <QTextCodec>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   a.setFont(QFont("Microsoft yahei", 9));
   //…Ë÷√÷–Œƒ±‡¬Î

   //QTextCode* codec = QTextCodec::codecForName("utf-8");
   //QTextCode::setCodecForLocale(codec);


    QtMainWindow w;
    w.show();
    return a.exec();
}
