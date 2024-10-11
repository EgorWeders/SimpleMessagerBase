#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "../base/client.h"
#include <QHostAddress>
#include <QObject>
#include <QUdpSocket>
const int DEFAULT_PORT = 65001;
class UdpClient : public QObject, public Client {
	Q_OBJECT
	QUdpSocket   socket;
	QHostAddress target;
	QHostAddress host;
    int port = DEFAULT_PORT;
    int targetPort = DEFAULT_PORT;
    void bind()
    {
        socket.bind(host, port);
        connect(&socket, &QUdpSocket::readyRead, this, &UdpClient::acceptData);
    }
private slots:
	void acceptData() {
		QByteArray buff;
		buff.resize(socket.pendingDatagramSize());
		if (socket.open(QIODevice::ReadWrite)) {
			socket.readDatagram(buff.data(), buff.size());
			this->recvQueue.push_back(Message(buff.data(), buff.size()));
		}
        auto msg = this->getLastMessage();
        qDebug() << "Recv id" << msg.id() << "body" << QString::fromStdString(msg.stringBody());
    };
    void sendData() {};

public:
    UdpClient(const QString &addr, int port = DEFAULT_PORT)
        : socket(this)
        , host(addr)
        , port(port)
    {
        bind();
        this->idGen = new IdCounterGenerator();
    };
    UdpClient(const QHostAddress &addr, int port = DEFAULT_PORT)
        : socket(this)
        , host(addr)
        , port(port)
    {
        bind();
        this->idGen = new IdCounterGenerator();
    };
    bool isHostValid() { return !host.isNull(); }
    bool isTargetValid() { return !target.isNull(); }
    bool setTarget(const QString &addr, int port = DEFAULT_PORT)
    {
        target.setAddress(addr);
        targetPort = port;
        return !target.isNull();
    }
    bool setTarget(const QHostAddress &addr, int port = DEFAULT_PORT)
    {
        target.setAddress(addr.toString());
        targetPort = port;
        return !target.isNull();
    }
    const QHostAddress &getTarget() const { return target; }

    const QHostAddress &getHost() const { return host; }
    // Client interface
public:
	bool sendMessage() override {

		if (senderQueue.size() <= 0)
			return false;
		Message tempMsgd = senderQueue.back();
		if (tempMsgd.isNull()) {
			return false;
		}
		if (this->sendMutator)
			tempMsgd.applyFilter(sendMutator);
		senderQueue.pop_back();

        qDebug() << "Sending id <" << tempMsgd.id() << "> body <"
                 << QByteArray(tempMsgd.body(), tempMsgd.bodySize()) << "> to" << target
                 << targetPort;
        return socket.writeDatagram(QByteArray(tempMsgd.data(), tempMsgd.dataSize()),
                                    target,
                                    targetPort);
    }
};

#endif // UDPCLIENT_H
