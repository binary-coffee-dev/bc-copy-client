#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QPair>
#include <QJsonDocument>
#include <QFileSystemModel>
#include <QProgressBar>
#include <QTreeWidget>
#include <QItemDelegate>
#include <QApplication>
#include <QTreeWidgetItem>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionViewItem>
#include <QFontMetrics>

#include "itemmodel.h"
#include "appsingleton.h"
#include "properties.h"
#include "filestree.h"

namespace Ui {
class MainWindow;
}

class CopyViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline CopyViewDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index ) const override
    {
        if (index.column() != 1) {
            QItemDelegate::paint(painter, option, index);
            return;
        }

        // Set up a QStyleOptionProgressBar to precisely mimic the
        // environment of a progress bar.
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.state = QStyle::State_Enabled;
        progressBarOption.direction = QApplication::layoutDirection();
        qDebug() << "--" << option.rect.x() << option.rect.y() << option.rect.width() << option.rect.height();
        progressBarOption.rect = option.rect;
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
        progressBarOption.textVisible = true;

        // Set the progress and text values of the style option.
        int progress = 50;//qobject_cast<MainWindow *>(parent())->clientForRow(index.row())->progress();
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString::asprintf("%d%%", progressBarOption.progress);

        // Draw the progress bar onto the view.
//        painter->translate(option.rect.x(), option.rect.y());
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
//        painter->resetTransform();
    }
};

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

    void updateCopiesList();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
