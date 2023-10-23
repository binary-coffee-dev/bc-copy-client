#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QDialog>
#include <QAbstractButton>
#include <QPushButton>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QObject>

#include "appsingleton.h"
#include "constvariables.h"

namespace Ui {
class Properties;
}

class Properties : public QDialog
{
    Q_OBJECT

public:
    explicit Properties(QWidget *parent = nullptr);
    ~Properties();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_buttonBox_apply();

    void on_pushButton_clicked();

private:
    Ui::Properties *ui;
};

#endif // PROPERTIES_H
