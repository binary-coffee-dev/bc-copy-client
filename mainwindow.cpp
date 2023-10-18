#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileSystemModel>
#include "itemmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // todo
//    QFileSystemModel *tree = new QFileSystemModel();
    ItemModel *tree = new ItemModel();

//    tree->setRootPath(QDir::currentPath());
//    tree->insertRow(0, new QModelIndex());

    this->ui->treeView->setModel(tree);
}

void MainWindow::on_pushButton_2_clicked()
{
    // todo
}
