#ifndef DELETEFILE_H
#define DELETEFILE_H

#include <QWidget>

#include <QFileSystemModel>
#include <QFile>

namespace Ui {
class DeleteFile;
}

class DeleteFile : public QWidget
{
    Q_OBJECT

public:
    explicit DeleteFile(QWidget *parent = nullptr);
    ~DeleteFile();

private slots:
    void on_treeFile_clicked(const QModelIndex &index);
    void on_buttRemove_clicked();
    void on_treeFile_activated(const QModelIndex &index);
    void on_buttExit_clicked();

private:
    Ui::DeleteFile *ui;
    QFileSystemModel *model;
    QString path, work;
};

#endif // DELETEFILE_H
