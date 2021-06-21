#ifndef CREATEFILE_H
#define CREATEFILE_H

#include <QWidget>

#include <QFileSystemModel>
#include <QRegularExpression>

namespace Ui {
class CreateFile;
}

class CreateFile : public QWidget
{
    Q_OBJECT

public:
    explicit CreateFile(QString *jobs, QWidget *parent = nullptr);
    ~CreateFile();

signals:
    void returnPath(QString pathtofile, QString namefile);

private slots:
    void on_treeFile_clicked(const QModelIndex &index);
    void on_buttOpen_clicked();
    void on_treeFile_activated(const QModelIndex &index);
    void on_ButtCancel_clicked();

private:
    Ui::CreateFile *ui;
    QFileSystemModel *model;
    QString path, work;
    QStringList filename;
};

#endif // CREATEFILE_H
