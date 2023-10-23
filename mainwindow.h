#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QPair>
#include <QJsonDocument>

#include "itemmodel.h"
#include "appsingleton.h"
#include "properties.h"
#include "filestree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionProperties_triggered();

    void on_actionClose_triggered();

    void on_newCopies_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
