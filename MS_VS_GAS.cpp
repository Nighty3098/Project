#include "MS_VS_GAS.h"
#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>

MS_VS_GAS::MS_VS_GAS(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.openFirstTask, &QPushButton::clicked, this, [this]() { openGraphWindow(); });
    connect(ui.openSecondTask, &QPushButton::clicked, this, [this]() { openArrayWindow(); });
    connect(ui.openThirdTask, &QPushButton::clicked, this, [this]() { openMatrixWindow(); });
    connect(ui.aboutApp, &QPushButton::clicked, this, [this]() { openDialogAbout(); });
}

MS_VS_GAS::~MS_VS_GAS()
{}

void MS_VS_GAS::openGraphWindow() {
    graphWindow = new GraphWindow(this);

    graphWindow->show();
}

void MS_VS_GAS::openArrayWindow() {
    arrayWindow = new ArrayWindow(this);

    arrayWindow->show();
}

void MS_VS_GAS::openMatrixWindow() {
    matrixWindow = new MatrixWindow(this);

    matrixWindow->show();
}

void MS_VS_GAS::openDialogAbout() {
    QMessageBox msgBox;
       
    msgBox.setStyleSheet("QPushButton { background-color: transparent; color: #fff; }");
    msgBox.setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    
    msgBox.setFont(QFont("Arial 13"));
    msgBox.setText(QString::fromLocal8Bit("о приложении"));

    msgBox.setWindowTitle("About");

    msgBox.setIcon(QMessageBox::Information);

    msgBox.exec();
}