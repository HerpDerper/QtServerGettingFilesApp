#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "server.h"
#include "configuration.h"

const QString IP_ADRESS = "ip";
const QString PORT = "port";

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void AddToLog(QString text);
    void LoadFile();

private:
    Configuration* configuration;
    Server* server;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
