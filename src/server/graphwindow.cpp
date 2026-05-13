#include "graphwindow.hpp"
#include "ui_graphwindow.h"

#include <QTime>
#include <QColor>
#include <QFont>
#include <QtMath>

GraphWindow::GraphWindow(unsigned short udpPort, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::GraphWindow)
    , m_udp(udpPort, this)
    , m_start(QTime::currentTime())
{
    ui->setupUi(this);
    setWindowTitle("/proc/stat - CPU load monitor");
    setGeometry(400, 250, 800, 480);
    setupWindow(ui->customPlot);

    connect(&m_udp, &UdpReceiver::cpuDataReady, this, &GraphWindow::onCpuDataReceived);
}
GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::setupWindow(QCustomPlot *customPlot)
{
    customPlot->clearGraphs();

    customPlot->xAxis->setLabel("Time (s)");
    customPlot->yAxis->setLabel("CPU load (%)");
    customPlot->yAxis->setRange(0.0, 100.0);
    customPlot->xAxis->setRange(0.0, s_kWindowSec);

    customPlot->axisRect()->setupFullAxesBox(true);
    customPlot->xAxis->setTicks(true);
    customPlot->xAxis->setTickLabels(true);

    customPlot->setBackground(QBrush(QColor(18, 18, 28)));
    customPlot->axisRect()->setBackground(QBrush(QColor(18, 18, 28)));

    auto styleAxis = [](QCPAxis *ax) {
        ax->setBasePen(QPen(QColor(120, 120, 140)));
        ax->setTickPen(QPen(QColor(120, 120, 140)));
        ax->setSubTickPen(QPen(QColor(80, 80, 100)));
        ax->setTickLabelColor(QColor(200, 200, 220));
        ax->setLabelColor(QColor(200, 200, 220));
        ax->grid()->setPen(QPen(QColor(50, 50, 70), 1, Qt::DotLine));
        ax->grid()->setSubGridVisible(false);
    };
    styleAxis(customPlot->xAxis);
    styleAxis(customPlot->yAxis);
    styleAxis(customPlot->xAxis2);
    styleAxis(customPlot->yAxis2);

    customPlot->replot();
}

QCPGraph* GraphWindow::graphForCore(unsigned short coreIndex)
{
    if (m_graphs.contains(coreIndex))
        return m_graphs[coreIndex];

    QCustomPlot* cp = ui->customPlot;
    QCPGraph* g = cp->addGraph();

    if (coreIndex == ALL_CORES_ID)
    {
        QPen pen(Qt::white);
        pen.setWidthF(3.6);
        pen.setStyle(Qt::DashLine);
        g->setPen(pen);
        g->setName("Total");
    }
    else
    {
        const int idx = m_graphs.size();
        const int hue = (idx * 137) % 360;
        QPen pen(QColor::fromHsv(hue, 210, 150));
        pen.setWidthF(1.8);
        g->setPen(pen);
        g->setName(QStringLiteral("cpu%1").arg(coreIndex));
    }

    g->setLineStyle(QCPGraph::lsLine);
    g->setScatterStyle(QCPScatterStyle::ssNone);

    cp->legend->setVisible(true);

    m_graphs.insert(coreIndex, g);
    return g;
}

void GraphWindow::onCpuDataReceived(unsigned short coreIndex, double percent)
{
    const double now = m_start.msecsTo(QTime::currentTime()) / 1000.0;
    QCustomPlot* cp  = ui->customPlot;

    graphForCore(coreIndex)->addData(now, percent * 100.0);

    const double pruneKey = now - s_kWindowSec * 2.0;
    for (QCPGraph* g : std::as_const(m_graphs))
        g->data()->removeBefore(pruneKey);

    cp->xAxis->setRange(now, s_kWindowSec, Qt::AlignRight);
    cp->replot(QCustomPlot::rpQueuedReplot);

    statusBar()->showMessage(
        QStringLiteral("last message time: %3 s")
            .arg(now, 0, 'f', 1));
}