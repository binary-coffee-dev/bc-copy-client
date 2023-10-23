#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QPair>

#include "itemmodel.h"
#include "wsclient.h"
#include "copythread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void onTreeChangeEvent(QJsonDocument json);
    void onCopyEvent(QJsonDocument json);
    void onAuthEvent(QJsonDocument json);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    QPair<QString, TreeItemType> getFileType(QString fileName);

    void loadLocalDirectory();
    void createParentPaths(TreeItem *treeItem);
    void createChildrenPaths(TreeItem *treeItem);
    QList<TreeItem*> listOfCopyItems(TreeItem *treeItem);

private:
    Ui::MainWindow *ui;
    WsClient *client;
    CopyThread *copyThread;

    QString localDirectory;
};

#endif // MAINWINDOW_H
