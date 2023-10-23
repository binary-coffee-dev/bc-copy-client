#include "appsingleton.h"

AppSingleton* AppSingleton::instanceValue = 0;

AppSingleton::AppSingleton(QObject *parent): QObject(parent)
{
    QCoreApplication::setOrganizationName("CopyService");
    QCoreApplication::setOrganizationDomain("copyservice.online");
    QCoreApplication::setApplicationName("CopyClient");
    settings = new QSettings();

    // start webSocket client
    client = new WsClient(settings);
//    connect(client, &WsClient::onTreeChange, this, &MainWindow::onTreeChangeEvent);
//    connect(client, &WsClient::onCopyMessage, this, &MainWindow::onCopyEvent);
    connect(client, &WsClient::onAuthResponse, this, &AppSingleton::onAuthEvent);

    // start thread copies
    copyThread = new CopyThread(client, settings);
    QObject::connect(client, &WsClient::onCopyMessage, copyThread, &CopyThread::onCopyData);

    // try to connect to the server
    client->connectToServer();
}

QString AppSingleton::getLocalPath()
{
    return settings->value(Constants::localDirectoryConfigName()).toString();
}

void AppSingleton::onAuthEvent(QJsonDocument json)
{
    bool authStatus = json.object().value("status").toString() == "accepted";
    if (!authStatus) {
        QMessageBox msgBox;
        msgBox.setText("Error to authenticate in the server.");
        msgBox.exec();
    }
}
