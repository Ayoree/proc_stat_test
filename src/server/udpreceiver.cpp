#include "udpreceiver.hpp"
#include <QtNetwork/QNetworkDatagram>

UdpReceiver::UdpReceiver(unsigned short port, QObject* parent)
    : QObject(parent)
{
    m_socket.bind(QHostAddress::Any, port);
    connect(&m_socket, &QUdpSocket::readyRead, this, &UdpReceiver::onReadyRead);
}

void UdpReceiver::onReadyRead()
{
    while (m_socket.hasPendingDatagrams()) {
        const QByteArray data = m_socket.receiveDatagram().data();
        if (static_cast<size_t>(data.size()) < sizeof(udp_client_message_t))
            continue;

        udp_client_message_t msg;
        memcpy(&msg, data.constData(), sizeof(msg));
        emit cpuDataReady(msg.core_num, msg.percent);
    }
}