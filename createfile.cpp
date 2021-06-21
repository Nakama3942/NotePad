#include "createfile.h"
#include "ui_createfile.h"

CreateFile::CreateFile(QString * jobs, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateFile)
{
    ui->setupUi(this);

    work = *jobs; //Сохраняю название работы

    //Создаю модель системы
    model = new QFileSystemModel;
    model->setNameFilters(QStringList() << "*.txt");
    model->setNameFilterDisables(false);

    //Передаю модель в TreeView
    ui->treeFile->setModel(model);
    ui->treeFile->setRootIndex(model->index("notes"));
    ui->treeFile->setColumnWidth(0, 500);

    model->setRootPath("notes"); //Устанавливаю текущий каталог

    //Отключаю некоторые элементы, если планируется создание файла, а не открытие
    //TODO Организовать отдельное окно для создания файла (нужно, чтобы было
    //меньше путаницы в коде)
    if(work == "Создать")
    {
        ui->fileName->setDisabled(false);
        ui->fileName->setPlaceholderText("Имя файла");
        ui->buttOpen->setText(*jobs);
        ui->treeFile->setDisabled(true);
    }
}

CreateFile::~CreateFile()
{
    delete ui;
}

//При нажатии на элемент файлового дерева, сохраняю его индекс в переменную, которая
//хранит его путь, а потом вывожу его в QLabel, который отображает этот самый путь
//пользователю
void CreateFile::on_treeFile_clicked(const QModelIndex &index)
{
    path = model->filePath(index);
    ui->lineTextPath->setText(path);
}

//При нажатии на эту кнопку излучается сигнал, который передаёт адресс к файлу
//и само его название, после - закрываю это окно
void CreateFile::on_buttOpen_clicked()
{
    if(work == "Создать")
    {
        //Этот сигнал излучается, если это окно было открыто для создания файла
        emit returnPath("notes/" + ui->fileName->text() + ".txt", ui->fileName->text());
        close();
    }
    else
    {
        //Если окно открыто для открытия файла, то здесь я отделяю название файла
        //от его пути и расширения и излучаю сигнал, который возвращает полный
        //адресс и название, взятое из этого адресса
        QRegularExpression rx("[/.]"); //Указываю разделители
        filename = ui->lineTextPath->text().split(rx); //Разбиваю список на элементы
        emit returnPath(ui->lineTextPath->text(), filename.value(filename.size() - 2));
        close();
    }
}

//Этот слот реагирует не на одинарное нажатие, которое просто сохраняет адресс
//выбранного файла, а на активацию (двойное нажатие, Enter и т.д.). В результате
//этот слот выболняет функционал и выбора файла и нажатия на кнопку "Открыть" ("Создать" не
//может быть, так как в окне создание файловое дерево не активно)
void CreateFile::on_treeFile_activated(const QModelIndex &index)
{
    path = model->filePath(index);
    ui->lineTextPath->setText(path);

    QRegularExpression rx("[/.]"); //Указываю разделители
    filename = ui->lineTextPath->text().split(rx); //Разбиваю список на элементы
    emit returnPath(ui->lineTextPath->text(), filename.value(filename.size() - 2));
    close();
}

//Закрываю окно
void CreateFile::on_ButtCancel_clicked()
{
    close();
}
