#include "GraphWindow.h"
#include <QFile>
#include <QUrl>
#include <QLineSeries>
#include "qcustomplot.h"
#include <QThread>

GraphWindow::GraphWindow(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    ui.equationLabel->setPixmap(QPixmap(":/MS_VS_GAS/resources/equation.png"));

    graphWidget = new QCustomPlot();
    graphWidget->xAxis->setRange(-50, 50);
    graphWidget->yAxis->setRange(-50, 50);

    ui.graphLayout->addWidget(graphWidget);

    connect(ui.startProcess, &QPushButton::clicked, this, [this]() {
        QThread* graphThread = new QThread;
        QObject::connect(graphThread, &QThread::started, this, [this]() {
            double xn = ui.startLineEdit->text().toDouble();
            double xk = ui.endLineEdit->text().toDouble();
            double xh = ui.stepLineEdit->text().toDouble();

            if (xh <= 0) {
                QMessageBox::critical(nullptr, QString("Ошибка"), QString("Xh не должно быть меньше 0.1"));
            }
            else {
                createGraph(graphWidget, xn, xk, xh);
            }
        });
        graphThread->start();
    });
}

GraphWindow::~GraphWindow()
{}

void GraphWindow::createGraph(QCustomPlot* customPlot, double xStart, double xEnd, double xStep) {
    // Создаем векторы для хранения значений x и y
    QVector<double> x, y;

    // Вычисляем значения функции в заданном промежутке
    // Мы используем цикл for для перебора значений аргумента функции от xn до xk с шагом xh
    for (double xVal = xStart; xVal <= xEnd; xVal += xStep) {
        // Вычисляем значение функции для текущего значения аргумента
        double yVal;
        // Проверяем, в какой области определения функции находится текущее значение аргумента
        if (xVal <= 0) {
            // Если xVal <= 0, то вычисляем значение функции по первой формуле
            yVal = pow(xVal, 2) + sin(7 * xVal) - 1;
        }
        else if (xVal <= 5) {
            // Если 0 < xVal <= 5, то вычисляем значение функции по второй формуле
            yVal = fabs(pow(xVal, 3) + pow(10, xVal));
        }
        else if(xVal > 5) {
            // Если xVal > 5, то вычисляем значение функции по третьей формуле
            yVal = pow(2 * pow(xVal, 4) + pow(xVal, 2) + 1, 1.0 / 7);
        }
        else {

        }
        // Добавляем вычисленные значения x и y в соответствующие векторы
        x.append(xVal);
        y.append(yVal);
    }

    // Создаем график
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    customPlot->graph(0)->setPen(QPen(Qt::blue));

    // Установка осей
    customPlot->xAxis->setRange(xStart, xEnd);
    customPlot->yAxis->setRange(-10, 100); // Установка диапазона оси y

    // Обновляем график
    customPlot->replot();
}
