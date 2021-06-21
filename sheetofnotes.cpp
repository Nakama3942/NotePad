#include "sheetofnotes.h"
#include "ui_sheetofnotes.h"

SheetOfNotes::SheetOfNotes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SheetOfNotes)
{
    ui->setupUi(this);
    ui->fontsize->setCurrentIndex(7); //Устанавливаю стандартный размер шрифта в ComboBox'е
    ui->fontComboBox->setCurrentFont(QFont("Arial")); //Устанавливаю стандартный шрифт
    move(0, 100); //Устанавливаю позицию, от которой изначально будет отрисовываться окно
}

SheetOfNotes::~SheetOfNotes()
{
    delete ui;
}

// // // // // // // // // // // // // // // // // // // //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////          -------------------          /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// // // // // // // // // // // // // // // // // // // //
/**********************************************************
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-              NOTE МЕНЮ ФАЙЛА               +-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *********************************************************/

/**********************************************************
 *                  NOTE БЛОК СО СЛОТАМИ                  *
 **********************************************************/

//Слот создания файла
void SheetOfNotes::on_actCreate_triggered()
{
    //Если note возвращает true, вызывается метод, создающий окно создания
    if(note("Создать новую заметку?")) winload(true);
}

//Слот открытия файла
void SheetOfNotes::on_actOpen_triggered()
{
    //Если note возвращает true, вызывается метод, создающий окно открытия
    if(note("Открыть заметку?")) winload(false);
}

//Слот сохранения текстового файла
void SheetOfNotes::on_actSaveText_triggered()
{
    //Если поле ввода активно, сохраняется текст с этого поля и
    //вызывается метод сохранения
    if(ui->winNote->isEnabled())
    {
        userNote = ui->winNote->toPlainText(); //Считываю текстовую информацию с поля ввода
        save(); //Вызываю метод сохранения
    }
    else winCrit("Нет заметки для сохранения!"); //Иначе вывожу сообщение об ошибке
}

//Слот сохранения файла в HTML
void SheetOfNotes::on_actSaveHTML_triggered()
{
    //Если поле ввода активно, сохраняется код с этого поля и
    //вызывается метод сохранения
    if(ui->winNote->isEnabled())
    {
        userNote = ui->winNote->toHtml(); //Считываю HTML информацию с поля ввода
        save(); //Вызываю метод сохранения
    }
    else winCrit("Нет заметки для сохранения!"); //Иначе вывожу сообщение об ошибке
}

//Слот сохранения и закрытия текстового файла
void SheetOfNotes::on_actSaveTextClose_triggered()
{
    closesave(true); //Вызываю метод, который сохраняет текст и потом выключает поле ввода
}

//Слот сохранения и закрытия файла в HTML
void SheetOfNotes::on_actSaveHTMLClose_triggered()
{
    closesave(false); //Вызываю метод, который сохраняет HTML и потом выключает поле ввода
}

//Кнопка закрытия файла без сохранения
void SheetOfNotes::on_actCloseNoSave_triggered()
{
    closenosave(); //Вызываю метод, который выключает поле ввода без сохранения
}

//Слот удаления текущего файла
void SheetOfNotes::on_actRemove_triggered()
{
    //Если окно открыто, формирую вопрос
    if(ui->winNote->isEnabled())
    {
        QMessageBox Quest;
        Quest.setIcon(QMessageBox::Question);
        Quest.setText("<html><head/><body><p><span style=\" font-size:12pt;\">Удалить заметку?</span></p></body></html>");
        Quest.setInformativeText("После этого будет удалён файл "
                                "со всей информацией.");
        Quest.setStandardButtons(QMessageBox::Yes |
                                QMessageBox::No);
        Quest.setDefaultButton(QMessageBox::No);
        int clearer = Quest.exec();
        switch (clearer)
        {
        case QMessageBox::Yes:
            ui->winNote->setText(""); //Очищаю поле ввода
            ui->winNote->setEnabled(false); //Делаю поле ввода неактивным
            QFile::remove(*path); //Удаляю файл
            delete(path); //Удаляю путь к файлу
            break;
        case QMessageBox::No:
            break;
        }
    }
    else winCrit("Нет заметки для удаления!"); //Иначе вывожу сообщение об ошибке
}

