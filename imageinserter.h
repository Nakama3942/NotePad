#ifndef IMAGEINSERTER_H
#define IMAGEINSERTER_H

#include <QWidget>
#include <QFileSystemModel>

namespace Ui {
class ImageInserter;
}

class ImageInserter : public QWidget
{
    Q_OBJECT

public:
    explicit ImageInserter(QWidget *parent = nullptr);
    ~ImageInserter();

signals:
    void returnPath(QString pathtofile);

private slots:
    void on_treeFile_clicked(const QModelIndex &index);
    void on_buttLoad_clicked();
    void on_treeFile_activated(const QModelIndex &index);
    void on_buttcurrent_released();
    void on_butthome_released();
    void on_buttroot_released();
    void on_butttemp_released();
    void on_buttClose_clicked();

private:
    Ui::ImageInserter *ui;
    QFileSystemModel *model;
    QString path, work;
};

#endif // IMAGEINSERTER_H
