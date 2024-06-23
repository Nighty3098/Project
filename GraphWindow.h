#pragma once

#include <QMainWindow>
#include "ui_graphWindow.h"
#include <QLineSeries>
#include "qcustomplot.h"

class GraphWindow  : public QMainWindow
{
	Q_OBJECT

public:
	GraphWindow(QMainWindow *parent);
	~GraphWindow();

private:
    Ui::GraphWindow ui;

    QCustomPlot* graphWidget;

private slots:
    void createGraph(QCustomPlot* customPlot, double xStart, double xEnd, double xStep);
};