//Слот удаления другого файла
void SheetOfNotes::on_actRemOther_triggered()
{
    //Если поле ввода активно, спрашиваю, хочет ли пользователь продолжить
    //удаление и после положительного ответа и закрытия поля ввода открываю
    //окно удаления файлов, иначе сразу открываю окно удаления файлов
    if(ui->winNote->isEnabled())
    {
        if(note("Продолжить удаление?"))
        {
            DeleteFile *delf = new DeleteFile;
            delf->show();
        }
    }
    else
    {
        DeleteFile *delf = new DeleteFile;
        delf->show();
    }
}

//Открываю и показываю текущую заметку в файловом менеджере
void SheetOfNotes::on_actExp_triggered()
{
    //Задаю путь к файлу и активирую процес выделения файла в проводнике
    work << "/select," << QDir::toNativeSeparators(*path);
    QProcess::startDetached("explorer.exe", work);
}

//Слот выхода из программы
void SheetOfNotes::on_actCloseProg_triggered()
{
    close();
}

/**********************************************************
 *                  NOTE БЛОК С МЕТОДАМИ                  *
 **********************************************************/

//Метод, вызываемый "winload" для создания файла в директории и сохранения
//пути к нему и имени для дальнейшей работы с файлом
void SheetOfNotes::createFile(QString pathtofile, QString namefile)
{
    enableded(); //Включаю редактор шрифта

    //Сохраняю путь
    path = new QString;
    *path = pathtofile;

    //Сохраняю название файла
    filename = new QString;
    *filename = namefile;

    //Создаю файл
    QFile Note(*path);
    Note.open(QIODevice::WriteOnly | QIODevice::Text);
    Note.close();

    ui->winNote->setFontPointSize(20); //Увеличиваю начальный шрифт до размера 20
}

//Метод, вызываемый "winload" для открытия выбранного файла и считывания с него
//информации в поле редактирования текста
void SheetOfNotes::openFile(QString pathtofile, QString namefile)
{
    enableded(); //Включаю редактор шрифта

    //Сохраняю путь
    path = new QString;
    *path = pathtofile;

    //Сохраняю название файла
    filename = new QString;
    *filename = namefile;

    //Создаю файл и считываю с него информацию
    QFile Note(*path);
    if (Note.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textNote(&Note);
        userNote = textNote.readAll();
    }
    Note.close();

    ui->winNote->setFontPointSize(20); //Увеличиваю начальный шрифт до размера 20
    ui->winNote->setText(userNote); //Помещаю информацию в поле ввода
}

//Метод, вызываемый "note" для открытия окна и создания связи между окнами
void SheetOfNotes::winload(bool create)
{
    //Создаю окно и соединяю сигнал окна с методом, принимающим
    //адресс файла и его название
    if(create)
    {
        QString jobs = "Создать";
        CreateFile *winTree = new CreateFile(&jobs);
        winTree->show();
        connect(winTree, &CreateFile::returnPath, this, &SheetOfNotes::createFile);
    }
    else
    {
        QString jobs = "Открыть";
        CreateFile *winTree = new CreateFile(&jobs);
        winTree->show();
        connect(winTree, &CreateFile::returnPath, this, &SheetOfNotes::openFile);
    }
}

//Метод, запускающий окно создания/открытия файла
bool SheetOfNotes::note(QString text)
{
    //Если поле активно, формиирую сообщение
    //Если не возвращается false (пользователь не
    //отменил операцию) - возвращается true
    if(ui->winNote->isEnabled())
    {
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Question);
        Warn.setText("<html><head/><body><p><span style=\" font-size:12pt;\">" + text + "</span></p></body></html>");
        Warn.setInformativeText("<html><head/><body><p>Если Вы <span style=\" font-weight:600;\">проигнорируете</span> - текущая информация будет потеряна;</p>"
                                "<p>если <span style=\" font-weight:600;\">продолжите</span> - сохранится в текстовом формате;</p>"
                                "<p>если <span style=\" font-weight:600;\">сохраните</span> - сохранится в HTML формате.</p></body></html>");
        Warn.setStandardButtons(QMessageBox::Ignore |
                                QMessageBox::Yes |
                                QMessageBox::Save |
                                QMessageBox::Cancel);
        Warn.setDefaultButton(QMessageBox::Yes);
        int clearer = Warn.exec();
        switch (clearer)
        {
        case QMessageBox::Ignore:
            closenosave();
            break;
        case QMessageBox::Yes:
            closesave(true);
            break;
        case QMessageBox::Save:
            closesave(false);
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }
    return true;
}

