#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MS_VS_GAS.h"
#include "GraphWindow.h"
#include "ArrayWindow.h"
#include "MatrixWindow.h"

class MS_VS_GAS : public QMainWindow
{
    Q_OBJECT

public:
    MS_VS_GAS(QWidget *parent = nullptr);
    ~MS_VS_GAS();

private:
    Ui::MS_VS_GASClass ui;
    GraphWindow* graphWindow;
    ArrayWindow* arrayWindow;
    MatrixWindow* matrixWindow;

private slots:
    void openGraphWindow();
    void openArrayWindow();
    void openMatrixWindow();

    void openDialogAbout();
};
