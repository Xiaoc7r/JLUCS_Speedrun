#include <QtWidgets>
#include <math.h>
#include "button.h"
#include "calculator.h"

Calculator::Calculator(QWidget *parent)
    : QDialog(parent)
{
    nFirst=0.0;
    nSecond=0.0;
    nResult=0.0;
    index=0;

    inputindex=0;  //0:strFirst 1:strSecond
    strFirst="";
    strSecond="";



    display = new QLineEdit("0");
    display->setReadOnly(false);   
    display->setAlignment(Qt::AlignRight); 
    display->setMaxLength(15);
 //   display->setValidator(new QIntValidator(1,65536));
    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);


    QColor digitColor(150, 205, 205);
    QColor backspaceColor(225, 185, 135);
    QColor memoryColor(100, 155, 155);
    QColor operatorColor(155, 175, 195);




    d0Button= createButton(tr("0"), digitColor);
    d1Button= createButton(tr("1"), digitColor);
    d2Button= createButton(tr("2"), digitColor);
    d3Button= createButton(tr("3"), digitColor);
    d4Button= createButton(tr("4"), digitColor);
    d5Button= createButton(tr("5"), digitColor);
    d6Button= createButton(tr("6"), digitColor);
    d7Button= createButton(tr("7"), digitColor);
    d8Button= createButton(tr("8"), digitColor);
    d9Button= createButton(tr("9"), digitColor);

    addButton= createButton(tr("+"),operatorColor);
    subButton= createButton(tr("-"),operatorColor);
    timesButton= createButton(tr("*"),operatorColor);
    divButton= createButton(tr("/"),operatorColor);

    equalButton= createButton(tr("="),operatorColor);
    pointButton= createButton(tr("."),operatorColor);

    connect(d0Button,SIGNAL(clicked()),this,SLOT(d0Clicked()));
    connect(d1Button,SIGNAL(clicked()),this,SLOT(d1Clicked()));
    connect(d2Button,SIGNAL(clicked()),this,SLOT(d2Clicked()));
    connect(d3Button,SIGNAL(clicked()),this,SLOT(d3Clicked()));
    connect(d4Button,SIGNAL(clicked()),this,SLOT(d4Clicked()));
    connect(d5Button,SIGNAL(clicked()),this,SLOT(d5Clicked()));
    connect(d6Button,SIGNAL(clicked()),this,SLOT(d6Clicked()));
    connect(d7Button,SIGNAL(clicked()),this,SLOT(d7Clicked()));
    connect(d8Button,SIGNAL(clicked()),this,SLOT(d8Clicked()));
    connect(d9Button,SIGNAL(clicked()),this,SLOT(d9Clicked()));

    connect(pointButton,SIGNAL(clicked()),this,SLOT(pointClicked()));
    connect(equalButton,SIGNAL(clicked()),this,SLOT(equalClicked()));
    connect(addButton,SIGNAL(clicked()),this,SLOT(addClicked()));
    connect(subButton,SIGNAL(clicked()),this,SLOT(subClicked()));
    connect(timesButton,SIGNAL(clicked()),this,SLOT(mulClicked()));
    connect(divButton,SIGNAL(clicked()),this,SLOT(divClicked()));


   QGridLayout *mainLayout = new QGridLayout;
   mainLayout->setSizeConstraint(QLayout::SetFixedSize);
  
   mainLayout->addWidget(display, 0, 0, 1,4);
  
   mainLayout->addWidget(d0Button, 4, 0);
   mainLayout->addWidget(d1Button, 3, 0);
   mainLayout->addWidget(d2Button, 3, 1);
   mainLayout->addWidget(d3Button, 3, 2);
   mainLayout->addWidget(d4Button, 2, 0);
   mainLayout->addWidget(d5Button, 2, 1);
   mainLayout->addWidget(d6Button, 2, 2);
   mainLayout->addWidget(d7Button, 1, 0);
   mainLayout->addWidget(d8Button, 1, 1);
   mainLayout->addWidget(d9Button, 1, 2);



    mainLayout->addWidget(pointButton, 4, 1);
    mainLayout->addWidget(equalButton, 4, 2);
    mainLayout->addWidget(divButton, 4, 3);
    mainLayout->addWidget(timesButton, 3, 3);
    mainLayout->addWidget(subButton, 2, 3);
    mainLayout->addWidget(addButton, 1, 3);

    setLayout(mainLayout);
    setWindowTitle(tr("Calculator"));
}


