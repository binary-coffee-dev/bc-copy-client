#ifndef FILESTREE_H
#define FILESTREE_H

#include <QDialog>
#include <QDir>

#include "itemmodel.h"
#include "appsingleton.h"

namespace Ui {
class FilesTree;
}

class FilesTree : public QDialog
{
    Q_OBJECT

public:
    explicit FilesTree(QWidget *parent = nullptr);
    ~FilesTree();

private slots:
    void createParentPaths(TreeItem *treeItem);
    void createChildrenPaths(TreeItem *treeItem);
    QList<TreeItem*> listOfCopyItems(TreeItem *treeItem);

    void onTreeChangeEvent(QJsonDocument json);

    void on_syncTree_clicked();

    void on_copyButton_clicked();

    void on_removeButton_clicked();

    void on_FilesTree_rejected();

    void on_FilesTree_accepted();

private:
    Ui::FilesTree *ui;

    QMetaObject::Connection signalConnection;

    void loadLocalDirectory();
    QPair<QString, TreeItemType> getFileType(QString fileName);
};

#endif // FILESTREE_H