//Метод, сохраняющий текст с поля в файл по сохранённому пути и вызываемый
//другими методами, когда нужно не только сохранение
void SheetOfNotes::save()
{
    //Открываю файл и помещаю в него информацию
    QFile Note(*path);
    if (Note.open(QIODevice::WriteOnly |
                  QIODevice::Text |
                  QIODevice::Truncate |
                  QIODevice::ExistingOnly))
    {
        QTextStream textNote(&Note);
        textNote << userNote << Qt::endl;
    }
    Note.close();
}

//Метод, закрывающий файл после сохранения
void SheetOfNotes::closesave(bool text)
{
    //Если поле ввода активно - сохраняю текст и выключаю редактор
    if(ui->winNote->isEnabled())
    {
        if(text)
            userNote = ui->winNote->toPlainText();
        else
            userNote = ui->winNote->toHtml();
        save(); //Сохраняю информацию
        ui->winNote->setText(""); //Очищаю поле ввода
        disableded(); //Выключаю редактор шрифта
        delete(path); //Удаляю путь к файлу
        delete(filename); //Удаляю название файла
    }
    else winCrit("Нет заметки для сохранения!"); //Иначе вызываю сообщение об ошибке
}

//Метод, закрывающий файл без сохранения
void SheetOfNotes::closenosave()
{
    //Если поле ввода активно - сразу выключаю редактор
    if(ui->winNote->isEnabled())
    {
        ui->winNote->setText(""); //Очищаю поле ввода
        disableded(); //Выключаю редактор шрифта
        delete(path); //Удаляю путь к файлу
        delete(filename); //Удаляю название файла
    }
    else winCrit("Нет заметки для сохранения!"); //Иначе вызываю сообщение об ошибке
}

//Метод, который строит окно ошибки при попытке
//сохранить файл, когда он не был открыт
void SheetOfNotes::winCrit(QString cause)
{
    QMessageBox Crit;
    Crit.setIcon(QMessageBox::Critical);
    Crit.setText("<html><head/><body><p><span style=\" font-size:12pt;\">Ошибка</span></p></body></html>");
    Crit.setInformativeText(cause);
    Crit.setStandardButtons(QMessageBox::Ok);
    Crit.setDefaultButton(QMessageBox::Ok);
    int clearer = Crit.exec();
    switch (clearer)
    {
    case QMessageBox::Ok:
        break;
    }
}

//Включаю редактор: поле ввода и элементы редактирование
void SheetOfNotes::enableded()
{
    ui->winNote->setEnabled(true); //Делаю поле ввода активным
    ui->fontComboBox->setEnabled(true); //Включаю редактор шрифта
    ui->fontsize->setEnabled(true); //Включаю редактор размера
    ui->formatgroup->setEnabled(true); //Включаю редактор формата
    ui->paragraphgroup->setEnabled(true); //Включаю редактор выравнивания
    ui->tablegroup->setEnabled(true); //Включаю редактор таблиц
    ui->imagegroup->setEnabled(true); //Включаю добавление изображений

    //Устанавливаю стандартный шрифт и его размер в панели ввода
    QFont font("Arial", 20);
    ui->winNote->setFont(font);
}

//Выключаю редактор: поле ввода и элементы редактирование
void SheetOfNotes::disableded()
{
    ui->winNote->setEnabled(false); //Делаю поле ввода неактивным
    ui->fontComboBox->setEnabled(false); //Выключаю редактор шрифта
    ui->fontsize->setEnabled(false); //Выключаю редактор размера
    ui->formatgroup->setEnabled(false); //Выключаю редактор формата
    ui->paragraphgroup->setEnabled(false); //Выключаю редактор выравнивания
    ui->tablegroup->setEnabled(false); //Выключаю редактор таблиц
    ui->imagegroup->setEnabled(false); //Выключаю добавление изображений
}

// // // // // // // // // // // // // // // // // // // //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////          -------------------          /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// // // // // // // // // // // // // // // // // // // //
/**********************************************************
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-           NOTE МЕНЮ ГЕНЕРАТОРА             +-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *********************************************************/

