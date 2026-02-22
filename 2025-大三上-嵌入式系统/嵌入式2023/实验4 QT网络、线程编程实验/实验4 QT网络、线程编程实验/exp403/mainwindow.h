#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class ShowArea;
class MainWindow:public QMainWindow
{
  Q_OBJECT
 public:
   MainWindow();
 private:
   ShowArea *showArea;
};



#endif
