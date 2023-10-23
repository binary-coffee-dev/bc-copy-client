#ifndef WSCLIENT_H
#define WSCLIENT_H

#include <QObject>
#include <QtWebSockets/QtWebSockets>
#include <QMap>

enum MessageType { TREE_MSG, AUTH_MSG, COPY_MSG, UNKOWN };

class WsClient : public QObject
{
    Q_OBJECT
public:
    explicit WsClient(const QUrl &url, QObject *parent = Q_NULLPTR);

    void sendTextMessage(QString message);
    void getTree();
    int copyRequest(int start, int end, QString hash);
    void authorization(QString name, QString password);

Q_SIGNALS:
    void onTreeChange(QJsonDocument json);
    void onCopyMessage(QJsonDocument json);
    void onAuthResponse(QJsonDocument json);

protected:
    void onClosed();
    void onConnected();
    void onTextMessageReceived(QString message);
    int generateMessageId();

    void registerMessage(int id, MessageType type);
    MessageType unregisterMessage(int id);

private:
    QWebSocket *websocket;
    QUrl url;

    QMap<int, MessageType> messages;
};


#endif // WSCLIENT_H
