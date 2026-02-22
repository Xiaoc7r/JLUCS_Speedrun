#include <QtWidgets>

#include "dialog.h"
#include "wigglywidget.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    WigglyWidget *wigglyWidget = new WigglyWidget;
    QLineEdit *lineEdit = new QLineEdit;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(wigglyWidget);
    layout->addWidget(lineEdit);
    setLayout(layout);

    connect(lineEdit, SIGNAL(textChanged(QString)),
            wigglyWidget, SLOT(setText(QString)));

    lineEdit->setText(tr("Hello world!"));

    setWindowTitle(tr("Wiggly"));
    resize(360, 145);
}