//Создаю вордовский файл
void SheetOfNotes::on_gen_doc_triggered()
{
    //Беру текст из поля ввода
    QString html = ui->winNote->toHtml();

    //Создаю файл и записываю в него HTML-код
    QString genfile = "documents/" + *filename + ".doc";
    QFile Doc(genfile);
    if (Doc.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream documents(&Doc);
        documents << html << Qt::endl;
    }
    Doc.close();
}

//Создаю odt-файл
void SheetOfNotes::on_gen_odt_triggered()
{
    //Беру текст из поля ввода
    QString html = ui->winNote->toHtml();

    //Создаю объект класса QTextDocument и помещаю в него текст
    QTextDocument document;
    document.setHtml(html);

    //NOTE QTextDocumentWriter - контейнер который формирует на запись формат
    //документа. Поддерживает обычный текст, HTML, Markdown, ODF.

    //ODF - Open Documents Format. Вот он та мне и нужен. Он используется в
    //LibreOffice и это формат нужен для реализации трьох методов генерации
    //pdf-ки (есть, идёт следующим, реализован по другому), генерации вордовских
    //файлов (есть, предшествует, реализован по другому) и для генерации файлов
    //этого формата (реализовать можно только так).
    QString genfile = "documents/" + *filename + ".odt";
    QTextDocumentWriter odtshka(genfile);
    odtshka.setFormat("odf");
    odtshka.write(&document);
}

//Создаю pdf-файл
void SheetOfNotes::on_gen_pdf_triggered()
{
    //Формирую дату для pdf-ки
    QDateTime TimeForRecording(QDateTime::currentDateTime());
    Date = TimeForRecording.date().toString();
    Time = TimeForRecording.time().toString();
    TimeDate = Time + " " + Date;

    //Беру текст из поля ввода в формате HTML и сохраняю в строку вместе со временем
    QString html = "<div align=right><span style=\" font-size:14pt;\">" + TimeDate + "</span></div><p>" + ui->winNote->toHtml() + "</p>";

/*
 * Из примера, как это делается, откуда я брал код:
 *
 *  QString html = "<div align=right>"
 *     "City, 11/11/2015"
 *  "</div>"
 *  "<div align=left>"
 *     "Sender Name<br>"
 *     "street 34/56A<br>"
 *     "121-43 city"
 *  "</div>"
 *  "<h1 align=center>DOCUMENT TITLE</h1>"
 *  "<p align=justify>"
 *     "document content document content document content document content document content document content document content document content document content document content "
 *     "document content document content document content document content document content document content document content document content document content document content "
 *  "</p>"
 *  "<div align=right>sincerly</div>";
 */

    //NOTE QTextDocument - это контейнер для структурированных документов с
    //форматированным текстом, обеспечивающий поддержку стилизованного текста и
    //различных типов элементов документа, таких как списки, таблицы, фреймы и
    //изображения. Они могут быть созданы для использования в QTextEdit или
    //использоваться независимо.

    //Создаю объект класса QTextDocument и помещаю в него текст
    QTextDocument document;
    document.setHtml(html);

    //NOTE Это устройство представляет собой серию страниц печатного вывода и
    //используется почти так же, как другие устройства рисования, такие
    //как QWidget и QPixmap . Предоставляется набор дополнительных функций для
    //управления особенностями устройства, такими как ориентация и разрешение, а
    //также для перехода по страницам в документе по мере его создания.

    //Устанавливаю формат документа
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    QString genfile = "documents/" + *filename + ".pdf";
    printer.setOutputFileName(genfile);
    printer.setPageMargins(QMarginsF(2, 2, 2, 2));

    //Создаю документ
    document.print(&printer);
}

// // // // // // // // // // // // // // // // // // // //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////          -------------------          /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// // // // // // // // // // // // // // // // // // // //
/**********************************************************
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-             NOTE МЕНЮ ПРАВКИ               +-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *********************************************************/

//Отменить
void SheetOfNotes::on_actUndo_triggered() { ui->winNote->undo(); }

//Вернуть
void SheetOfNotes::on_actRedo_triggered() { ui->winNote->redo(); }

//Вырезать
void SheetOfNotes::on_actCut_triggered() { ui->winNote->cut(); }

//Копировать
void SheetOfNotes::on_actCopy_triggered() { ui->winNote->copy(); }

//Вставить
void SheetOfNotes::on_actPaste_triggered() { ui->winNote->paste(); }

