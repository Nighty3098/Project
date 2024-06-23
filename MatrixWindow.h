#pragma once

#include <QMainWindow>
#include "ui_matrixWindow.h"

class MatrixWindow  : public QMainWindow
{
	Q_OBJECT

public:
	MatrixWindow(QMainWindow *parent);
	~MatrixWindow();

private:
	Ui::MatrixWindow ui;

private slots:
	void negativeNumbersProduct();
	void summElements();
	void createArray();
	bool checkMatrix();
	void setRandomData();
	void createMatrix();
	void saveData();
	void clearMatrix();
};
