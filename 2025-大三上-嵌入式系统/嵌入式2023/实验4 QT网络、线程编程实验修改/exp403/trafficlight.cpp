#include <QtGui>
#include "trafficlight.h"


ShowArea::ShowArea(QWidget *parent)
  :QWidget(parent)
{
  QImage loadImage;
  setAttribute(Qt::WA_StaticContents);
  nStep=0;
  nTime=5;
  if (loadImage.load("main.jpg"))
   {
    image=loadImage;
    QSize newSize=loadImage.size().expandedTo(size());
    resizeImage(&loadImage,newSize);
    update();
   }
   QTimer *timer=new QTimer(this);
   connect(timer,SIGNAL(timeout()),SLOT(go()));
   timer->start(1000); 
}


void ShowArea::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawImage(QPoint(0,0),image);
 switch (nStep) 
 {
  case 0:
     {
//led1
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
      painter.drawEllipse(165,45,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(165,75,20,20);
      painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
      painter.drawEllipse(165,105,20,20);
//led2
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
      painter.drawEllipse(257,33,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(257,73,20,20);
      painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
      painter.drawEllipse(257,103,20,20);
//led3
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
      painter.drawEllipse(558,98,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(558,128,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(558,158,20,20);
//led4
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
      painter.drawEllipse(652,115,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(652,145,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(652,175,20,20);
     }
     break;
  case 1:
	  {
//led1
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
      painter.drawEllipse(165,45,20,20);
      painter.setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
      painter.drawEllipse(165,75,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(165,105,20,20);
//led2
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
      painter.drawEllipse(257,33,20,20);
      painter.setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
      painter.drawEllipse(257,73,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(257,103,20,20);
//led3
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
      painter.drawEllipse(558,98,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(558,128,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(558,158,20,20);
//led4
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
      painter.drawEllipse(652,115,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(652,145,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(652,175,20,20);

	  }
	  break;
  case 2:
	  {
//led1
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
      painter.drawEllipse(165,45,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(165,75,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(165,105,20,20);
//led2
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
      painter.drawEllipse(257,33,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(257,73,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(257,103,20,20);
//led3
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
      painter.drawEllipse(558,98,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(558,128,20,20);
      painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
      painter.drawEllipse(558,158,20,20);
//led4
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
      painter.drawEllipse(652,115,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(652,145,20,20);
      painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
      painter.drawEllipse(652,175,20,20);

	  }
     break;
  case 3:
	  {
//led1
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
      painter.drawEllipse(165,45,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(165,75,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(165,105,20,20);
//led2
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
      painter.drawEllipse(257,33,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(257,73,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(257,103,20,20);
//led3
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
      painter.drawEllipse(558,98,20,20);
      painter.setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
      painter.drawEllipse(558,128,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(558,158,20,20);
//led4
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
      painter.drawEllipse(652,115,20,20);
      painter.setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
      painter.drawEllipse(652,145,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(652,175,20,20);

	  }
     break;
  case 4:
	  {
//led1
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
      painter.drawEllipse(165,45,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(165,75,20,20);
      painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
      painter.drawEllipse(165,105,20,20);
//led2
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
      painter.drawEllipse(257,33,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(257,73,20,20);
      painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
      painter.drawEllipse(257,103,20,20);
//led3
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
      painter.drawEllipse(558,98,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(558,128,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(558,158,20,20);
//led4
      painter.setPen(QPen(Qt::black,Qt::DashLine));
      painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
      painter.drawEllipse(652,115,20,20);
      painter.setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
      painter.drawEllipse(652,145,20,20);
      painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
      painter.drawEllipse(652,175,20,20);
	  }
     break;
 }


}


void  ShowArea::resizeImage(QImage *image,const QSize &newSize)
{
 if (image->size()==newSize) return;
 QImage newImage(newSize,QImage::Format_RGB32);
 newImage.fill(qRgb(255,255,255));
 QPainter painter(&newImage);
 painter.drawImage(QPoint(0,0),*image);
 *image=newImage; 
}

void ShowArea::go()
{
 switch(nStep)
 {
   case 0:
      {
        nTime--;
        if(nTime<=0)
         { nTime=5;
           nStep=1;
           update();
          }
      }
      break;
   case 1:
     {
       nTime--;
       if(nTime<=0)
         {
           nStep=2;
           nTime=5;
           update();   
         } 
     }
     break;
   case 2:
    {
      nTime--;
      if(nTime<=0)
       {
        nStep=3;
        nTime=5;
        update();  
       }  
    }
    break;
   case 3:
     {
       nTime--;
       if(nTime<=0)
          {
            nStep=4;
            nTime=5;
            update();
          }
     }
    break;
   case 4:
     {
       nTime--;
       if(nTime<=0)
        {
          nStep=1;
          nTime=5;
          update();
        }  
     }
    break; 
 } 
}
