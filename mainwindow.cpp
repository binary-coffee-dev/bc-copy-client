#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileSystemModel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // start webSocket client
    client = new WsClient(QUrl(QStringLiteral("ws://localhost:9001")));
    connect(client, &WsClient::onTreeChange, this, &MainWindow::onTreeChangeEvent);
    connect(client, &WsClient::onCopyMessage, this, &MainWindow::onCopyEvent);
    connect(client, &WsClient::onAuthResponse, this, &MainWindow::onAuthEvent);

    // local directory
    localDirectory = QString("/home/gonzalezext/opt/data/copy-service");
    loadLocalDirectory();

    // start thread copies
    copyThread = new CopyThread(client, &localDirectory);
}

void MainWindow::loadLocalDirectory()
{
    TreeItem *rootItem = new TreeItem({tr("Name"), tr("Type"), tr("Size")});
    QDir dir(localDirectory);

    qDebug() << dir.entryList(QStringList(), QDir::Files);
    qDebug() << dir.entryList(QStringList(), QDir::Dirs);

    QQueue<QDir> q;
    QQueue<TreeItem*> qt;

    q.enqueue(dir);
    qt.enqueue(rootItem);

    while (!q.empty()) {
        // this code is mean to work on linux need changes in window and mac
        QDir d = q.dequeue();
        TreeItem *father = qt.dequeue();

        QStringList dirs = d.entryList(QStringList(), QDir::Dirs);
        for (int i = 0; i < dirs.size(); i++) {
            if (dirs.at(i).compare(".", Qt::CaseInsensitive) == 0 || dirs.at(i).compare("..", Qt::CaseInsensitive) == 0) {
                continue;
            }
            qDebug() << dirs.at(i);

            // this code is mean to work on linux need changes in window and mac
            QString path = QString("%1/%2").arg(d.absolutePath(), dirs.at(i));
            QDir nd(path);
            qDebug() << path;

            TreeItem *dirItem = new TreeItem({dirs.at(i), tr(""), tr("")}, father, 0, path, "", FOLDER);
            father->appendChild(dirItem);

            q.enqueue(nd);
            qt.enqueue(dirItem);
        }

        QStringList files = d.entryList(QStringList(), QDir::Files);
        for (int i = 0; i < files.size(); i++) {
            QString path = QString("%1/%2").arg(d.absolutePath(), files.at(i));
            QPair<QString, TreeItemType> type = getFileType(files.at(i));
            TreeItem *dirItem = new TreeItem({files.at(i), type.first, tr("")}, father, 0, path, "", type.second);
            father->appendChild(dirItem);
        }
    }

    ui->treeView_2->setModel(new ItemModel(rootItem));
}

void MainWindow::createParentPaths(TreeItem *treeItem)
{
    TreeItem* c=treeItem;

    QStack<QDir> paths;

    while (c != nullptr) {
        QDir dir(QString("%1%2").arg(localDirectory, c->getPath()));
        if (dir.exists()) {
            break;
        } else {
            if (c->getType() == FOLDER || c->getType() == ROOT) {
                paths.push(dir);
            }
            c = c->parentItem();
        }
    }

    while (!paths.empty()) {
        QDir dir = paths.pop();
        dir.mkpath(".");
    }
}

void MainWindow::createChildrenPaths(TreeItem *treeItem)
{
    QQueue<TreeItem*> q;
    q.enqueue(treeItem);

    while (!q.empty()) {
        TreeItem *it = q.dequeue();
        QDir dir(QString("%1%2").arg(localDirectory, it->getPath()));
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        for (int i = 0; i < it->childCount(); i++) {
            TreeItem *child = it->child(i);
            if (child->getType() == FOLDER) {
                q.enqueue(child);
            }
        }
    }
}

