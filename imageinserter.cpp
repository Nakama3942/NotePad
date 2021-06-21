#include "imageinserter.h"
#include "ui_imageinserter.h"

ImageInserter::ImageInserter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageInserter)
{
    ui->setupUi(this);

    //Аналогично окну Открытия/Создания я создаю окно загрузки картинок//

    //Создаю модель системы
    model = new QFileSystemModel;
    model->setNameFilters(QStringList() << "*.jpg" << "*.jpeg" << "*.png");
    model->setNameFilterDisables(false);

    //Передаю модель в TreeView
    ui->treeFile->setModel(model);
    ui->treeFile->setRootIndex(model->index(QDir::currentPath()));
    ui->treeFile->setColumnWidth(0, 500);

    model->setRootPath(QDir::currentPath()); //Устанавливаю текущий каталог
}

ImageInserter::~ImageInserter()
{
    delete ui;
}

//Сохраняю адресс выбранного элемента
void ImageInserter::on_treeFile_clicked(const QModelIndex &index)
{
    //В отличие от открытия, здесь я просто сохраняю адресс и не вывожу его в окне
    path = model->filePath(index);
}

//Загружаю картинку кнопкой
void ImageInserter::on_buttLoad_clicked()
{
    //Излучаю сигнал, который возвращает адресс к файлу
    emit returnPath(path);
}

//Загружаю картинку в файловом дереве
void ImageInserter::on_treeFile_activated(const QModelIndex &index)
{
    //Аналогично открытию, здесь я выполняю оба действия выбора файла и нажатия
    //на кнопку загрузить: и сохраняю путь к файлу и сразу его возвращаю
    path = model->filePath(index);
    emit returnPath(path);
}

//Устанавливаю начальную директорию для дерева каталогов
//Здесь этим каталогом будет каталог размещения исполнительного файла
void ImageInserter::on_buttcurrent_released()
{
    ui->treeFile->setRootIndex(model->index(QDir::currentPath()));
    model->setRootPath(QDir::currentPath());
}

//Устанавливаю начальную директорию для дерева каталогов
//Здесь этим каталогом будет домашний каталог пользователя
void ImageInserter::on_butthome_released()
{
    ui->treeFile->setRootIndex(model->index(QDir::homePath()));
    model->setRootPath(QDir::homePath());
}

//Устанавливаю начальную директорию для дерева каталогов
//Здесь этим каталогом будет коренвой каталог системы
void ImageInserter::on_buttroot_released()
{
    ui->treeFile->setRootIndex(model->index(QDir::rootPath()));
    model->setRootPath(QDir::rootPath());
}

//Устанавливаю начальную директорию для дерева каталогов
//Здесь этим каталогом будет каталог временных файлов системы
void ImageInserter::on_butttemp_released()
{
    ui->treeFile->setRootIndex(model->index(QDir::tempPath()));
    model->setRootPath(QDir::tempPath());
}

//Закрываю программу
void ImageInserter::on_buttClose_clicked()
{
    close();
}
