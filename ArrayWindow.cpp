#include "ArrayWindow.h"
#include <QRandomGenerator>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QRegExp>
#include <QThread>

ArrayWindow::ArrayWindow(QMainWindow* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.randomDataButton, &QPushButton::clicked, this, [this]() { setArrayData(); });
    connect(ui.readDataButton, &QPushButton::clicked, this, [this]() { openDatafile(); });
    connect(ui.startProcess, &QPushButton::clicked, this, [this]() {
        QThread* processThread = new QThread;
        QObject::connect(processThread, &QThread::started, this, [this]() {
            getElementsMultiples();
            getSummElements();
        });
        processThread->start();
    });

    connect(ui.startSortingButton, &QPushButton::clicked, this, [this]() {
        QThread* sortThread = new QThread;
        QObject::connect(sortThread, &QThread::started, this, [this]() {
            if (checkInput()) {
                if (ui.isQuickSort->isChecked()) {
                    quickSort();
                }
                if (ui.isInsertionSort->isChecked()) {
                    insertionSort();
                }
                else {
                }
            }
            else {

            }
        });
        sortThread->start();
    });

    connect(ui.arraySearchingButton, &QPushButton::clicked, this, [this]() {
        QThread* searchThread = new QThread;
        QObject::connect(searchThread, &QThread::started, this, [this]() {
            if (checkInput()) {
                if (ui.isNextSearch->isChecked()) {
                    sequentialSearch();
                }
                if (ui.isBinarySearch->isChecked()) {
                    binarySearch();
                }
                else {
                }
            }
            else {

            }
        });
        searchThread->start();
    });
}

ArrayWindow::~ArrayWindow()
{}

void ArrayWindow::binarySearch() {
    QString text = ui.dataArray->toPlainText();
    QString targetStr = ui.dataForSearch->text();
    int target = targetStr.toInt();

    QVector<int> arr;

    QRegExp rx("\\s*([-+]?\\d+)\\s*");
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1) {
        arr.append(rx.cap(1).toInt());
        pos += rx.matchedLength();
    }

    std::sort(arr.begin(), arr.end());

    int low = 0;
    int high = arr.size() - 1;
    bool found = false;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == target) {
            ui.sortedArray->setPlainText("Элемент найден: " + targetStr);
            found = true;
            break; // exit the loop
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (!found) {
        ui.sortedArray->setPlainText("Не найдено");
    }
}

void ArrayWindow::sequentialSearch() {
    QString inputText = ui.dataArray->toPlainText();
    QStringList elements = inputText.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    QVector<int> array;

    foreach(QString element, elements) {
        bool ok;
        int value = element.toInt(&ok);
        if (ok) {
            array.append(value);
        }
    }

    QString searchText = ui.dataForSearch->text();

    bool ok;
    int searchValue = searchText.toInt(&ok);

    if (ok) {
        int index = -1;
        for (int i = 0; i < array.size(); i++) {
            if (array[i] == searchValue) {
                index = i;
                break;
            }
        }

        QString outputText;
        if (index != -1) {
            outputText = "Элемент найден на позиции " + QString::number(index);
        }
        else {
            outputText = "Элемент не найден";
        }

        ui.sortedArray->setPlainText(outputText);
    }
    else {
        ui.dataForSearch->setText("Ошибка: текст для поиска должен быть целым числом");
    }
}

void ArrayWindow::setArrayData() {
    int minValue = ui.minValue->value();
    int maxValue = ui.maxValue->value();

    int size = ui.maxItem->value();

    int* array = new int[size];
    for (int i = 0; i < size; ++i) {
        array[i] = QRandomGenerator::global()->bounded(minValue, maxValue + 1);
    }

    QString arrayString;
    for (int i = 0; i < size; ++i) {
        arrayString += QString::number(array[i]) + " ";
    }

    ui.dataArray->setPlainText(arrayString);

    delete[] array;
}

void ArrayWindow::openDatafile() {
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Открыть файл", "", "Текстовые файлы (*.txt)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            QString fileContent = stream.readAll();
            file.close();

            ui.dataArray->setPlainText(fileContent);
        }
        else {
            ui.dataArray->setPlainText("Ошибка: не удалось открыть файл");
        }
    }
    else {

    }
}

void ArrayWindow::getElementsMultiples() {
    QString text = ui.dataArray->toPlainText();

    QStringList elements = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    int count = 0;

    foreach(QString element, elements) {
        bool ok;
        int value = element.toInt(&ok);

        if (ok && value % 5 == 0) {
            count++;
        }
    }

    ui.label->setText(QString("Количество элементов, кратных 5: %1").arg(count));
}

void ArrayWindow::getSummElements() {
    QString text = ui.dataArray->toPlainText();

    QStringList elements = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    QVector<int> array;

    foreach(QString element, elements) {
        bool ok;
        int value = element.toInt(&ok);
        if (ok) {
            array.append(value);
        }
    }

    if (array.isEmpty()) {
        ui.label_2->setText(QString("Массив пустой"));
    }

    int maxPositive = 0;

    for (int i = 0; i < array.size(); i++) {
        if (array[i] > 0 && array[i] > maxPositive) {
            maxPositive = array[i];
        }
    }

    if (array.size() < 2) {
        ui.label_2->setText(QString("Размер массива меньше 2"));
    }

    ui.label_2->setText(QString("Сумма второго и наибольшего положительного числа массива: %1").arg(array[1] + maxPositive));
}

void ArrayWindow::insertionSort() {
    QString inputText = ui.dataArray->toPlainText();

    QStringList elements = inputText.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    QVector<int> array;

    foreach(QString element, elements) {
        bool ok;
        int value = element.toInt(&ok);
        if (ok) {
            array.append(value);
        }
    }

    for (int i = 1; i < array.size(); i++) {
        int key = array[i];
        int j = i - 1;
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }

    QString outputText;
    for (int i = 0; i < array.size(); i++) {
        outputText += QString::number(array[i]) + " ";
    }

    ui.sortedArray->setPlainText(outputText);
}


void ArrayWindow::quickSort() {
    QString inputText = ui.dataArray->toPlainText();

    QStringList elements = inputText.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    QVector<int> array;

    foreach(QString element, elements) {
        bool ok;
        int value = element.toInt(&ok);
        if (ok) {
            array.append(value);
        }
    }

    quickSortImpl(array, 0, array.size() - 1);

    QString outputText;
    for (int i = 0; i < array.size(); i++) {
        outputText += QString::number(array[i]) + " ";
    }

    ui.sortedArray->setPlainText(outputText);
}

void ArrayWindow::quickSortImpl(QVector<int>& array, int left, int right) {
    int pivot = array[(left + right) / 2];
    int i = left;
    int j = right;

    while (i <= j) {
        while (array[i] < pivot) {
            i++;
        }
        while (array[j] > pivot) {
            j--;
        }
        if (i <= j) {
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }

    if (left < j) {
        quickSortImpl(array, left, j);
    }
    if (i < right) {
        quickSortImpl(array, i, right);
    }
}

bool ArrayWindow::checkInput() {
    QString inputText = ui.dataArray->toPlainText();
    QRegExp regex("^(-?\\d+(\\s+)?)+$"); // Регулярное выражение для проверки наличия только чисел

    if (!regex.exactMatch(inputText)) {
        QMessageBox::warning(nullptr, QString("Ошибка"), QString("Ввод должен содержать только числа, разделенные пробелами."));
        return false;
    }
    else {
        return true;
    }
}
