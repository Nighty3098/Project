#include "MatrixWindow.h"
#include <QTableWidgetItem>
#include <QRandomGenerator>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>

MatrixWindow::MatrixWindow(QMainWindow*parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.createMatrixTable, &QPushButton::clicked, this, [this]() {
		QThread* matrixThread = new QThread;
		QObject::connect(matrixThread, &QThread::started, this, [this]() {
			createMatrix(); 
		}); 
		matrixThread->start();
	});
	connect(ui.setMatrixData, &QPushButton::clicked, this, [this]() { 
		QThread* setDataThread = new QThread;
		QObject::connect(setDataThread, &QThread::started, this, [this]() {
			setRandomData();
		});
		setDataThread->start();
	});
	connect(ui.startProcessBtn, &QPushButton::clicked, this, [this]() {
		QThread* processThread = new QThread;
		QObject::connect(processThread, &QThread::started, this, [this]() {
			if (checkMatrix()) {
				negativeNumbersProduct(); summElements(); createArray();
			}
			else {
				qDebug() << "Err";
			}});
			processThread->start();
		});
	connect(ui.saveDataToFileBtn, &QPushButton::clicked, this, [this]() { saveData(); });
	connect(ui.clearDataButton, &QPushButton::clicked, this, [this]() { clearMatrix(); });
}

MatrixWindow::~MatrixWindow()
{}

void MatrixWindow::negativeNumbersProduct() {
	int product = 1;

	for (int row = 0; row < ui.matrixTable->rowCount(); ++row) {
		for (int column = 0; column < ui.matrixTable->columnCount(); ++column) {
			QTableWidgetItem* item = ui.matrixTable->item(row, column);
			if (item != nullptr) {
				int value = item->text().toInt();
				if (value < 0 && value % 2 == 0) {
					product *= value;
				}
			}
		}
	}

	qDebug() << product;

    ui.productOfNegativeNumbers->setText(QString("Произведение отрицательных чётных чисел: %1").arg(product));
}

void MatrixWindow::summElements() {
	int sum = 0;

	for (int i = 0; i < ui.matrixTable->rowCount(); ++i) {
		QTableWidgetItem* item = ui.matrixTable->item(i, i);
		if (item != nullptr) {
			int value = item->text().toInt();
			sum += value;
		}
	}
	qDebug() << sum;

    ui.summElements->setText(QString(" Сумма элементов на главной диагонали: %1").arg(sum));
}

void MatrixWindow::createArray() {
	QVector<int> sums;

	for (int i = 0; i < ui.matrixTable->rowCount(); ++i) {
		int sum = 0;
		for (int j = 0; j < ui.matrixTable->columnCount(); ++j) {
			QTableWidgetItem* item = ui.matrixTable->item(i, j);
			if (item != nullptr) {
				int value = item->text().toInt();
				if (value % 2 == 0) {
					sum += value;
				}
			}
		}
		sums.append(sum);
	}

	QString text;
	for (int i = 0; i < sums.size(); ++i) {
		text += QString("%1 ").arg(sums[i]);
	}

	ui.answerArray->setText(text);
}

bool MatrixWindow::checkMatrix() {
	int rowCount = ui.matrixTable->rowCount();
	int columnCount = ui.matrixTable->columnCount();

	for (int row = 0; row < rowCount; ++row) {
		for (int column = 0; column < columnCount; ++column) {
			QTableWidgetItem* item = ui.matrixTable->item(row, column);
			if (!item) {
				// Клетка пуста
                QMessageBox::critical(nullptr, QString("Ошибка"), QString("Клетка пуста"));
				return false;
			}

			QString text = item->text();
			bool isNumeric = true;
			bool hasMinus = false;

			for (int i = 0; i < text.length(); ++i) {
				if (text[i] == '-') {
					if (i > 0) {
						isNumeric = false;
						break;
					}
					hasMinus = true;
				}
				else if (!text[i].isDigit()) {
					isNumeric = false;
					break;
				}
			}

			if (!isNumeric) {
				// Клетка содержит нечисловое значение
                QMessageBox::critical(nullptr, QString("Ошибка"), QString("Клетка содержит нечисловое значение"));
				return false;
			}
		}
	}

	return true; // Все клетки содержат только числа
}

void MatrixWindow::setRandomData() {
	int rows = ui.matrixTable->rowCount();
	int columns = ui.matrixTable->colorCount();

	int maxValue = ui.maxIntMatrix->value();
	int minValue = ui.minIntMatrix->value();

	QRandomGenerator generator;

	for (int row = 0; row < rows; ++row) {
		for (int column = 0; column < columns; ++column) {
			int value = generator.bounded(maxValue - minValue + 1) + minValue;

			QTableWidgetItem* item = new QTableWidgetItem(QString::number(value));

			ui.matrixTable->setItem(row, column, item);
		}
	}
}

void MatrixWindow::createMatrix() {
	int rows = ui.ySizeMatrix->value();
	int columns = ui.xSizeMatrix->value();

	qDebug() << "Rows: " << rows << "; Columns: " << columns;

	QTableWidgetItem* item = new QTableWidgetItem();

	ui.matrixTable->setColumnCount(columns);
	ui.matrixTable->setRowCount(rows);
}

void MatrixWindow::saveData() {
	QString fileName = "ArrayData.txt";
	QString data = ui.answerArray->text();

	QFile file(fileName);
	if (file.exists()) {
		// Файл уже существует, предлагаем пользователю перезаписать его
        int ret = QMessageBox::warning(nullptr, QString("Файл уже существует"),
            QString("Файл уже существует. Перезаписать?"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No);
		if (ret == QMessageBox::No) {
			return; // Пользователь отказался от перезаписи
		}
	}

	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		// Ошибка открытия файла для записи
        QMessageBox::critical(nullptr, QString("Ошибка"), QString("Не могу открыть файл для записи"));
		return;
	}

	QTextStream out(&file);
	out << data;

	if (!file.flush()) {
		// Ошибка записи в файл
        QMessageBox::critical(nullptr, QString("Ошибка"), QString("Не могу записать в файл"));
		return;
	}

	file.close();
}

void MatrixWindow::clearMatrix() {
	int rowCount = ui.matrixTable->rowCount();
	int columnCount = ui.matrixTable->columnCount();

	for (int row = 0; row < rowCount; ++row) {
		for (int column = 0; column < columnCount; ++column) {
			QTableWidgetItem* item = ui.matrixTable->item(row, column);
			if (item) {
				item->setText("");
			}
		}
	}
}
