#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE
class Button;

class Calculator : public QDialog
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = 0);

private slots:
    void d0Clicked();
    void d1Clicked();
    void d2Clicked();
    void d3Clicked();
    void d4Clicked();
    void d5Clicked();
    void d6Clicked();
    void d7Clicked();
    void d8Clicked();
    void d9Clicked();


    void addClicked();      // 加+
    void mulClicked();      // 乘*
    void subClicked();      // 减-
    void divClicked();     // 除/ 
    void equalClicked();                 // 运算=
    void pointClicked();                 // 小数点 .


private:
    Button *createButton(const QString &text, const QColor &color);
    double nFirst,nSecond;
    double nResult;
    int  index;     //index=0 +     1 -   2 *   3 /
    int inputindex;  //0:no  1:strFirst 2:strSecond
    QString strFirst,strSecond;

    QLineEdit *display;

    Button *d0Button,*d1Button,*d2Button,*d3Button;
    Button *d4Button,*d5Button,*d6Button,*d7Button;
    Button *d8Button,*d9Button;
    Button *addButton;
    Button *subButton;
    Button *timesButton;
    Button *divButton;
    Button *pointButton;
    Button *equalButton;
};

#endif


