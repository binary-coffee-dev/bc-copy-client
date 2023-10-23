#include "properties.h"
#include "ui_properties.h"

Properties::Properties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Properties)
{
    ui->setupUi(this);

    QPushButton* applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(applyButton, &QPushButton::clicked, this,  &Properties::on_buttonBox_apply);

    ui->serverDomainEdit->setText(AppSingleton::instance()->settings->value(Constants::domainConfigName(), "").toString());
    ui->clientNameEdit->setText(AppSingleton::instance()->settings->value(Constants::clientNameConfigName(), "").toString());
    ui->clientKeyEdit->setText(AppSingleton::instance()->settings->value(Constants::clientKeyConfigName(), "").toString());

    // todo: use Download folder by default
    QString downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    ui->copyFolder->setText(AppSingleton::instance()->settings->value(Constants::localDirectoryConfigName(), downloadDirectory).toString());

//    QString path = "/";
//    QFileSystemModel* fileModel = new QFileSystemModel(this);
//    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
//    //set root path
//    fileModel->setRootPath(path);

//    ui->treeView->setModel(fileModel);
}

Properties::~Properties()
{
    delete ui;
}

void Properties::on_buttonBox_accepted()
{
    qDebug() << "accepted";
    on_buttonBox_apply();
}

void Properties::on_buttonBox_rejected()
{
    qDebug() << "rejected";
}

void Properties::on_buttonBox_apply()
{
    qDebug() << "apply";
    AppSingleton::instance()->settings->setValue(Constants::domainConfigName(), ui->serverDomainEdit->text());
    AppSingleton::instance()->settings->setValue(Constants::clientNameConfigName(), ui->clientNameEdit->text());
    AppSingleton::instance()->settings->setValue(Constants::clientKeyConfigName(), ui->clientKeyEdit->text());
    AppSingleton::instance()->settings->setValue(Constants::localDirectoryConfigName(), ui->copyFolder->text());

    // restart ws connection
    AppSingleton::instance()->client->connectToServer();
}

void Properties::on_pushButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOptions(QFileDialog::ShowDirsOnly);
    dialog.setDirectory(ui->copyFolder->text());
    if (dialog.exec()) {
        QList<QString> dirs = dialog.selectedFiles();
        qDebug() << dirs;
        if ( dirs.size() > 0) {
            QString saveFilesDirectory = dirs.at(0);
            ui->copyFolder->setText(saveFilesDirectory);
        }
    }
}
