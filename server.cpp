#include "server.h"


Server::Server(QObject *parent):QTcpServer(parent)
{

}

void Server::setFile(QByteArray file)
{
    for(Thread* th : threads)
        th->setFile(file);
}

void Server::removeSocket(int index)
{
    emit MoveToLog(QString("Пользователь с дескриптором %1 отключился").arg(index));
    threads.remove(index);
}

void Server::sendFileData()
{
    for(Thread* th : threads)
        th->setForce(true);
}


void Server::incomingConnection(qintptr descriptor)
{
    Thread* thread = new Thread(this, descriptor);
    threads.insert(descriptor, thread);
    connect(thread, &Thread::removeNeeded, this, &Server::removeSocket, Qt::DirectConnection);
    connect(thread, &Thread::MoveToLog, this, &Server::MoveToLog, Qt::DirectConnection);
    thread->start();
    emit MoveToLog(QString("Пользователь с дескриптором %1 подключён").arg(descriptor));
}
