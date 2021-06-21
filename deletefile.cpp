#include "deletefile.h"
#include "ui_deletefile.h"

DeleteFile::DeleteFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeleteFile)
{
    ui->setupUi(this);

    //Аналогично окну Открытия/Создания я создаю окно удаления файлов//

    //Создаю модель системы
    model = new QFileSystemModel;
    model->setNameFilters(QStringList() << "*.txt");
    model->setNameFilterDisables(false);

    //Передаю модель в TreeView
    ui->treeFile->setModel(model);
    ui->treeFile->setRootIndex(model->index("notes"));
    ui->treeFile->setColumnWidth(0, 500);

    model->setRootPath("notes"); //Устанавливаю текущий каталог
}

DeleteFile::~DeleteFile()
{
    delete ui;
}

//Сохраняю адресс выбранного элемента
void DeleteFile::on_treeFile_clicked(const QModelIndex &index)
{
    //В отличие от открытия, здесь я просто сохраняю адресс и не вывожу его в окне
    path = model->filePath(index);
}

//Удаляю файл кнопкой
void DeleteFile::on_buttRemove_clicked()
{
    //Удаляю файл по сохранённому пути
    QFile::remove(path);
}

//Удаляю файл в файловом дереве
void DeleteFile::on_treeFile_activated(const QModelIndex &index)
{
    //Аналогично открытию, здесь я выполняю оба действия выбора файла и нажатия
    //на кнопку удалить: и сохраняю путь к файлу и сразу его удаляю
    path = model->filePath(index);
    QFile::remove(path);
}

//Закрываю окно
void DeleteFile::on_buttExit_clicked()
{
    close();
}

