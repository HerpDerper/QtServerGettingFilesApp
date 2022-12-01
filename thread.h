#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QDataStream>

enum ThreadStatus
{
    FILE_SENDED, FILE_WAIT, NO_FILE
};

class Thread : public QThread
{

    Q_PROPERTY(QByteArray file READ getFile WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(bool force READ getForce WRITE setForce NOTIFY forceChanged)
    Q_OBJECT
    QString REQUEST = "request";
signals:
    void fileChanged();
    void forceChanged();
    void MoveToLog(QString text);
    void removeNeeded(int index);
public:
    Thread(QObject* parent = nullptr, qintptr descriptor = -1);
    ~Thread();
    void setFile(QByteArray file);
    QByteArray getFile() const;

    bool getForce() const;
    void setForce(bool value);
    void forceSending();
    void terminateThread();

    // QThread interface
protected:
    void run();

private:
    ThreadStatus status = NO_FILE;
    QTcpSocket* socket;
    qintptr descriptor;
    quint64 nextBlockSize;
    QByteArray file;
    bool force = false;

private slots:
    void sendFileData();
};

#endif // THREAD_H
