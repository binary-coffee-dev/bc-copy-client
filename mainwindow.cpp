#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    AppSingleton::instance();

    QStringList headers;
    headers << tr("Name") << tr("Progress") << tr("Down rate") << tr("Status");

    QTreeWidget *list = ui->copiesList;
    list->setItemDelegate(new CopyViewDelegate(list));
    list->setHeaderLabels(headers);
//    list->setSelectionBehavior(QAbstractItemView::SelectRows);
    list->setAlternatingRowColors(true);
    list->setRootIsDecorated(false);

    QFontMetrics fm = fontMetrics();
    QHeaderView *header = list->header();
    header->resizeSection(0, fm.horizontalAdvance("file-name"));
    header->resizeSection(1, fm.horizontalAdvance(headers.at(1) + "  "));
    header->resizeSection(2, qMax(fm.horizontalAdvance(headers.at(2) + "  "), fm.horizontalAdvance(" 1234.0 KB/s ")));
    header->resizeSection(4, qMax(fm.horizontalAdvance(headers.at(3) + "  "), fm.horizontalAdvance(tr("Downloading") + "  ")));

//    list->setItemWidget(progress, );
//    list->addItem(progress);

    updateCopiesList();
//    updateCopiesList();
//    updateCopiesList();
//    updateCopiesList();
//    updateCopiesList();
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

void MainWindow::updateCopiesList()
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->copiesList);

    item->setText(0, "some");
    item->setText(1, "");
    item->setText(2, "some");
    item->setText(3, "some");

    ui->copiesList->addTopLevelItem(item);
}