Button *Calculator::createButton(const QString &text, const QColor &color)
{
    Button *button = new Button(text, color);
    return button;
}

//**************************
void Calculator::d0Clicked()
{

 if(inputindex==0)  //0:strFirst 1:strSecond
   {
	   strFirst=strFirst+tr("0");
         int digitValue = strFirst.toInt();
         if (digitValue == 0)  {display->setText("0"); strFirst="";}
	 else    display->setText(strFirst);
           qDebug()<<strFirst<<endl;


   } else {
           strSecond=strSecond+"0";
           int digitValue = strSecond.toInt();
           if (digitValue == 0)  {display->setText("0"); strSecond="";}
           else    display->setText(strSecond);
           qDebug()<<strSecond<<endl;
          }

	qDebug()<<"0"<<endl;
//	QMessageBox::information(this,tr("Input Digital"),tr("Input 0"));
}
void Calculator::d1Clicked()
{ 
	 if(inputindex==0)  //0:strFirst 1:strSecond
   {
           strFirst=strFirst+tr("1");
         int digitValue = strFirst.toInt();
         if (digitValue == 0)  {display->setText("0"); strFirst="";}
         else    display->setText(strFirst);
           qDebug()<<strFirst<<endl;


   } else {
           strSecond=strSecond+"1";
           int digitValue = strSecond.toInt();
           if (digitValue == 0)  {display->setText("0"); strSecond="";}
           else    display->setText(strSecond);
           qDebug()<<strSecond<<endl;
          }

         qDebug()<<"Digital 1"<<endl;
}
void Calculator::d2Clicked()
{
	 if(inputindex==0)  //0:strFirst 1:strSecond
   {
           strFirst=strFirst+tr("2");
         int digitValue = strFirst.toInt();
         if (digitValue == 0)  {display->setText("0"); strFirst="";}
         else    display->setText(strFirst);
           qDebug()<<strFirst<<endl;


   } else {
           strSecond=strSecond+"2";
           int digitValue = strSecond.toInt();
           if (digitValue == 0)  {display->setText("0"); strSecond="";}
           else    display->setText(strSecond);
           qDebug()<<strSecond<<endl;
          }

	 qDebug()<<"Digital 2"<<endl;
}
void Calculator::d3Clicked()
{
	 if(inputindex==0)  //0:strFirst 1:strSecond
   {
           strFirst=strFirst+tr("3");
         int digitValue = strFirst.toInt();
         if (digitValue == 0)  {display->setText("0"); strFirst="";}
         else    display->setText(strFirst);
           qDebug()<<strFirst<<endl;


   } else {
           strSecond=strSecond+"3";
           int digitValue = strSecond.toInt();
           if (digitValue == 0)  {display->setText("0"); strSecond="";}
           else    display->setText(strSecond);
           qDebug()<<strSecond<<endl;
          }

	 qDebug()<<"Digital 3"<<endl;
}
void Calculator::d4Clicked()
{
	 if(inputindex==0)  //0:strFirst 1:strSecond
   {
           strFirst=strFirst+tr("4");
         int digitValue = strFirst.toInt();
         if (digitValue == 0)  {display->setText("0"); strFirst="";}
         else    display->setText(strFirst);
           qDebug()<<strFirst<<endl;


   } else {
           strSecond=strSecond+"4";
           int digitValue = strSecond.toInt();
           if (digitValue == 0)  {display->setText("0"); strSecond="";}
           else    display->setText(strSecond);
           qDebug()<<strSecond<<endl;
          }
	 qDebug()<<"Digital 4"<<endl;
}
void Calculator::d5Clicked()
{
	 if(inputindex==0)  //0:strFirst 1:strSecond
   {
           strFirst=strFirst+tr("5");
         int digitValue = strFirst.toInt();
         if (digitValue == 0)  {display->setText("0"); strFirst="";}
         else    display->setText(strFirst);
           qDebug()<<strFirst<<endl;


   } else {
           strSecond=strSecond+"5";
           int digitValue = strSecond.toInt();
           if (digitValue == 0)  {display->setText("0"); strSecond="";}
           else    display->setText(strSecond);
           qDebug()<<strSecond<<endl;
          }

        qDebug()<<"Digital 5"<<endl;
}
void Calculator::d6Clicked()
{
	 if(inputindex==0)  //0:strFirst 1:strSecond
   {
           strFirst=strFirst+tr("6");
         int digitValue = strFirst.toInt();
         if (digitValue == 0)  {display->setText("0"); strFirst="";}
         else    display->setText(strFirst);
           qDebug()<<strFirst<<endl;


   } else {
           strSecond=strSecond+"6";
           int digitValue = strSecond.toInt();
           if (digitValue == 0)  {display->setText("0"); strSecond="";}
           else    display->setText(strSecond);
           qDebug()<<strSecond<<endl;
          }

	 qDebug()<<"Digital 6"<<endl;
}
void Calculator::d7Clicked()
{
	 if(inputindex==0)  //0:strFirst 1:strSecond
   {
           strFirst=strFirst+tr("7");
         int digitValue = strFirst.toInt();
         if (digitValue == 0)  {display->setText("0"); strFirst="";}
         else    display->setText(strFirst);
           qDebug()<<strFirst<<endl;


   } else {
           strSecond=strSecond+"7";
           int digitValue = strSecond.toInt();
           if (digitValue == 0)  {display->setText("0"); strSecond="";}
           else    display->setText(strSecond);
           qDebug()<<strSecond<<endl;
          }

	 qDebug()<<"Digital 7"<<endl;
}
void Calculator::d8Clicked()
{
	 if(inputindex==0)  //0:strFirst 1:strSecond
   {
           strFirst=strFirst+tr("8");
         int digitValue = strFirst.toInt();
         if (digitValue == 0)  {display->setText("0"); strFirst="";}
         else    display->setText(strFirst);
           qDebug()<<strFirst<<endl;


   } else {
           strSecond=strSecond+"8";
           int digitValue = strSecond.toInt();
           if (digitValue == 0)  {display->setText("0"); strSecond="";}
           else    display->setText(strSecond);
           qDebug()<<strSecond<<endl;
          }
	 qDebug()<<"Digital 8"<<endl;
}
void Calculator::d9Clicked()
{
	 if(inputindex==0)  //0:strFirst 1:strSecond
   {
           strFirst=strFirst+tr("9");
         int digitValue = strFirst.toInt();
         if (digitValue == 0)  {display->setText("0"); strFirst="";}
         else    display->setText(strFirst);
           qDebug()<<strFirst<<endl;


   } else {
           strSecond=strSecond+"9";
           int digitValue = strSecond.toInt();
           if (digitValue == 0)  {display->setText("0"); strSecond="";}
           else    display->setText(strSecond);
           qDebug()<<strSecond<<endl;
          }

	 qDebug()<<"Digital 9"<<endl;
}
void Calculator::addClicked()      // 加+
{
	 if (inputindex==0) inputindex=1;
	 index=1;
	 qDebug()<<"Digital add"<<endl;
}
void Calculator::mulClicked()      // 乘*
{         if (inputindex==0) inputindex=1;
         index=3;
	 qDebug()<<"Digital mul"<<endl;
}
void Calculator::subClicked()      // 减-
{        if (inputindex==0) inputindex=1;
         index=2;
	 qDebug()<<"Digital sub"<<endl;
}
void Calculator::divClicked()     // 除/ 
{
        if (inputindex==0) inputindex=1;
         index=4;
	 qDebug()<<"Digital div"<<endl;
}
void Calculator::equalClicked()                 // 运算=
{
	 nFirst = strFirst.toInt();
	 nSecond=strSecond.toInt();
	 switch(index)
	 {
		 case 1:
			 {
                           nResult=nFirst+nSecond;
			   display->setText(QString::number(nResult));
			 }
			 break;
                 case 2:
                         {
                           nResult=nFirst-nSecond;
                           display->setText(QString::number(nResult));
                         }
                         break;			 
                 case 3:
                         {
                           nResult=nFirst*nSecond;
                           display->setText(QString::number(nResult));
                         }
                         break;
                 case 4:
                         {
                           nResult=nFirst/nSecond;
                           display->setText(QString::number(nResult));
                         }
                         break;			 
	 }
	 index=0;
	 nFirst=0;
	 nSecond=0;
	 strFirst="";
	 strSecond="";
	 inputindex=0;
	 nResult=0;
	 qDebug()<<"Digital ="<<endl;
}
void Calculator::pointClicked()                 // 小数点 .
{
	 qDebug()<<"Digital point"<<endl;
}



