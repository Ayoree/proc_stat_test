#pragma once

#include <QMainWindow>
#include <QMap>
#include <QTime>
#include "qcustomplot/qcustomplot.hpp"
#include "udpreceiver.hpp"

namespace Ui { class GraphWindow; }

class GraphWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GraphWindow(unsigned short udpPort, QWidget* parent = nullptr);
    ~GraphWindow();

    void setupWindow(QCustomPlot* customPlot);

public slots:
    void onCpuDataReceived(unsigned short coreIndex, double percent);

private:
    QCPGraph* graphForCore(unsigned short coreIndex);

    Ui::GraphWindow* ui;
    UdpReceiver m_udp;
    QTime m_start;

    QMap<unsigned short, QCPGraph*> m_graphs;

    static constexpr double s_kWindowSec = 60.0;
};