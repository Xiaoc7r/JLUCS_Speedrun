#include <QApplication>
#include <QPushButton>

int main( int argc, char * argv[])
{
QApplication app(argc, argv);
QPushButton hello(" Hello world！");
hello.resize(300 , 300 );
hello.show();
return app.exec();
}

