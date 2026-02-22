#include <QtWidgets>
#include "dialog.h"

Dialog::Dialog()
{
    createMenu();
    createHorizontalGroupBox();
    createGridGroupBox();
    createFormGroupBox();

    bigEditor = new QTextEdit;
    bigEditor->setPlainText(tr("This widget takes up all the remaining space "
                               "in the top-level layout."));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(horizontalGroupBox);
    mainLayout->addWidget(gridGroupBox);
    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(bigEditor);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Basic Layouts"));
}

void Dialog::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    exitAction = fileMenu->addAction(tr("E&xit"));
    menuBar->addMenu(fileMenu);

    connect(exitAction, SIGNAL(triggered()), this, SLOT(accept()));
}

void Dialog::createHorizontalGroupBox()
{
    horizontalGroupBox = new QGroupBox(tr("Horizontal layout"));
    QHBoxLayout *layout = new QHBoxLayout;

    for (int i = 0; i < NumButtons; ++i) {
        buttons[i] = new QPushButton(tr("Button %1").arg(i + 1));
	layout->addWidget(buttons[i]);
    }
    horizontalGroupBox->setLayout(layout);
}

void Dialog::createGridGroupBox()
{
    gridGroupBox = new QGroupBox(tr("Grid layout"));
    QGridLayout *layout = new QGridLayout;

    for (int i = 0; i < NumGridRows; ++i) {
	labels[i] = new QLabel(tr("Line %1:").arg(i + 1));
	lineEdits[i] = new QLineEdit;
	layout->addWidget(labels[i], i + 1, 0);
	layout->addWidget(lineEdits[i], i + 1, 1);
    }

    smallEditor = new QTextEdit;
    smallEditor->setPlainText(tr("This widget takes up about two thirds of the "
                                 "grid layout."));
    layout->addWidget(smallEditor, 0, 2, 4, 1);

    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 20);
    gridGroupBox->setLayout(layout);
}

void Dialog::createFormGroupBox()
{
    formGroupBox = new QGroupBox(tr("Form layout"));
    QFormLayout *layout = new QFormLayout;
    layout->addRow(new QLabel(tr("Line 1:")), new QLineEdit);
    layout->addRow(new QLabel(tr("Line 2, long text:")), new QComboBox);
    layout->addRow(new QLabel(tr("Line 3:")), new QSpinBox);
    formGroupBox->setLayout(layout);
}
