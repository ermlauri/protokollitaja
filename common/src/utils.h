#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QStringList>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QUdpSocket>

class Utils : public QObject
{
    Q_OBJECT
public:
    struct BroadcastTarget {
        QString localIp;
        QHostAddress broadcastAddress;
    };

    static QList<BroadcastTarget> getBroadcastTargets();
    static QStringList getLocalIps();
    static void sendInbandBroadcast(QString target);
};

#endif // UTILS_H
