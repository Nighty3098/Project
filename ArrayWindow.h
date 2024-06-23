#pragma once

#include <QMainWindow>
#include "ui_arrayWindow.h"

class ArrayWindow : public QMainWindow
{
	Q_OBJECT

public:
	ArrayWindow(QMainWindow* parent);
	~ArrayWindow();

private:
	Ui::ArrayWindow ui;

private slots:
	void setArrayData();
	void openDatafile();
	void getElementsMultiples();
	void getSummElements();
	bool checkInput();
	void insertionSort();
	void quickSort();
	void sequentialSearch();
    void binarySearch();
	void quickSortImpl(QVector<int>& array, int left, int right);
};
