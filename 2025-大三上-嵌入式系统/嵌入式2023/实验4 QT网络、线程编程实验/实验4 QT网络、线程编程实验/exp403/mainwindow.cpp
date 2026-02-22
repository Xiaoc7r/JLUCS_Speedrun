#include <QtGui>
#include "mainwindow.h"
#include  "trafficlight.h"

MainWindow::MainWindow()
{
  showArea=new ShowArea;
  setCentralWidget(showArea);

  setWindowTitle(tr("Trafficlight Control System!!!"));
  resize(800,600);
 
}
