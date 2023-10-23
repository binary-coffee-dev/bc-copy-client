#ifndef APPSINGLETON_H
#define APPSINGLETON_H

#include <QObject>
#include <QJsonDocument>
#include <QMessageBox>

#include "wsclient.h"
#include "copythread.h"

class AppSingleton : public QObject
{
    Q_OBJECT
public:
    AppSingleton(QObject *parent = Q_NULLPTR);

    static AppSingleton *instanceValue;

    static AppSingleton *instance() {
        if (!instanceValue) {
            instanceValue = new AppSingleton();
        }
        return instanceValue;
    }
    QString getLocalPath();

    WsClient *client;
    CopyThread *copyThread;
    QSettings *settings;

private slots:
    void onAuthEvent(QJsonDocument json);
};

#endif // APPSINGLETON_H
