#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QObject>

#include "wsclient.h"

enum CopyStatus { READY, RUNNING, PAUSE, FINISH };

class CopyItem {
public:
    CopyItem() {};
    CopyItem(int bytesCount, int bytesSize, QString path, QString hash, CopyStatus status)
        : bytesCount(bytesCount), bytesSize(bytesSize), path(path), hash(hash), status(status) {};

    int getBytesCount() {
        return bytesCount;
    };
    int getBytesSize() {
        return bytesSize;
    }
    QString getHash() {
        return hash;
    }
    QString getPath() {
        return path;
    }
    void setBytesCount(int bytes) {
        bytesCount = bytes;
    }

private:
    int bytesCount;
    int bytesSize;
    QString path;
    QString hash;
    CopyStatus status;
};

class CopyThread : public QObject
{
    Q_OBJECT
public:
    explicit CopyThread(WsClient *client, QString *lp, QObject *parent = nullptr);

    void reanudateCopies();
    bool requestCopy(CopyItem *copyItem);
    void onCopyData(QJsonDocument json);
    void appendCopy(QString path, QString hash, int size);
    bool fileExist(CopyItem *ci);
    void createFileIfNotExist(CopyItem *ci);

signals:

private:
    QList<CopyItem*> currentCopies;
    QMutex currentCopiesMutex;

    QList<CopyItem*> pendingCopies;
    WsClient *client;
    const int maxCopies = 2;
    const int chunkSize = 255;
    QString *localPath;

    int copiesCount = 0;
    QMutex copiesCountMutex;

    QList<QPair<int, QString>> requests;
    QMutex requestsMutex;
};

#endif // COPYTHREAD_H
