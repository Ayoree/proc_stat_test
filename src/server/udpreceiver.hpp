#pragma once

#include <QObject>
#include <QtNetwork/QUdpSocket>

class UdpReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UdpReceiver(quint16 port, QObject* parent = nullptr);

signals:
    void cpuDataReady(unsigned short coreIndex, double percent);

private slots:
    void onReadyRead();

private:
    QUdpSocket m_socket;
};