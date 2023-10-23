#include "wsclient.h"

#include <QtWebSockets/QWebSocket>

WsClient::WsClient(const QUrl &url, QObject *parent) :
    QObject(parent), url(url)
{
    qDebug() << "Server url: " << url;
    qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();

    websocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest);

    connect(websocket, &QWebSocket::connected, this, &WsClient::onConnected);
    connect(websocket, &QWebSocket::disconnected, this, &WsClient::onClosed);
    connect(websocket, &QWebSocket::textMessageReceived, this, &WsClient::onTextMessageReceived);

    websocket->open(url);
}

void WsClient::sendTextMessage(QString message)
{
    websocket->sendTextMessage(message);
}

void WsClient::onClosed()
{
    qDebug() << "Client disconnected";
}

void WsClient::onConnected()
{
    qDebug() << "Client connected";
    authorization("test", "g0qycWPxJp-turMI");
}

void WsClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received: " << message;

    QJsonDocument jsonMessage = QJsonDocument::fromJson(message.toUtf8());

    MessageType type = unregisterMessage(jsonMessage.object().value("id").toInt());

    switch (type) {
    case AUTH_MSG:
        qDebug() << "Auth response";
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
    qDebug() << "Get tree from server";
    int id = generateMessageId();
    registerMessage(id, TREE_MSG);
    sendTextMessage(QString("{\"id\": %1,\"type\":\"TreeMsg\"}").arg(QString::number(id)));
}

int WsClient::copyRequest(int start, int end, QString hash)
{
    qDebug() << "Copy request message";
    int id = generateMessageId();
    registerMessage(id, COPY_MSG);
    QString cpyMsg = QString("{\"type\":\"CopyMsg\", \"id\": %1, \"start\": %2, \"end\": %3, \"file_hash\": \"%4\"}")
                         .arg(QString::number(id), QString::number(start), QString::number(end), hash);
    sendTextMessage(cpyMsg);
    return id;
}

void WsClient::authorization(QString name, QString password)
{
    qDebug() << "Authorization message";
    int id = generateMessageId();
    registerMessage(id, AUTH_MSG);
    sendTextMessage(QString("{\"id\": %1, \"name\": \"%2\", \"key\":\"%3\", \"type\":\"AuthMsg\"}").arg(QString::number(id), name, password));
}
