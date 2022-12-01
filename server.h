#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QVector>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QThread>
#include <QMap>
#include "thread.h"




class Server : public QTcpServer
{
    Q_OBJECT

signals:
    void MoveToLog(QString text);
public:
    Server(QObject* parent);
    void setFile(QByteArray file);
    void removeSocket(int index);
    void sendFileData();
protected:
    void incomingConnection(qintptr descriptor);
private:


    QMap<int, Thread*> threads = QMap<int, Thread*>();


};

#endif // SERVER_H
