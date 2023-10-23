#include "wsclient.h"
#include "qmessagebox.h"

#include <QtWebSockets/QWebSocket>

WsClient::WsClient(QSettings *settings, QObject *parent) : QObject(parent), settings(settings)
{
    authStatus = false;
    qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
}

void WsClient::sendTextMessage(QString message)
{
    websocket->sendTextMessage(message);
}

void WsClient::onClosed()
{
    qDebug() << "Client disconnected";
    authStatus = false;
}

void WsClient::onConnected()
{
    QString clientName = settings->value(Constants::clientNameConfigName(), "").toString();
    QString clientKey = settings->value(Constants::clientKeyConfigName(), "").toString();
    qDebug() << "Client connected" << clientName << clientKey;
    authorization(clientName, clientKey);
}

void WsClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received: " << message;

    QJsonDocument jsonMessage = QJsonDocument::fromJson(message.toUtf8());

    MessageType type = unregisterMessage(jsonMessage.object().value("id").toInt());

    switch (type) {
    case AUTH_MSG:
        qDebug() << "Auth response";

        // set auth
        authStatus = jsonMessage.object().value("status").toString() == "accepted";

        onAuthResponse(jsonMessage);
        break;
    case TREE_MSG:
        qDebug() << "Tree response";
        onTreeChange(jsonMessage);
        break;
    case COPY_MSG:
        qDebug() << "Copy response";
        onCopyMessage(jsonMessage);
        break;
    default:
        break;
    }
}

int WsClient::generateMessageId()
{
    return (int) QDateTime::currentMSecsSinceEpoch();
}

void WsClient::registerMessage(int id, MessageType type)
{
    messages[id] = type;
}

MessageType WsClient::unregisterMessage(int id)
{
    if (messages.contains(id)) {
        return messages[id];
    }
    return MessageType::UNKOWN;
}

void WsClient::getTree()
{
    if (authStatus) {
        qDebug() << "Get tree from server";
        int id = generateMessageId();
        registerMessage(id, TREE_MSG);
        sendTextMessage(QString("{\"id\": %1,\"type\":\"TreeMsg\"}").arg(QString::number(id)));
    }
}

int WsClient::copyRequest(int start, int end, QString hash)
{
    if (authStatus) {
        qDebug() << "Copy request message";
        int id = generateMessageId();
        registerMessage(id, COPY_MSG);
        QString cpyMsg = QString("{\"type\":\"CopyMsg\", \"id\": %1, \"start\": %2, \"end\": %3, \"file_hash\": \"%4\"}")
                             .arg(QString::number(id), QString::number(start), QString::number(end), hash);
        sendTextMessage(cpyMsg);
        return id;
    }
    return -1;
}

void WsClient::authorization(QString name, QString password)
{
    qDebug() << "Authorization message";
    int id = generateMessageId();
    registerMessage(id, AUTH_MSG);
    sendTextMessage(QString("{\"id\": %1, \"name\": \"%2\", \"key\":\"%3\", \"type\":\"AuthMsg\"}").arg(QString::number(id), name, password));
}

bool WsClient::connectToServer()
{
    QString domain = settings->value(Constants::domainConfigName(), "").toString();
    qDebug() << "Server url: " << domain;
    if (domain != "") {
        websocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest);

        connect(websocket, &QWebSocket::connected, this, &WsClient::onConnected);
        connect(websocket, &QWebSocket::disconnected, this, &WsClient::onClosed);
        connect(websocket, &QWebSocket::textMessageReceived, this, &WsClient::onTextMessageReceived);

        websocket->open(QUrl(domain));
    } else {
        QMessageBox msgBox;
        msgBox.setText("Invalid server url.");
        msgBox.exec();
    }
}