//Очистить
void SheetOfNotes::on_actClear_triggered() { ui->winNote->clear(); }

//Слот вставляет в текущем положении курсора текущие время и дату
void SheetOfNotes::on_timedate_triggered()
{
    //Беру время и дату
    QDateTime TimeForRecording(QDateTime::currentDateTime());

    //Перевожу время и дату в строку
    Date = TimeForRecording.date().toString();
    Time = TimeForRecording.time().toString();
    TimeDate = Time + " " + Date;

    //Сохраняю строку в бувер обмена
    QApplication::clipboard()->setText(TimeDate);

    //Вставляю из буфера обмена строку в текущее положение курсора
    ui->winNote->paste();
}

// // // // // // // // // // // // // // // // // // // //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////          -------------------          /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// // // // // // // // // // // // // // // // // // // //
/**********************************************************
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-      NOTE ЭЛЕМЕНТЫ НАСТРОЙКИ ШРИФТА        +-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *********************************************************/

/**********************************************************
 * NOTE БЛОК СО СЛОТАМИ
 *********************************************************/

//Устанавливаю шрифт текста
void SheetOfNotes::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->winNote->setFont(f);
}

//Устанавливаю размер текста
void SheetOfNotes::on_fontsize_activated(int index)
{
    //Беру индекс (0, 1, 2, 3...)
    //Умножаю на 2 (0, 2, 4, 6...)
    //Умножаю на 2, так как разница между элементыми равна двум
    //Прибавляю 6 (6, 8, 10, 12...)
    //Прибавляю 6, так как размеры начинаются с шестёрки
    //Устанавливаю размер в поле ввода
    // NOTE Если изменится первый размер в ComboBox'е, здесь вместо шестёрки
    // нужно указать тот элемент, который будет находиться на том месте вместо шестёрки
    ui->winNote->setFontPointSize((index * 2) + 6);
}

//Активирую жирный шрифт
void SheetOfNotes::on_bold_toggled(bool checked)
{
    //Если галочка активна - установить жирный формат, иначе - снять
    if(checked) ui->winNote->setFontWeight(QFont::Bold);
    else ui->winNote->setFontWeight(0);
    // NOTE Когда пользователь нажимает на поле ввода, галочки
    // сбрасываются, поэтому (чтобы два раза не нажимать на галочку), пользователю
    // можно будет нажать на кнопку "очистить формат", которая уже имеет команды
    // из else-операторов всех галочек управления форматом
}

//Активирую курсивный шрифт
void SheetOfNotes::on_italicfont_toggled(bool checked)
{
    //Если галочка активна - установить курсивный формат, иначе - снять
    if(checked) ui->winNote->setFontItalic(true);
    else ui->winNote->setFontItalic(false);
    // NOTE Когда пользователь нажимает на поле ввода, галочки
    // сбрасываются, поэтому (чтобы два раза не нажимать на галочку), пользователю
    // можно будет нажать на кнопку "очистить формат", которая уже имеет команды
    // из else-операторов всех галочек управления форматом
}

//Активирую подчёркнутый шрифт
void SheetOfNotes::on_underline_toggled(bool checked)
{
    //Если галочка активна - установить подчёркивание, иначе - снять
    if(checked) ui->winNote->setFontUnderline(true);
    else ui->winNote->setFontUnderline(false);
    // NOTE Когда пользователь нажимает на поле ввода, галочки
    // сбрасываются, поэтому (чтобы два раза не нажимать на галочку), пользователю
    // можно будет нажать на кнопку "очистить формат", которая уже имеет команды
    // из else-операторов всех галочек управления форматом
}

//Очищаю формат выделенного текста, так как при изменении курсора галочки формата
//сбрасываются и для выключения формата нужно дважды включать данный формат:
//заново поставить а потом снять галочку, чтобы снялся формат. Что бы пользователь
//с эти не заморачивался - добавлена эта кнопка: она снимает весь формат с
//выделенного текста, а галочкой можно будет снять формат, если пользователь
//случайно нажел на данную галочку
void SheetOfNotes::on_clearformat_released()
{
    ui->winNote->setFontWeight(0);
    ui->winNote->setFontItalic(false);
    ui->winNote->setFontUnderline(false);
}

