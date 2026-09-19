#include "utils.h"

QList<Utils::BroadcastTarget> Utils::getBroadcastTargets()
{
    QList<BroadcastTarget> targets;

    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        if (interface.type() != QNetworkInterface::Ethernet && interface.type() != QNetworkInterface::Wifi)
            continue;

        if (!interface.flags().testFlag(QNetworkInterface::IsUp) || !interface.flags().testFlag(QNetworkInterface::IsRunning))
            continue;

        foreach(QNetworkAddressEntry entry, interface.addressEntries()) {
            if (!entry.ip().isGlobal() || entry.ip().protocol() != QAbstractSocket::IPv4Protocol)
                continue;

            // Point-to-point and some virtual interfaces have no broadcast address, fall back to the limited broadcast
            QHostAddress broadcastAddress = entry.broadcast().isNull() ? QHostAddress(QHostAddress::Broadcast) : entry.broadcast();
            targets.append({entry.ip().toString(), broadcastAddress});
        }
    }

    return targets;
}

QStringList Utils::getLocalIps()
{
    QStringList cleanedAddresses;

    foreach(BroadcastTarget target, Utils::getBroadcastTargets()) {
        cleanedAddresses.append(target.localIp);
    }

    cleanedAddresses.removeDuplicates();
    return cleanedAddresses;
}

void Utils::sendInbandBroadcast(QString target)
{
    QUdpSocket udpSocket;

    foreach(BroadcastTarget broadcastTarget, Utils::getBroadcastTargets()) {
        // Each network gets its own local IP in the datagram, so the target answers to the reachable address
        QByteArray datagram = "InBand uu?;" + broadcastTarget.localIp.toLocal8Bit() + ";" + target.toLocal8Bit();
        udpSocket.writeDatagram(datagram.data(), datagram.size(), broadcastTarget.broadcastAddress, 45744);
        udpSocket.waitForBytesWritten();
    }

    udpSocket.disconnectFromHost();
}
