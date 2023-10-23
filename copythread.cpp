#include "copythread.h"

CopyThread::CopyThread(WsClient *client, QString *lp, QObject *parent)
    : QObject{parent}, client(client), localPath(lp)
{}

void CopyThread::reanudateCopies()
{
    qDebug() << "reanudate copies";
    copiesCountMutex.lock();
    if (copiesCount < maxCopies) {
        while (!pendingCopies.empty() && copiesCount < maxCopies) {
            CopyItem *ci = pendingCopies.first();
            pendingCopies.pop_front();

            if (requestCopy(ci)) {
                currentCopies.push_back(ci);
                copiesCount++;
            }
        }
    }
    copiesCountMutex.unlock();
}

bool CopyThread::requestCopy(CopyItem *copyItem)
{
    if (copyItem->getBytesCount() < copyItem->getBytesSize()) {
        createFileIfNotExist(copyItem);
        requestsMutex.lock();
        int id = client->copyRequest(copyItem->getBytesCount(), copyItem->getBytesCount() + chunkSize, copyItem->getHash());
        requests.append(qMakePair(id, copyItem->getHash()));
        requestsMutex.unlock();
        return true;
    }
    return false;
}

void CopyThread::onCopyData(QJsonDocument json)
{
    qDebug() << json;
    int id = json.object().value("id").toInt();
    int start = json.object().value("start").toInt();
    int end = json.object().value("end").toInt();
    QString data = json.object().value("data").toString();
    bool isLastData = json.object().value("last_data").toBool();
    qDebug() << data;

    requestsMutex.lock();
    QString hash = nullptr;
    for (int i = 0; i < requests.size(); i++) {
        if (requests.at(i).first == id) {
            hash = requests.at(i).second;
            requests.removeAt(i);
            break;
        }
    }
    if (hash == nullptr) {
        // todo
    }
    CopyItem *ci = nullptr;
    for (int i = 0; i < currentCopies.size(); i++) {
        CopyItem *v = currentCopies.at(i);
        if (hash == v->getHash()) {
            ci = v;
            break;
        }
    }
    requestsMutex.unlock();

    if (ci == nullptr) {
        // todo
    }

    QFile file((*localPath) + ci->getPath());
    file.open(QIODevice::Append);
    file.write(QByteArray::fromBase64(data.toUtf8()));
    file.close();

    if (isLastData) {
        for (int i = 0; i < currentCopies.size(); i++) {
            CopyItem *v = currentCopies.at(i);
            if (v->getHash() == hash) {
                currentCopies.removeAt(i);
                copiesCountMutex.lock();
                copiesCount--;
                copiesCountMutex.unlock();
                break;
            }
        }
        reanudateCopies();
    } else {
        ci->setBytesCount(end);
        if (requestCopy(ci)) {
            // todo
        }
    }
}

void CopyThread::appendCopy(QString path, QString hash, int size)
{
    qDebug() << "New copy: "<< path << ", " << hash << ", " << size;
    pendingCopies.append(new CopyItem(0, size, path, hash, READY));
}

bool CopyThread::fileExist(CopyItem *ci)
{
    QString filePath = (*localPath) + ci->getPath();
    qDebug() << filePath;
    return QFileInfo::exists(filePath);
}

void CopyThread::createFileIfNotExist(CopyItem *ci)
{
    if (!fileExist(ci)) {
        QFile file((*localPath) + ci->getPath());
        file.open(QIODevice::WriteOnly);
        file.close();
    }
}
