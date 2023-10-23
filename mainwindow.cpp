#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileSystemModel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // start webSocket client
//    client = new WsClient(QUrl(QStringLiteral("ws://localhost:9001")));
//    connect(client, &WsClient::onTreeChange, this, &MainWindow::onTreeChangeEvent);
//    connect(client, &WsClient::onCopyMessage, this, &MainWindow::onCopyEvent);
//    connect(client, &WsClient::onAuthResponse, this, &MainWindow::onAuthEvent);

    // local directory
//    localDirectory = QString("/home/gonzalezext/opt/data/copy-service");
//    loadLocalDirectory();
    AppSingleton::instance();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionProperties_triggered()
{
    Properties *properties = new Properties(this);
    properties->show();
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::on_newCopies_clicked()
{
    FilesTree *filesTree = new FilesTree(this);
    filesTree->show();
}

