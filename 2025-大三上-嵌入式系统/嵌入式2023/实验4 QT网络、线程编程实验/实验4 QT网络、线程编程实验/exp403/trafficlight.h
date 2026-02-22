#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H
#include <QWidget>
#include <QImage>
#include <QColor>
#include <QPoint>
class ShowArea:public QWidget
{
  Q_OBJECT
 public:
   ShowArea(QWidget *parent=0);
 private:
   int nStep;
   int nTime;
   QImage image;
   void resizeImage(QImage *image,const QSize &newSize);
public slots: 
   void go();
 protected:
   void paintEvent(QPaintEvent *event);

};


#endif 
