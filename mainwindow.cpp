#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new Server(this);
    configuration = new Configuration();
    connect(server, &Server::MoveToLog, this, &MainWindow::AddToLog);
    int port = configuration->ReadFromConf(PORT) != "default"?configuration->ReadFromConf(PORT).toInt() : 120;
    QString ip = configuration->ReadFromConf(IP_ADRESS) != "default"? configuration->ReadFromConf(IP_ADRESS).toString() : "127.0.0.1";
    if(server->listen(QHostAddress(ip), port))
        AddToLog("Сервер слушает порт 120");

    connect(ui->btnResend, &QPushButton::clicked, server, &Server::sendFileData);
    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::LoadFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddToLog(QString text)
{
    ui->textBrowser->append(text);
}

void MainWindow::LoadFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Загрузить файл", "", "Текстовый файл (*.txt)");
    if(filename.isEmpty())
        return;
    QFile* file = new QFile(filename);
   if(!file->open(QIODevice::ReadOnly))
       return;
    server->setFile(file->readAll());
}

