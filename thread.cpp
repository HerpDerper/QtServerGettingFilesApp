#include "thread.h"
#include <QDebug>

Thread::Thread(QObject *parent, qintptr descriptor)
{
    nextBlockSize = 0;
    this->descriptor = descriptor;
}

Thread::~Thread()
{
    emit removeNeeded(descriptor);
    delete socket;
    terminate();
    quit();
}

void Thread::run()
{
    socket = new QTcpSocket;
    connect(socket, &QTcpSocket::disconnected, this, &Thread::deleteLater, Qt::DirectConnection);
    socket->setSocketDescriptor(descriptor);
    socket->open(QIODevice::ReadWrite);
    connect(socket, &QTcpSocket::readyRead, this, &Thread::sendFileData, Qt::DirectConnection);
    connect(this, &Thread::fileChanged, this, &Thread::sendFileData, Qt::DirectConnection);
    exec();
}

bool Thread::getForce() const
{
    return force;
}

void Thread::setForce(bool value)
{
    force = value;
    if (value == true){
        forceSending();
    }
    emit forceChanged();
}

void Thread::sendFileData()
{
    if (socket->state() != QTcpSocket::SocketState::ConnectedState){
        emit MoveToLog("Пользователь не подключён");
        return;
    }
    QString requested = 0;
    if(socket->bytesAvailable()!=0)
    {
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_5_10);
        if (nextBlockSize == 0) {
            if ( socket->bytesAvailable() < (int)sizeof(quint64) )
                return;
            in >> nextBlockSize;
        }
        if (socket->bytesAvailable() < nextBlockSize)
            return;

        QByteArray file;
        in >> requested;
        qDebug() << requested;
        nextBlockSize = 0;
    }
    if (status  == ThreadStatus::NO_FILE){
       emit MoveToLog("Данные не могу быть отправленны, так как файл не был загружен");
       return;
    }
    if(status ==ThreadStatus::FILE_WAIT || requested == REQUEST || force)
    {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);
        out << quint64(0) << file;
        out.device()->seek(0);
        out << quint64(data.size() - sizeof(quint64));
        socket->write(data);
        socket->waitForBytesWritten();
        status = ThreadStatus::FILE_SENDED;
        emit MoveToLog(QString("Файл с размером %1, был отправлен").arg(file.size()));
        force = false;
    }
}

void Thread::forceSending()
{
    if (status == ThreadStatus::NO_FILE)
    {
        emit MoveToLog("Невозможно отправить данные, файл не задан");
        return;
    }
    force = true;
    emit MoveToLog("Производиться отправка данных");
    sendFileData();
}

void Thread::terminateThread()
{
    emit removeNeeded(descriptor);
}

void Thread::setFile(QByteArray file)
{
    this->file = file;
    status = ThreadStatus::FILE_WAIT;
    emit fileChanged();
    emit MoveToLog(QString("Файл с размером %1, был установлен в качестве текущего").arg(file.size()));
}

QByteArray Thread::getFile() const
{
    return file;
}