QList<TreeItem*> MainWindow::listOfCopyItems(TreeItem *treeItem)
{
    QList<TreeItem*> res;

    if (treeItem->getType() == FOLDER) {
        QQueue<TreeItem*> q;
        q.enqueue(treeItem);

        while (!q.empty()) {
            TreeItem *it = q.dequeue();
            for (int i = 0; i < it->childCount(); i++) {
                TreeItem *child = it->child(i);
                if (child->getType() == FOLDER) {
                    q.enqueue(child);
                } else if (child->getType() == FOLDER || child->getType() == IMAGE || child->getType() == OTHER) {
                    res.append(child);
                }
            }
        }
    } else if (treeItem->getType() == VIDEO || treeItem->getType() == IMAGE || treeItem->getType() == OTHER) {
        qDebug() << treeItem;
        res.append(treeItem);
    }

    return res;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTreeChangeEvent(QJsonDocument json)
{
    qDebug() << "Tree change event";

    TreeItem *rootItem = new TreeItem({tr("Name"), tr("Type"), tr("Size")});
    QQueue<QJsonObject> q;
    QQueue<TreeItem*> qt;

    q.enqueue(json.object().value("root").toObject());
    qt.enqueue(rootItem);

    while (!q.empty()) {
        QJsonObject it = q.dequeue();
        TreeItem* father = qt.dequeue();

        QJsonArray dirs = it.value("dirs").toArray();
        if (!dirs.empty()) {
            for (int i = 0; i < dirs.size(); i++) {
                QJsonObject d = dirs.at(i).toObject();
                QString name = d.value("name").toString();
                QString path = QString("%1/%2").arg(father->getPath(), name);
                TreeItem *dirItem = new TreeItem({name, tr(""), tr("")}, father, 0, path, d.value("hash").toString(), FOLDER);
                father->appendChild(dirItem);

                q.enqueue(d);
                qt.enqueue(dirItem);
            }
        }

        QJsonArray files = it.value("files").toArray();
        if (!files.empty()) {
            for (int i = 0; i < files.size(); i++) {
                QJsonObject f = files.at(i).toObject();
                QString name = f.value("name").toString();
                QString path = QString("%1/%2").arg(father->getPath(), name);
                int size = f.value("size").toInt();
                QPair<QString, TreeItemType> type = getFileType(name);
                TreeItem *fileItem = new TreeItem({name, type.first, f.value("size").toInteger()}, father, size, path, f.value("hash").toString(), type.second);
                father->appendChild(fileItem);
            }
        }
    }

    ui->treeView->setModel(new ItemModel(rootItem));
}

void MainWindow::onCopyEvent(QJsonDocument json)
{
    qDebug() << "Copy event";
    copyThread->onCopyData(json);
}

void MainWindow::onAuthEvent(QJsonDocument json)
{
    qDebug() << "Auth event";
}

void MainWindow::on_pushButton_clicked()
{
    // copy
    QModelIndexList selectionList = ui->treeView->selectionModel()->selectedIndexes();
    if (!selectionList.empty()) {
        QModelIndex item = selectionList.at(0);
        TreeItem *treeItem = static_cast<TreeItem*>(item.internalPointer());
        qDebug() << treeItem->getPath();

        createParentPaths(treeItem);
        if (treeItem->getType() == FOLDER) {
            createChildrenPaths(treeItem);
        }

        // add copy files to list
        QList<TreeItem*> files = listOfCopyItems(treeItem);
        for (int i = 0; i < files.size(); i++) {
            // todo: check here if the files was already copied
            copyThread->appendCopy(files.at(i)->getPath(), files.at(i)->getHash(), files.at(i)->getSize());
        }

        copyThread->reanudateCopies();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    // delete
    QModelIndexList selectionList = ui->treeView->selectionModel()->selectedIndexes();
    if (!selectionList.empty()) {
        QModelIndex item = selectionList.at(0);
        TreeItem *treeItem = static_cast<TreeItem*>(item.internalPointer());
        qDebug() << treeItem->getPath();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    client->getTree();
}

QPair<QString, TreeItemType> MainWindow::getFileType(QString fileName)
{
    QStringList videos = QString("webm mkv flv vob ogv ogg drc mng avi mts m2ts mov qt wmv yuv rm rmvb viv asf amv mp4 m4p m4v mpg mp2 mpeg mpe mpv m2v svi 3gp 3g2 mxf roq nsv flv f4v f4p f4a f4b").split(" ");
    for (int i = 0; i < (int) videos.size(); i++) {
        if (fileName.endsWith(QString(".%1").arg(videos.at(i)), Qt::CaseInsensitive)) {
            return qMakePair("video", VIDEO);
        }
    }

    QStringList images = QString("jpg jpeg jfi jpe jif jfif heif heic png gif svg svgz eps webp tiff tif pnp raw arw cr rw2 nrw k25").split(" ");
    for (int i = 0; i < (int) images.size(); i++) {
        if (fileName.endsWith(QString(".%1").arg(images.at(i)), Qt::CaseInsensitive)) {
            return qMakePair("image", IMAGE);
        }
    }

    return qMakePair("unknow", OTHER);
}