//Включаю выравнивание по левой стороне
void SheetOfNotes::on_alignL_released()
{
    ui->winNote->setAlignment(Qt::AlignLeft);
}

//Включаю выравнивание по центру
void SheetOfNotes::on_alignC_released()
{
    ui->winNote->setAlignment(Qt::AlignCenter);
}

//Включаю выравнивание по правой стороне
void SheetOfNotes::on_alignR_released()
{
    ui->winNote->setAlignment(Qt::AlignRight);
}

//Включаю выравнивание по ширне
void SheetOfNotes::on_alignJ_released()
{
    ui->winNote->setAlignment(Qt::AlignJustify);
}

//Создаю таблицу
void SheetOfNotes::on_buttaccept_released()
{
    //Беру текущее местоположение курсора
    QTextCursor currentTable = ui->winNote->textCursor();

    //По нему создаю таблицу с размерами, которые
    //пользователь указал возле кнопки "создать таблицу"
    currentTable.insertTable(ui->spinrow->value(), ui->spincolumn->value());

    //В текущий курсор поля ввода вставляю курсор с таблицей
    ui->winNote->setTextCursor(currentTable);
}

//Загружаю картинку
void SheetOfNotes::on_buttloadimage_released()
{
    //Создаю окно и соединяю сигнал окна с методом, принимающим адресс картинки
    ImageInserter *ImIn = new ImageInserter;
    ImIn->show();
    connect(ImIn, &ImageInserter::returnPath, this, &SheetOfNotes::loadImage);
}

//Снимаю ВСЕ галочки форматирования при изменении положения курсора в поле ввода
void SheetOfNotes::on_winNote_cursorPositionChanged()
{
    //Достаточно просто нажать на редактор
    ui->bold->setChecked(false);
    ui->italicfont->setChecked(false);
    ui->underline->setChecked(false);
    ui->spinrow->setValue(0);
    ui->spincolumn->setValue(0);
}

/**********************************************************
 * NOTE БЛОК С МЕТОДАМИ
 *********************************************************/

//Метод, вставляющий картинку
void SheetOfNotes::loadImage(QString pathtoFile)
{
    //Аналогично таблице, беру текущее местоположение курсора, по адресу
    //беру картинку и вставляю в курсор и передаю в текущий
    //курсор - курсор с картинкой
    QTextCursor currentTable = ui->winNote->textCursor();
    currentTable.insertImage(pathtoFile);
    ui->winNote->setTextCursor(currentTable);
}

// // // // // // // // // // // // // // // // // // // //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////          -------------------          /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// // // // // // // // // // // // // // // // // // // //
/**********************************************************
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-          NOTE МЕНЮ ВЫБОРА ЯЗЫКА            +-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *********************************************************/

//Выбор русского языка
void SheetOfNotes::on_actRus_triggered()
{
    //Если загрузка успешна - устанавливаю перевод, а после - перевожу интерфейс
    if(qtLanguageTranslator.load("languages/NotePad_ru_RU.qm"))
        qApp->installTranslator(&qtLanguageTranslator);
    ui->retranslateUi(this);
}

//Выбор английского языка
void SheetOfNotes::on_actEng_triggered()
{
    //Если загрузка успешна - устанавливаю перевод, а после - перевожу интерфейс
    if(qtLanguageTranslator.load("languages/NotePad_en_US.qm"))
        qApp->installTranslator(&qtLanguageTranslator);
    ui->retranslateUi(this);
}

// // // // // // // // // // // // // // // // // // // //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////          -------------------          /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// // // // // // // // // // // // // // // // // // // //
/**********************************************************
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-           NOTE МЕНЮ ДОКУМЕНТАЦИИ           +-+-+-
 * +-+-+-                                            +-+-+-
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *********************************************************/

//Открытие окна с информацией про программу
void SheetOfNotes::on_aboutProg_triggered()
{
    aboutProg *abProg = new aboutProg;
    abProg->show();
}

//Открытие окна с информацией про Qt
void SheetOfNotes::on_aboutQt_triggered()
{
    QMessageBox::aboutQt(this, "Про Qt");
}

//-------------------------------------------------------//

//Место для следующих слотов, которые будут выполнять функционал следующих
//обновлений и буду реализованы, но пока неизвестно, что будет добавлено
//и как оно должно располагаться в коде, поэтому слоты будут добавляться здесь
//и уже потом получать своё место

