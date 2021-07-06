/* ****************************************************************************
 * -------------------------------------------------------------------------- *
 *                                                                            *
 * Copyright © 2021 Kalynovsky Valentin. All rights reserved.                 *
 * Licensed under the Apache License, Version 2.0                             *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * ************************************************************************** */

#include "sheetofnotes.h"
#include "ui_sheetofnotes.h"

SheetOfNotes::SheetOfNotes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SheetOfNotes)
{
    ui->setupUi(this);
    ui->fontsize->setValue(20); //Устанавливаю стандартный размер шрифта в combobox
    ui->fontComboBox->setCurrentFont(QFont("Arial")); //Устанавливаю стандартный шрифт
    ui->spinparagraphindent->setValue(10); //Устанавливаю отступ абзаца в spinbox
    ui->spinlineheight->setValue(0); //Устанавливаю межстрочное расстояние в spinbox

    //isFirstTime = true; метод для старого функционала

    //Передаю в объект документ
    m_highlighter = new highlight_search(ui->winNote->document());

    move(0, 40); //Устанавливаю позицию, от которой изначально будет отрисовываться окно
}

SheetOfNotes::~SheetOfNotes()
{
    delete ui;
}

void SheetOfNotes::contextMenuEvent(QContextMenuEvent *event)
{
    //Создаю пустой объект класа QMenu
    QMenu *texteditMenu = new QMenu(nullptr);

    //Если активно поле ввода - доступно и контекстное меню
    if(ui->winNote->isEnabled())
    {
        //Добавляю новые элементы меню
        texteditMenu->addAction(ui->actUndo);
        texteditMenu->addAction(ui->actRedo);
        texteditMenu->addSeparator();
        texteditMenu->addAction(ui->actCut);
        texteditMenu->addAction(ui->actCopy);
        texteditMenu->addAction(ui->actPaste);
        texteditMenu->addSeparator();
        texteditMenu->addAction(ui->actHighlightall);
        texteditMenu->addSeparator();
        texteditMenu->addAction(ui->actClear);
        texteditMenu->addSeparator();
        texteditMenu->addAction(ui->timedate);
        texteditMenu->addAction(ui->loadimage);
    }

    //Вывожу меню при нажатии правой кнопкой мыши
    texteditMenu->exec(event->globalPos());
}

// // // // // // // // // // // // // // // // // // // //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////          -------------------          /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////                 SLOTS                 /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////          -------------------          /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// // // // // // // // // // // // // // // // // // // //

// -+-+- // -+-+- // -+-+- //  // -+-+- //-+-+- // -+-+- //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
//             -----СЛОТЫ ЭЛЕМЕНТОВ МЕНЮ-----            //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// -+-+- // -+-+- // -+-+- //  // -+-+- //-+-+- // -+-+- //

// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////               МЕНЮ ФАЙЛА              /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //

//Слот создания файла
void SheetOfNotes::on_actCreate_triggered()
{
    //WARNING Плохо протестировано
    QString nameNewNote;
    bool ok = false, contin = false;

    if(ui->winNote->isEnabled()) contin = note(tr("Создать новую заметку?"));

    if(contin)
        nameNewNote = QInputDialog::getText(this,
                                            tr("Создать заметку"),
                                            tr("Введите название новой заметки"),
                                            QLineEdit::Normal,
                                            "", &ok);

    //Если note возвращает true, вызывается метод, создающий окно создания
    if(ok) createFile("notices/notes/" + nameNewNote + ".txt", nameNewNote);
}

//Слот создания файла кода
void SheetOfNotes::on_actCreatecode_triggered()
{
    //WARNING Плохо протестировано
    QString nameNewNote;
    bool ok = false, contin = false;

    if(ui->winNote->isEnabled()) contin = note(tr("Создать новый код?"));

    if(contin)
        nameNewNote = QInputDialog::getText(this,
                                            tr("Новый код"),
                                            tr("Введите название нового кода"),
                                            QLineEdit::Normal,
                                            "", &ok);

    //Если note возвращает true, вызывается метод, создающий окно создания
    if(ok) createFile("notices/codes/" + nameNewNote + ".cpp", nameNewNote);

    //Переключаю на подходящийрежим полк ввода
    ui->viewcode->setChecked(true);
}

//Слот добавления файла
void SheetOfNotes::on_actAdd_triggered()
{
    //Отображаю окно, где пользователь выбирает текстовый файл
    QString nameNote =nullptr;
    nameNote = QFileDialog::getOpenFileName(this,
                                            tr("Добавить заметку"),
                                            QDir::homePath(),
                                            tr("Text files (*.txt)"));

    //Если пользователь выбрал файл, тогда:
    if(nameNote != nullptr)
    {
        //Достаю название самого файла:
        QRegularExpression rx("[/.]"); //Указываю разделители
        QStringList nameFile = nameNote.split(rx); //Разбиваю список на элементы

        //Открываю тот файл и считываю данные
        QFile otherNotice(nameNote);
        if (otherNotice.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream textNote(&otherNotice);
            userNote = textNote.readAll();
        }
        otherNotice.close();

        //Создаю новый файл
        createFile("notices/notes/" + nameFile[nameFile.size() - 2] + ".txt" ,
                   nameFile[nameFile.size() - 2]);

        //Размещаю в редакторе считанную информацию
        ui->winNote->setText(userNote);

        ui->winNote->moveCursor(QTextCursor::End); //Устанавливаю курсор в конец файла
    }
}

//Слот открытия файла
void SheetOfNotes::on_actOpen_triggered()
{
    //WARNING Плохо протестировано
    bool contin = false;

    if(ui->winNote->isEnabled()) contin = note(tr("Открыть заметку?"));

    if(contin || !ui->winNote->isEnabled())
    {
        //Отображаю окно, где пользователь выбирает текстовый файл
        QString nameNote =nullptr;
        nameNote = QFileDialog::getOpenFileName(this,
                                                tr("Открыть заметку"),
                                                "notices/notes/",
                                                tr("Text files (*.txt)"));

        QRegularExpression rx("[/.]"); //Указываю разделители
        QStringList namenote = nameNote.split(rx); //Разбиваю список на элементы

        //Если note возвращает true, вызывается метод, создающий окно открытия
        if(nameNote != nullptr)
            openFile(nameNote, namenote.value(namenote.size() - 2));
    }
}

//Слот для открытия кода
void SheetOfNotes::on_actOpencode_triggered()
{
    //WARNING Плохо протестировано
    bool contin = false;

    if(ui->winNote->isEnabled()) contin = note(tr("Открыть код?"));

    if(contin || !ui->winNote->isEnabled())
    {
        //Отображаю окно, где пользователь выбирает текстовый файл
        QString nameCode = nullptr;
        nameCode = QFileDialog::getOpenFileName(this,
                                                tr("Открыть код"),
                                                "notices/codes/",
                                                tr("Text files (*.cpp)"));

        QRegularExpression rx("[/.]"); //Указываю разделители
        QStringList namecode = nameCode.split(rx); //Разбиваю список на элементы

        //Если note возвращает true, вызывается метод, создающий окно открытия
        if(nameCode != nullptr)
            openFile(nameCode, namecode.value(namecode.size() - 2));

        //Переключаю на подходящийрежим полк ввода
        ui->viewcode->setChecked(true);
        on_viewcode_released();
    }
}

//Слот открытия бинарного файла
void SheetOfNotes::on_actOpenbin_triggered()
{
    //Отображаю окно, где пользователь выбирает бинарный файл
    QString binName = nullptr;
    binName = QFileDialog::getOpenFileName(this,
                                           tr("Открыть бинарный файл"),
                                           "notices/binarys/",
                                           tr("Binary files (*.bin)"));

    //Создаю строку, которая будет хранить бинарный код
    QString stringbin;

    //Если файл выбран:
    if(binName != nullptr)
    {
        //создаю объект файла
        QFile bin(binName);

        //Если файл открыт
        if(bin.open(QIODevice::ReadOnly))
        {
            //Считываю бинарный поток и помещаю в файл
            QDataStream binstream(&bin);
            binstream >> stringbin;
        }
        bin.close();

        //Разделяю путь, чтобы достать само название конкретного файла
        QRegularExpression rx("[/.]"); //Указываю разделители
        QStringList binname = binName.split(rx); //Разделяю путь

        //Создаю новую заметку под считывающуюся информацию
        createFile("notices/notes/" + binname.value(binname.size() - 2) + ".txt",
                   binname.value(binname.size() - 2));

        //Помещаю считанную информацию в заметку
        ui->winNote->setText(stringbin);

        //Перемещаю курсор в конец файла
        ui->winNote->moveCursor(QTextCursor::End); //Устанавливаю курсор в конец файла
    }
}

//Слот сохранения текстового файла
void SheetOfNotes::on_actSave_triggered()
{
    //Если поле ввода активно, сохраняется текст с этого поля и
    //вызывается метод сохранения
    if(ui->winNote->isEnabled())
    {
        userNote = ui->winNote->toPlainText(); //Считываю текстовую информацию с поля ввода
        save(); //Вызываю метод сохранения
    }
    else winCrit(tr("Нет файла для сохранения!")); //Иначе вывожу сообщение об ошибке
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
    else winCrit(tr("Нет заметки для сохранения!")); //Иначе вывожу сообщение об ошибке
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
        Quest.setText(tr("<html><head/><body><p><span style=\" font-size:12pt;\">Удалить заметку?</span></p></body></html>"));
        Quest.setInformativeText(tr("После этого будет удалён файл "
                                    "со всей информацией."));
        Quest.setStandardButtons(QMessageBox::Yes |
                                 QMessageBox::No);
        Quest.setDefaultButton(QMessageBox::No);
        int clearer = Quest.exec();
        switch (clearer)
        {
        case QMessageBox::Yes:
            ui->winNote->setText(""); //Очищаю поле ввода
            disableded(); //Выключаю редактор текста
            QFile::remove(*path); //Удаляю файл
            delete(path); //Удаляю путь к файлу
            break;
        case QMessageBox::No:
            break;
        }
    }
    else winCrit(tr("Нет заметки для удаления!")); //Иначе вывожу сообщение об ошибке
}

//Слот удаления другого файла
void SheetOfNotes::on_actRemOther_triggered()
{
    //Если поле ввода активно, спрашиваю, хочет ли пользователь продолжить
    //удаление и после положительного ответа и закрытия поля ввода открываю
    //окно удаления файлов, иначе сразу открываю окно удаления файлов
    if(ui->winNote->isEnabled())
    {
        if(note(tr("Продолжить удаление?"))) deleteFile();
    }
    else deleteFile();

}

//Открываю и показываю текущую заметку в файловом менеджере
void SheetOfNotes::on_actExpFile_triggered()
{
    //Задаю путь к файлу и активирую процес выделения файла в проводнике
    if(ui->winNote->isEnabled())
    {
        QStringList work;
        work << "/select," << QDir::toNativeSeparators(*path);
        QProcess::startDetached("explorer.exe", work);
    }
}

//Открываю директорию, где хранятся файлы пользователя (заметки, архивы и т.д.)
void SheetOfNotes::on_actExpDir_triggered()
{
    //Открываю файловую директорию программы в проводнике
    QProcess::startDetached("explorer.exe",
                            QStringList() << QDir::toNativeSeparators("notices/"));
}

//Слот выхода из программы
void SheetOfNotes::on_actCloseProg_triggered()
{
    close();
}

// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////             МЕНЮ ЭКСПОРТА             /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //

//Создаю вордовский файл
void SheetOfNotes::on_gen_doc_triggered()
{
    //Беру текст из поля ввода
    QString html = ui->winNote->toHtml();

    //Создаю файл и записываю в него HTML-код
    QString genfile = "notices/documents/" + *filename + ".doc";
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

    //QTextDocumentWriter - контейнер который формирует на запись формат
    //документа. Поддерживает обычный текст, HTML, Markdown, ODF.

    //ODF - Open Documents Format. Вот он та мне и нужен. Он используется в
    //LibreOffice и это формат нужен для реализации трьох методов генерации
    //pdf-ки (есть, идёт следующим, реализован по другому), генерации вордовских
    //файлов (есть, предшествует, реализован по другому) и для генерации файлов
    //этого формата (реализовать можно только так).
    QString genfile = "notices/documents/" + *filename + ".odt";
    QTextDocumentWriter odtshka(genfile);
    odtshka.setFormat("odf");
    odtshka.write(&document);
}

//Создаю pdf-файл
void SheetOfNotes::on_gen_pdf_triggered()
{
    //Беру текст из поля ввода в формате HTML и сохраняю в строку
    QString html = ui->winNote->toHtml();

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

    //QTextDocument - это контейнер для структурированных документов с
    //форматированным текстом, обеспечивающий поддержку стилизованного текста и
    //различных типов элементов документа, таких как списки, таблицы, фреймы и
    //изображения. Они могут быть созданы для использования в QTextEdit или
    //использоваться независимо.

    //Создаю объект класса QTextDocument и помещаю в него текст
    QTextDocument document;
    document.setHtml(html);

    //Это устройство представляет собой серию страниц печатного вывода и
    //используется почти так же, как другие устройства рисования, такие
    //как QWidget и QPixmap . Предоставляется набор дополнительных функций для
    //управления особенностями устройства, такими как ориентация и разрешение, а
    //также для перехода по страницам в документе по мере его создания.

    //Устанавливаю формат документа
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    QString genfile = "notices/documents/" + *filename + ".pdf";
    printer.setOutputFileName(genfile);
    printer.setPageMargins(QMarginsF(ui->leftmarginspinbox->value(),
                                     ui->upmarginspinbox->value(),
                                     ui->rightmarginspinbox->value(),
                                     ui->downmarginspinbox->value()));

    //Создаю документ
    document.print(&printer);
}

//Создаю zip-файл
void SheetOfNotes::on_gen_zip_triggered()
{
    //Отображаю окно, где пользователь выбирает архиватор
    QString nameExec =nullptr;
    nameExec = QFileDialog::getOpenFileName(this,
                                            tr("Выберите архиватор"),
                                            QDir::rootPath(),
                                            tr("Executable (*.exe)"));

    //Если пользователь выбрал файл, тогда:
    if(nameExec != nullptr)
    {
        //Запускаю процесс архивирования
        QProcess archivator;

        //Пережаю в процесс выбранную программу и аргументы
        //Аргументы заточены под WinRAR и выполняют:
        //Создаю архив, добавляют дату и время архивации,
        //Удаляют исходную заметку, делают архивирования максимального
        //пятого уровня сжатия, сохдают архив в определённой директории
        //и из определённого заданного файла
        archivator.startDetached(nameExec, QStringList() << "a"
                                                         << "-ag"
                                                         << "-df"
                                                         << "-m5"
                                                         << "notices/archives/" + *filename + ".zip"
                                                         << "notices/notes/" + *filename + ".txt");

        //Ожидаю, пока программа запустится, иначе процесс прерывает
        //сама пограмма, не дождавшись ответа запуска архиватора
        archivator.waitForFinished(-1);
    }
}

//Создаю bin-файл
void SheetOfNotes::on_gen_bin_triggered()
{
    //Создаю объект бинарного файла
    QFile bin("notices/binarys/" + *filename + ".bin");
    if(bin.open(QIODevice::WriteOnly))
    {
        //Если файл открыт - помещаю текст в файл
        QDataStream binstream(&bin);
        userNote = ui->winNote->toHtml();
        binstream << userNote;
    }
    bin.close();

    //Удаляю заметку
    QFile::remove(*path);

    //Выключаю элементы редактора
    closenosave();
}

// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////              МЕНЮ ПРАВКИ              /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //

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

//Выделить всё
void SheetOfNotes::on_actHighlightall_triggered()
{
    //QTextCursor также имеет anchor() позицию. Текст, который находится
    //между anchor() и position(), является выделенным.
    //Попробуем это использовать

    //Беру текущий курсор
    QTextCursor highlightAll = ui->winNote->textCursor();

    //Устанавливаю позицию в начало документа с переносом привязки
    highlightAll.setPosition(0, QTextCursor::MoveAnchor);

    //Двигаю позицию в конец файла без переноса привязки
    highlightAll.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

    //Устанавливаю курсор
    ui->winNote->setTextCursor(highlightAll);

    //Так как разница между привязкой и текущей позицией составляет
    //весь документ - курсор возвращает выделение всего документа
}

//Очистить
void SheetOfNotes::on_actClear_triggered() { ui->winNote->clear(); }

// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////              МЕНЮ ВСТАВКИ             /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //

//Слот вставляет в текущем положении курсора текущие время и дату
void SheetOfNotes::on_timedate_triggered()
{
    //Беру время и дату
    QDateTime TimeForRecording(QDateTime::currentDateTime());

    //Перевожу время и дату в строку
    QString Date = TimeForRecording.date().toString(),
            Time = TimeForRecording.time().toString(),
            TimeDate = Time + " " + Date;

    //Сохраняю строку в буфер обмена
    QApplication::clipboard()->setText(TimeDate);

    //Вставляю из буфера обмена строку в текущее положение курсора
    ui->winNote->paste();
}

//Загружаю картинку
void SheetOfNotes::on_loadimage_triggered()
{
    //Отображаю окно, где пользователь выбирает картинки
    QStringList namesImages = QFileDialog::getOpenFileNames(this,
                                                     tr("Загрузить картинку"),
                                                     QDir::homePath(),
                                                     tr("Images (*.png *.jpeg *.jpg)"));

    for (int i = 0; i < namesImages.size(); i++) {
        //Аналогично таблице, беру текущее местоположение курсора, по адресу
        //беру картинку, вставляю в курсор и передаю в текущий
        //курсор - курсор с картинкой
        QTextCursor currentImage = ui->winNote->textCursor();
        currentImage.insertImage(namesImages[i]);
        ui->winNote->setTextCursor(currentImage);
    }
}

// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////           МЕНЮ ВЫБОРА ЯЗЫКА           /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //

//Выбор английского языка (США)
void SheetOfNotes::on_actEngUS_triggered()
{
    //Если загрузка успешна - устанавливаю перевод, а после - перевожу интерфейс
    if(qtLanguageTranslator.load("languages/NotePad_en_US.qm"))
        qApp->installTranslator(&qtLanguageTranslator);
    ui->retranslateUi(this);
}

//Выбор японского языка
void SheetOfNotes::on_actJap_triggered()
{
    //Если загрузка успешна - устанавливаю перевод, а после - перевожу интерфейс
    if(qtLanguageTranslator.load("languages/NotePad_jp_JP.qm"))
        qApp->installTranslator(&qtLanguageTranslator);
    ui->retranslateUi(this);
}

//Выбор русского языка (Россия)
void SheetOfNotes::on_actRusRU_triggered()
{
    //Если загрузка успешна - устанавливаю перевод, а после - перевожу интерфейс
    if(qtLanguageTranslator.load("languages/NotePad_ru_RU.qm"))
        qApp->installTranslator(&qtLanguageTranslator);
    ui->retranslateUi(this);
}

//Выбор русского языка (Украина)
void SheetOfNotes::on_actRusUA_triggered()
{
    //Если загрузка успешна - устанавливаю перевод, а после - перевожу интерфейс
    if(qtLanguageTranslator.load("languages/NotePad_ru_UA.qm"))
        qApp->installTranslator(&qtLanguageTranslator);
    ui->retranslateUi(this);
}

//Выбор украинского языка
void SheetOfNotes::on_actUkr_triggered()
{
    //Если загрузка успешна - устанавливаю перевод, а после - перевожу интерфейс
    if(qtLanguageTranslator.load("languages/NotePad_ua_UA.qm"))
        qApp->installTranslator(&qtLanguageTranslator);
    ui->retranslateUi(this);
}

// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////           МЕНЮ ДОКУМЕНТАЦИИ           /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //

//Открытие окна с информацией про программу
void SheetOfNotes::on_aboutProg_triggered()
{
    QMessageBox Inform;
    Inform.setIcon(QMessageBox::Information);
    Inform.setText(tr("<html><head/><body><p><span style=\" font-size:12pt;\">Про программу</span></p></body></html>"));
    Inform.setInformativeText(tr("<html><head/><body><p><span style=\" font-size:11pt; font-style:italic;\">"
                            "А я всё продолжаю работать над своими проектиками... Сначала был CommPay, потом"
                            "FormWord, BDU (Blanks Documents of University), а теперь и NotePad. Этот проект"
                            "являет из себя аналог обычного блокнота, но с моим видением, каким он должен"
                            "быть... Этот проект я делал чисто ради того, чтобы научиться делать окна с файлами"
                            "системы и для увеличения своей коллекции программ.</span></p></body></html>"));
    Inform.setStandardButtons(QMessageBox::Ok);
    Inform.setDefaultButton(QMessageBox::Ok);
    int clearer = Inform.exec();
    switch (clearer) { case QMessageBox::Ok: break; }
}

//Открываю ссайт в браузере
void SheetOfNotes::on_CODEPROG_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Nakama3942/NotePad"));
}

//Открытие окна с информацией про Qt
void SheetOfNotes::on_aboutQt_triggered()
{
    QMessageBox::aboutQt(this, tr("Про Qt"));
}

// -+-+- // -+-+- // -+-+- //  // -+-+- //-+-+- // -+-+- //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
//    -----СЛОТЫ ЭЛЕМЕНТОВ ПОЛЬЗОВАТЕЛЬСКОГО ОКНА-----   //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// -+-+- // -+-+- // -+-+- //  // -+-+- //-+-+- // -+-+- //

// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ////              ----ПОЛЕ ВВОДА----             //// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //

//Каждый раз, при любом изменении курсора, обновляю tabgroup
void SheetOfNotes::on_winNote_cursorPositionChanged()
{
    loadOption(); //Считываю данные с файла и помещаю в свои комбобоксы
}

// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ////    ----ЭЛЕМЕНТЫ РЕДАКТИРОВАНИЯ ТЕКСТА----   //// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //

//Устанавливаю шрифт текста
void SheetOfNotes::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QFont font(f); //Создаю шрифт
    font.setPointSize(ui->fontsize->value()); //Устанавливаю размер, иначе сбивается
    ui->winNote->setCurrentFont(font); //Устанавливаю шрифт
}

//Устанавливаю размер текста
void SheetOfNotes::on_fontsize_valueChanged(int index)
{
    ui->winNote->setFontPointSize(index);
}

//Включаю выравнивание по левой стороне
void SheetOfNotes::on_alignL_released()
{
    ui->winNote->setAlignment(Qt::AlignLeft);
}

//Включаю выравнивание по центру
void SheetOfNotes::on_alignC_released()
{
    ui->winNote->setAlignment(Qt::AlignHCenter);
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

//Активирую жирный шрифт
void SheetOfNotes::on_boldf_released()
{
    //Если текст не жирный - сделать жирным, иначе - не жирным
    if(ui->winNote->fontWeight() != QFont::Bold) ui->winNote->setFontWeight(QFont::Bold);
    else ui->winNote->setFontWeight(0);
}

//Активирую курсивный шрифт
void SheetOfNotes::on_italicf_released()
{
    //Если текст не курсивный - сделать курсивным, иначе - не курсивным
    if(!ui->winNote->fontItalic()) ui->winNote->setFontItalic(true);
    else ui->winNote->setFontItalic(false);
}

//Активирую подчёркнутый шрифт
void SheetOfNotes::on_underlinef_released()
{
    //Если текст не подчёркнут - подчеркнуть, иначе - убрать подчёркивание
    if(!ui->winNote->fontUnderline()) ui->winNote->setFontUnderline(true);
    else ui->winNote->setFontUnderline(false);
}

//Активирую зачёркнутый шрифт
void SheetOfNotes::on_strikeoutf_released()
{
    //Беру текущий формат выделенного текста
    QTextCharFormat form = ui->winNote->currentCharFormat();

    //Если текст не зачёркнут - зачеркнуть, иначе - убрать зачёркивание
    if(!form.fontStrikeOut()) form.setFontStrikeOut(true);
    else form.setFontStrikeOut(false);

    //устанавливаю формат
    ui->winNote->setCurrentCharFormat(form);
}

//Активирую надчёркнутый шрифт
void SheetOfNotes::on_overlinef_released()
{
    //Беру текущий формат выделенного текста
    QTextCharFormat form = ui->winNote->currentCharFormat();

    //Если текст не надчёркнут - надчеркнуть, иначе - убрать надчёркивание
    if(!form.fontOverline()) form.setFontOverline(true);
    else form.setFontOverline(false);

    //устанавливаю формат
    ui->winNote->setCurrentCharFormat(form);
}

//Активирую верхний индекс
void SheetOfNotes::on_highindexf_released()
{
    //Беру текущий формат выделенного текста
    QTextCharFormat form = ui->winNote->currentCharFormat();

    //Если текст не в верхнем индексе - поднять, иначе - восстановить
    if(form.verticalAlignment() != QTextCharFormat::AlignSuperScript)
        form.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    else
        form.setVerticalAlignment(QTextCharFormat::AlignNormal);

    //устанавливаю формат
    ui->winNote->setCurrentCharFormat(form);
}

//Активирую нижний индекс
void SheetOfNotes::on_lowindexf_released()
{
    //Беру текущий формат выделенного текста
    QTextCharFormat form = ui->winNote->currentCharFormat();

    //Если текст не в нижнем индексе - опустить, иначе - восстановить
    if(form.verticalAlignment() != QTextCharFormat::AlignSubScript)
        form.setVerticalAlignment(QTextCharFormat::AlignSubScript);
    else
        form.setVerticalAlignment(QTextCharFormat::AlignNormal);

    //устанавливаю формат
    ui->winNote->setCurrentCharFormat(form);
}

//Устанавливаю цвет текста
void SheetOfNotes::on_colorfont_released()
{
    //Создаю окно выбора цвета, куда передаю текущий цвет
    QColor color = QColorDialog::getColor(ui->winNote->textColor());

    //Если пользователь подтвердил выбор - установить выбранный цвет
    if(color.isValid()) ui->winNote->setTextColor(color);
}

//Устанавливаю цвет фона
void SheetOfNotes::on_colorbackground_released()
{
    //Создаю окно выбора цвета, куда передаю текущий цвет
    QColor color = QColorDialog::getColor(ui->winNote->textColor());

    //Если пользователь подтвердил выбор - установить выбранный цвет
    if(color.isValid()) ui->winNote->setTextBackgroundColor(color);
}

//Включаю маркированный список
void SheetOfNotes::on_bulletedlistf_released()
{
    //Вызываю метод, который построит список, указывая, каким он должен быть
    createlist(QTextListFormat::ListSquare);
}

//Включаю нумерованный список
void SheetOfNotes::on_numberedlistf_released()
{
    //Вызываю метод, который построит список, указывая, каким он должен быть
    createlist(QTextListFormat::ListDecimal);
}

//Отмечаю элемент списка
void SheetOfNotes::on_checkedCheck_released()
{
    //Вызываю метод, который отметит элемент списка
    checkinglist(QTextBlockFormat::MarkerType::Checked);
}

//Убираю отмечивание списка
void SheetOfNotes::on_emptyCheck_released()
{
    //Вызываю метод, который отметит элемент списка
    checkinglist(QTextBlockFormat::MarkerType::Unchecked);
}

//Устанавливаю регистр для выбранного текста
void SheetOfNotes::on_boxregister_activated(int index)
{
    //Беру выделенный текст
    QTextCursor textregister = ui->winNote->textCursor();

    //Создаю строки старого и нового регистров соответственно
    QString oldtext = textregister.selectedText(), newtext = "";

    //Булевая переменная, хранящая информацию, был ли последним знаком пробел
    bool space = false;

    //Если последним знаком был пробел - вырываю его, и указываю это в булевой переменной
    if(oldtext[oldtext.size() - 1] == ' ')
    {
        oldtext = oldtext.mid(0, oldtext.size() - 1);
        space = true;
    }

    //Разделяю выделенный текст по слова
    //Нужно для пунктов: 4
    QStringList listword = oldtext.split(' ');

    //В зависимости от выбранного пункта, в выделенном тексте я делаю следующее
    switch (index)
    {
    case 0:
        break;

    case 1:
        //"Как в предложении."
        //Бервую букву делаю заглавной и в конце добавляю точку
        newtext = oldtext[0].toUpper()  + oldtext.mid(1).toLower() + ".";
        break;

    case 2:
        //"Нижний регистр"
        //Делаю все буквы строчными
        newtext = oldtext.toLower();
        break;

    case 3:
        //"Верхний регистр"
        //Делаю все буквы заглавными
        newtext = oldtext.toUpper();
        break;

    case 4:
        //"Начинать С Прописных"
        //В разбитом списке беру каждое слово и в каждом слове делаю
        //все первые буквы заглавными, а все остальные - строчными
        for (int i = 0; i < listword.size(); i++) {
            //Беру слово
            QString word = listword[i];

            //Меняю регистр
            word = word[0].toUpper() + word.mid(1).toLower();

            //Восстанавливаю предложение - после каждого слова (кроме
            //последнего) я добавляю пробел
            if(i != listword.size() - 1) newtext += word + " ";
            else newtext += word;
        }
        break;

    case 5:
        //"иЗМЕНЕНИЕ РЕГИСТРА"
        //Проверяю каждую букву - ясли она заглавная - делаю
        //строчной, иначе - заглавной
        for (int i = 0; i < oldtext.size(); i++)
        {
            if(oldtext[i].isUpper()) newtext += oldtext[i].toLower();
            else newtext += oldtext[i].toUpper();
        }
        break;
    }

    //Если был пробел - возвращаю
    if(space) newtext += " ";

    //Возвращаю текст - на месте курсора появится новый текст
    textregister.insertText(newtext);
}

//Динамически устанавливаю левый отступ всего абзаца
void SheetOfNotes::on_leftmarginspinbox_valueChanged()
{
    //Вызываю метод, который установит поля
    optionIndentHeight();
}

//Динамически устанавливаю верхний отступ всего абзаца
void SheetOfNotes::on_upmarginspinbox_valueChanged()
{
    //Вызываю метод, который установит поля
    optionIndentHeight();
}

//Динамически устанавливаю правый отступ всего абзаца
void SheetOfNotes::on_rightmarginspinbox_valueChanged()
{
    //Вызываю метод, который установит поля
    optionIndentHeight();
}

//Динамически устанавливаю нижний отступ всего абзаца
void SheetOfNotes::on_downmarginspinbox_valueChanged()
{
    //Вызываю метод, который установит поля
    optionIndentHeight();
}

//Увеличиваю отступы между абзацами текущему абзацу
void SheetOfNotes::on_increaseparagraphspacing_released()
{
    //Просто увеличиваю верхние и нижние отступы на единицу и устанавливаю их
    ui->upmarginspinbox->setValue(ui->upmarginspinbox->value() + 1);
    ui->downmarginspinbox->setValue(ui->downmarginspinbox->value() + 1);
    optionIndentHeight();
}

//Уменьшаю отступы между абзацами текущему абзацу
void SheetOfNotes::on_reduceparagraphspacing_released()
{
    //Просто уменьшаю верхние и нижние отступы на единицу и устанавливаю их
    ui->upmarginspinbox->setValue(ui->upmarginspinbox->value() - 1);
    ui->downmarginspinbox->setValue(ui->downmarginspinbox->value() - 1);
    optionIndentHeight();
}

//Удаляю отступы
void SheetOfNotes::on_delmargin_released()
{
    //Очищаю все отступы и устанавливаю новые (пустые)
    ui->leftmarginspinbox->setValue(0.0);
    ui->upmarginspinbox->setValue(0.0);
    ui->rightmarginspinbox->setValue(0.0);
    ui->downmarginspinbox->setValue(0.0);

    optionIndentHeight();
}

//Динамически устанавливаю оступ первой строки абзаца
void SheetOfNotes::on_spinparagraphindent_valueChanged()
{
    //Вызываю метод, который установит текущий отступ первой строки абзаца
    optionIndentHeight();
}

//Увеличиваю отступ первой строки абзаца
void SheetOfNotes::on_increaseindent_released()
{
    //Просто увеличиваю отступ первой строки и устанавливаю его
    ui->spinparagraphindent->setValue(ui->spinparagraphindent->value() + 2);
    optionIndentHeight();
}

//Уменьшаю отступ первой строки абзаца
void SheetOfNotes::on_reduceindent_released()
{
    //Просто уменьшаю отступ первой строки и устанавливаю его
    ui->spinparagraphindent->setValue(ui->spinparagraphindent->value() - 2);
    optionIndentHeight();
}

//Динамически устанавливаю межстрочный интервал
void SheetOfNotes::on_spinlineheight_valueChanged()
{
    //Вызываю метод, который установит межстрочный интервал
    optionIndentHeight();
}

//Увеличиваю межстрочный интервал
void SheetOfNotes::on_increaselinespacing_released()
{
    //Просто увеличиваю межстрочный интервал и устанавливаю его
    ui->spinlineheight->setValue(ui->spinlineheight->value() + 1);
    optionIndentHeight();
}

//Уменьшаю межстрочный интервал
void SheetOfNotes::on_reducelinespacing_released()
{
    //Просто увеличиваю межстрочный интервал и уменьшаю его
    ui->spinlineheight->setValue(ui->spinlineheight->value() - 1);
    optionIndentHeight();
}

//Динамически меняю поля документа
void SheetOfNotes::on_sizefield_valueChanged()
{
    ui->winNote->document()->setDocumentMargin(ui->sizefield->value());
}

//Создаю таблицу
void SheetOfNotes::on_buttaccept_released()
{
    //Беру текущее местоположение курсора
    QTextCursor currentable = ui->winNote->textCursor();

    //По нему создаю таблицу с размерами, которые
    //пользователь указал возле кнопки "создать таблицу"
    currentable.insertTable(ui->spinrow->value(), ui->spincolumn->value());

    //В текущий курсор поля ввода вставляю курсор с таблицей
    ui->winNote->setTextCursor(currentable);
}

//Объединяю выделенные ячейки
void SheetOfNotes::on_mergecells_released()
{
    //Беру текущее местоположение курсора
    QTextCursor currentable = ui->winNote->textCursor();

    //Если под курсором находится таблица - возвращаю указатель на таблицу,
    //иначе - nullptr
    QTextTable *curtab = currentable.currentTable();

    if(curtab != nullptr)
    {
        //Объединяю выбранные ячейки
        curtab->mergeCells(currentable);
    }
}

//Разбиваю ячейку на колонки
void SheetOfNotes::on_severalrowcells_released()
{
    //Вызываю метод, который разобъёт ячейки
    severalcells(true);
}

//Разбиваю ячейку на строки
void SheetOfNotes::on_severalcolumncells_released()
{
    //Вызываю метод, который разобъёт ячейки
    severalcells(false);
}

//Вставляю строку сверху
void SheetOfNotes::on_insertup_released()
{
    modificationCells(true, true, 0, 1);
}

//Вставляю строку снизу
void SheetOfNotes::on_insertdown_released()
{
    modificationCells(true, true, 1, 1);
}

//Вставляю колонку после
void SheetOfNotes::on_insertright_released()
{
    modificationCells(true, false, 1, 1);
}

//Вставляю колонку перед
void SheetOfNotes::on_insertleft_released()
{
    modificationCells(true, false, 0, 1);
}

//Удаляю строку сверху
void SheetOfNotes::on_deleteup_released()
{
    modificationCells(false, true, -1, 1);
}

//Удаляю строку снизу
void SheetOfNotes::on_deletedown_released()
{
    modificationCells(false, true, 1, 1);
}

//Удаляю колонка после
void SheetOfNotes::on_deleteright_released()
{
    modificationCells(false, false, 1, 1);
}

//Удаляю колонку перед
void SheetOfNotes::on_deleteleft_released()
{
    modificationCells(false, false, -1, 1);
}

//Удаляю таблицу
void SheetOfNotes::on_deletetable_released()
{
    //Беру текущее местоположение курсора
    QTextCursor currentable = ui->winNote->textCursor();

    //Если под курсором находится таблица - возвращаю указатель на таблицу,
    //иначе - nullptr
    QTextTable *curtab = currentable.currentTable();

    if(curtab != nullptr)
    {
        //Если указатель находится в таблице, удаляю все строки и столбики
        curtab->removeColumns(0, curtab->columns());
        curtab->removeRows(0, curtab->rows());
    }
}

//Ищу введёный текст в файле
void SheetOfNotes::on_findline_textChanged(const QString &arg1)
{
    //Если m_highlighter существует - передаю ему строку для поиска
    if(m_highlighter) m_highlighter->setHighlightString(arg1);

    //Старый код поиска:

//    //Для поиска по файлу нужен его документ
//    QTextDocument *forsearch = ui->winNote->document();

//    //Переменная для проверки, был ли найден текст
//    bool found = false;

//    //Если поиск делается не впервые (обнуляется каждый раз, когда
//    //пользователь нажимает на поле ввода) - откадываю сделанные изменения
//    //до прошлого состояния заметки (чтобы можно было отменить
//    //перекраску текста)
//    if(!isFirstTime) forsearch->undo();

//    //Курсор, который будет отображать найденный текст
//    QTextCursor findtext (forsearch);

//    //Создаю курсор, который будет ограничивать события, которые
//    //буду откатываться при каждом новом вводе буквы
//    QTextCursor blockcursor(forsearch);
//    blockcursor.beginEditBlock(); //Начинаю блок
//    //

//    //Задаю размер и цвет найденному тексту
//    QTextCharFormat colorForm;
//    colorForm.setForeground(Qt::red);

//    //Если курсор не пустой (если заметка не пуста) и если не
//    //достигнут конец файла, тогда выполняется следующее:
//    while (!findtext.isNull() && !findtext.atEnd())
//    {
//        //Нахожу в документе введёный в поле поиска текст (он зависит от регистра)
//        findtext = forsearch->find(arg1, findtext, QTextDocument::FindCaseSensitively);

//        //И сели документ не пустой, то:
//        if(!findtext.isNull())
//        {
//            found = true; //Отмечаю, что текст найден
//            findtext.mergeCharFormat(colorForm); //Выделяю найденый текст
//        }
//    }

//    //
//    blockcursor.endEditBlock(); //Заканчиваю блок

//    isFirstTime = false; //Отмечаю, что поиск проводится не впервые

//    //Если текст не найден и если этот самый виджет не пуст - делаю
//    //проверку: не прекратил ли пользователь поиск
//    if(!found && ui->findline->text() != "")
//    {
//        //Если активно поле поиска - вывожу ошибку
//        if(ui->findline->hasFocus()) winCrit("Текст не найден");
//    }
}

//Устанавливаю вид редактора - заметка
void SheetOfNotes::on_viewnotice_released()
{
    //Создаю характеристику вида обзора
    QString characteristic = tr("Заметка. Чем уникален: Не уникален");

    //Вызываю метод, который настроит поле ввода
    setFormatSheet(characteristic, 1100, 570);
}

//Устанавливаю вид редактора - документ
void SheetOfNotes::on_viewdocument_released()
{
    //Создаю характеристику вида обзора
    QString characteristic = tr("Документ. Чем уникален: Ширина виджета равна ширине документа и отображает то, как будет выглядеть документ");

    //Вызываю метод, который настроит поле ввода
    setFormatSheet(characteristic, 670, 570);
}

//Устанавливаю вид редактора - код
void SheetOfNotes::on_viewcode_released()
{
    //Создаю характеристику вида обзора
    QString characteristic = tr("Код. Чем уникален: Имеет подсветку синтаксиса языка С++");

    //Вызываю метод, который настроит поле ввода
    setFormatSheet(characteristic, 1100, 570);
}

// // // // // // // // // // // // // // // // // // // //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////          -------------------          /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////                METHODS                /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// ///////          -------------------          /////// //
// / //  +  //  -  / * / * / * / * / * /  -  //  +  // / //
// // // // // // // // // // // // // // // // // // // //

//Метод, вызываемый "winload" для создания файла в директории и сохранения
//пути к нему и имени для дальнейшей работы с файлом
void SheetOfNotes::createFile(QString pathtofile, QString namefile)
{
    //Сохраняю путь
    path = new QString;
    *path = pathtofile;

    //Сохраняю название файла
    filename = new QString;
    *filename = namefile;

    enableded(); //Включаю редактор шрифта

    //Создаю файл
    QFile Note(*path);
    Note.open(QIODevice::WriteOnly | QIODevice::Text);
    Note.close();

    ui->winNote->setFocus();//Делаю поле ввода активным
    ui->winNote->moveCursor(QTextCursor::End); //Устанавливаю курсор в конец файла

    optionIndentHeight(); //Вызываю чтобы установить стандартые показатели спинбоксов в первый абзац

    //Устанавливаю начальный размер шрифта
    //Если этого не делать - ставится 14, а я хочу 20
    ui->winNote->setFontPointSize(ui->fontsize->value());
}

//Метод, вызываемый "winload" для открытия выбранного файла и считывания с него
//информации в поле редактирования текста
void SheetOfNotes::openFile(QString pathtofile, QString namefile)
{
    //Сохраняю путь
    path = new QString;
    *path = pathtofile;

    //Сохраняю название файла
    filename = new QString;
    *filename = namefile;

    enableded(); //Включаю редактор шрифта

    //Создаю файл и считываю с него информацию
    QFile Note(*path);
    if (Note.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textNote(&Note);
        userNote = textNote.readAll();
    }
    Note.close();

    ui->winNote->setText(userNote); //Помещаю информацию в поле ввода

    ui->winNote->setFocus();//Делаю поле ввода активным
    ui->winNote->moveCursor(QTextCursor::End); //Устанавливаю курсор в конец файла

    loadOption(); //Вызываю чтобы считать параметры последнего абзаца в спинбоксы

    if(ui->winNote->document()->isEmpty()) ui->fontsize->setValue(20);
}

//Метод, запускающий окно создания/открытия файла
bool SheetOfNotes::note(QString text)
{
    //Если поле активно, формиирую сообщение
    //Если не возвращается false (пользователь не
    //отменил операцию) - возвращается true
    QMessageBox Warn;
    Warn.setIcon(QMessageBox::Question);
    Warn.setText("<html><head/><body><p><span style=\" font-size:12pt;\">" + text + "</span></p></body></html>");
    Warn.setInformativeText(tr("<html><head/><body><p>Если Вы <span style=\" font-weight:600;\">проигнорируете</span> - текущая информация будет потеряна;</p>"
                            "<p>если <span style=\" font-weight:600;\">продолжите</span> - сохранится в текстовом формате;</p>"
                            "<p>если <span style=\" font-weight:600;\">сохраните</span> - сохранится в HTML формате.</p></body></html>"));
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
        ui->spinparagraphindent->setValue(10); //Устанавливаю отступ абзаца в spinbox
        disableded(); //Выключаю элементы управления
        delete(path); //Удаляю путь к файлу
        delete(filename); //Удаляю название файла
    }
    else winCrit(tr("Нет заметки для сохранения!")); //Иначе вызываю сообщение об ошибке
}

//Метод, закрывающий файл без сохранения
void SheetOfNotes::closenosave()
{
    //Если поле ввода активно - сразу выключаю редактор
    if(ui->winNote->isEnabled())
    {
        ui->winNote->setText(""); //Очищаю поле ввода
        ui->spinparagraphindent->setValue(10); //Устанавливаю отступ абзаца в spinbox
        disableded(); //Выключаю редактор шрифта
        delete(path); //Удаляю путь к файлу
        delete(filename); //Удаляю название файла
    }
    else winCrit(tr("Нет заметки для сохранения!")); //Иначе вызываю сообщение об ошибке
}

//Метод, который удаляет выбранные файлы
void SheetOfNotes::deleteFile()
{
    //Отображаю окно, где пользователь выбирает картинки
    QStringList namesDelfiles = QFileDialog::getOpenFileNames(this,
                                                     tr("Удалить файл"),
                                                     "notices/",
                                                     tr("Other file (*.txt "
                                                        "*.cpp "
                                                        "*.doc "
                                                        "*.odt "
                                                        "*.pdf "
                                                        "*.zip "
                                                        "*.bin)"));
    //Удаляю по очереди все выбранные файлы
    for (int i = 0; i < namesDelfiles.size(); i++)
        QFile::remove(namesDelfiles[i]);
}

//Метод, который строит окно ошибки при попытке
//сохранить файл, когда он не был открыт
void SheetOfNotes::winCrit(QString cause)
{
    QMessageBox Crit;
    Crit.setIcon(QMessageBox::Critical);
    Crit.setText(tr("<html><head/><body><p><span style=\" font-size:12pt;\">Ошибка</span></p></body></html>"));
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
    this->setWindowTitle(*filename + tr(" - Блокнот"));

    ui->winNote->setEnabled(true); //Включаю поле ввода

    ui->fontComboBox->setEnabled(true); //Включаю редактор шрифта
    ui->fontsize->setEnabled(true); //Включаю редактор размера

    ui->tabgroup->setEnabled(true); //Включаю элементы редактирования

    //Включаю необходимые меню
    ui->exportFile->setEnabled(true);
    ui->edit->setEnabled(true);
    ui->insert->setEnabled(true);

    //Устанавливаю стандартный шрифт и его размер в панели ввода
    QFont font(ui->fontComboBox->currentFont().toString(), ui->fontsize->value());
    ui->winNote->setFont(font);

    //Симулирую нажатие кнопки для обновления labvieweditor2
    on_viewnotice_released();
}

//Выключаю редактор: поле ввода и элементы редактирование
void SheetOfNotes::disableded()
{
    this->setWindowTitle(tr("Блокнот"));

    ui->winNote->setEnabled(false); //Выключаю поле ввода

    ui->fontComboBox->setEnabled(false); //Выключаю редактор шрифта
    ui->fontsize->setEnabled(false); //Выключаю редактор размера

    ui->tabgroup->setEnabled(false); //Выключаю элементы редактирования

    //Выключаю необходимые меню
    ui->exportFile->setEnabled(false);
    ui->edit->setEnabled(false);
    ui->insert->setEnabled(false);

    //Очищаю labvieweditor2
    ui->viewnotice->setChecked(true);
    ui->labvieweditor2->setText("*");
}

//Метод, который устанавливает параметры абзаца и интервалов
//Если применять всё по отдельности - предыдущий будет сбрасываться и
//чтобы они все действовали одновременно - нужно устанавливать их вместе
void SheetOfNotes::optionIndentHeight()
{
    //Беру текущую позицию курсора (нужно для определения абзаца)
    QTextCursor currentext = ui->winNote->textCursor();

    //Если был список - беру его формат
    if(currentext.currentList() != nullptr)
    {
        QTextList *textlist = currentext.currentList();
        QTextListFormat textlistform = textlist->format();
    }

    //Создаю модель формата блока (абзаца)
    QTextBlockFormat textblockform = currentext.blockFormat();

    //Устанавливаю отступ первой строки абзаца
    textblockform.setTextIndent(ui->spinparagraphindent->value());

    //Устанавливаю межстрочный интерва
    textblockform.setLineHeight(ui->spinlineheight->value(), QTextBlockFormat::LineDistanceHeight);

    //Устанавливаю отступы абзаца
    textblockform.setLeftMargin(ui->leftmarginspinbox->value());
    textblockform.setTopMargin(ui->upmarginspinbox->value());
    textblockform.setRightMargin(ui->rightmarginspinbox->value());
    textblockform.setBottomMargin(ui->downmarginspinbox->value());

    //Применяю весь формат абзаца на абзац, выбранный курсором
    currentext.setBlockFormat(textblockform);
}

//Метод, который считывает параметры абзаца и интервалов с абзаца
//Все параметры собраны вместе, так как их вместе нужно определять
void SheetOfNotes::loadOption()
{
    //Считываю размер шрифта
    ui->fontsize->setValue(ui->winNote->fontPointSize());

    //Беру текущую позицию курсора (нужно для определения абзаца)
    QTextCursor currentext = ui->winNote->textCursor();

    //Если был список - беру его формат
    if(currentext.currentList() != nullptr)
    {
        QTextList *textlist = currentext.currentList();
        QTextListFormat textlistform = textlist->format();
    }

    //Если была таблица - восстанавливаю размер шрифта (каждый раз, когда
    //создаётся таблица, почему-то сбивается размер шрифта в таблице и после)
    if(currentext.currentTable() != nullptr && ui->fontsize->value() == 6)
    {
        //Восстанавливаю шрифт в таблице
        ui->fontsize->setValue(20);
        ui->winNote->setFontPointSize(20);
    }

    //Если была таблица перед последним символом (т.е. если
    //файл заканчивается таблицей) - устанавливаю в конце файла
    //стандартные показатели. Так как форматирование сбивается
    //на последнем символе после таблицы, то я проверяю, чтобы текущий
    //курсор не был в таблице, а предыдущий был. В этом случае я
    //восстанавливаю форматирование под текущим курсором, иначе - формат
    //не сбивался и делать нечего.

    //Беру курсоры
    QTextCursor curintable = ui->winNote->textCursor();
    QTextCursor curaftertable = ui->winNote->textCursor();
    curintable.setPosition(curaftertable.position() - 1);

    //Если таблица последняя - восстанавливаю формат
    if(curintable.currentTable() != nullptr && curaftertable.currentTable() == nullptr)
    {
        ui->fontsize->setValue(20); //Устанавливаю стандартный размер шрифта в combobox
        ui->spinparagraphindent->setValue(10); //Устанавливаю отступ абзаца в spinbox
        ui->spinlineheight->setValue(0); //Устанавливаю межстрочное расстояние в spinbox
    }

    //Вывожу размер таблицы
    if(currentext.currentTable() != nullptr)
    {
        //Если таблица есть - вывожу её размеры
        QTextTable *textable = currentext.currentTable();
        ui->spinrow->setValue(textable->rows());
        ui->spincolumn->setValue(textable->columns());
        ui->spinseveral->setValue(0);
    }
    else
    {
        //иначе - таблицы нет и вывожу нулевые размеры
        ui->spinrow->setValue(0);
        ui->spincolumn->setValue(0);
        ui->spinseveral->setValue(0);
    }

    //Создаю модель формата блока (абзаца) и считываю информацию с курсора
    QTextBlockFormat indicationFormat = currentext.blockFormat();

    //Определяю отступ первой строки абзаца
    ui->spinparagraphindent->setValue(indicationFormat.textIndent());

    //Определяю межстрочный интервал
    ui->spinlineheight->setValue(indicationFormat.lineHeight());

    //Определяю отступы абзаца
    ui->leftmarginspinbox->setValue(indicationFormat.leftMargin());
    ui->upmarginspinbox->setValue(indicationFormat.topMargin());
    ui->rightmarginspinbox->setValue(indicationFormat.rightMargin());
    ui->downmarginspinbox->setValue(indicationFormat.bottomMargin());

    //Востанавливаю регистр
    ui->boxregister->setCurrentIndex(0);

    //Очищаю поле поиска
    ui->findline->setText("");
    //if(ui->winNote->hasFocus()) isFirstTime = true; метод для старого функционала
}

//Метод, который создаёт список
void SheetOfNotes::createlist(QTextListFormat::Style listform)
{
    //Беру указатель на текущую позицию/выбранный текст
    QTextCursor currentlist = ui->winNote->textCursor();

    //Очищаю буфер обмена
    QApplication::clipboard()->clear();

    //Если выбран текст - помещаю выбранный текст в буфер обмена
    ui->winNote->copy();

    //Создаю список и устанавливаю стиль
    QTextListFormat textlistform;
    textlistform.setStyle(listform);

    //Вставляю список на текущую позицию/вместо выбранного текста
    currentlist.insertList(textlistform);

    //Если выбран текст - вставляю в первую позицию замещённый текст
    ui->winNote->paste();
}

//Метод, который отмечает элемент списка
void SheetOfNotes::checkinglist(QTextBlockFormat::MarkerType type)
{
    //Беру формат курсора
    QTextCursor check = ui->winNote->textCursor();
    QTextBlockFormat checked = check.blockFormat();

    //Беру стиль маркера и устанавливаю выделение только
    //на маркированные маркированные списки
    QTextList *list = check.currentList();
    QTextListFormat listform = list->format();
    if(listform.style() == QTextListFormat::ListSquare) checked.setMarker(type);

    //Возвращаю формат курсору
    check.setBlockFormat(checked);
}

//Метод, который (в зависимости от режима) разбивает ячейки
void SheetOfNotes::severalcells(bool rows)
{
    //mode - режим работы: true  - rows, false - columns

    //Беру текущее местоположение курсора
    QTextCursor currentable = ui->winNote->textCursor();

    //Если под курсором находится таблица - возвращаю указатель на таблицу,
    //иначе - nullptr
    QTextTable *curtab = currentable.currentTable();

    //Если указатель находится в таблице - продолжаю работу
    if(curtab != nullptr)
    {
        //Переменная, которая хранит указатель на текущую ячейку
        QTextTableCell curtabcell = curtab->cellAt(currentable);

        if(rows)
        {
            //Добавляю после текущего столбика несколько столбиков
            curtab->insertColumns(curtabcell.column() + 1, ui->spinseveral->value());

            //Объединяю назад с новыми ячейками те, которые не нужно было
            //делить, т.е. выше и ниже ячейки, которую поделили. Таким
            //образом разбитой останется только выбранная
            for (int i = 0; i < curtab->rows(); i++)
                if(i != curtabcell.row())
                    curtab->mergeCells(i, curtabcell.column(), 1, ui->spinseveral->value() + 1);
        }
        else
        {
            //Добавляю ниже текущей строки нестролько строк
            curtab->insertRows(curtabcell.row() + 1, ui->spinseveral->value());

            //Объединяю назад с новыми ячейками те, которые не нужно было
            //делить, т.е. левее и правее ячейки, которую поделили. Таким
            //образом разбитой останется только выбранная
            for (int i = 0; i < curtab->columns(); i++)
                if(i != curtabcell.column())
                    curtab->mergeCells(curtabcell.row(), i, ui->spinseveral->value() + 1, 1);
        }
    }
}

//Метод, который (в зависимости от параметров) работает с ячейками таблицы
void SheetOfNotes::modificationCells(bool insert, bool row, int index, int amount)
{
    //true - insert, false - remove
    //true - rows, false - columns

    //Беру текущее местоположение курсора
    QTextCursor currentable = ui->winNote->textCursor();

    //Если под курсором находится таблица - возвращаю указатель на таблицу,
    //иначе - nullptr
    QTextTable *curtab = currentable.currentTable();

    if(curtab != nullptr)
    {
        //Переменная, которая хранит указатель на текущую ячейку
        QTextTableCell curtabcell = curtab->cellAt(currentable);

        if(insert && row)
        {
            //Добавляю строку
            curtab->insertRows(curtabcell.row() + index, amount);
        }
        else if(insert && !row)
        {
            //Добавляю колонку
            curtab->insertColumns(curtabcell.column() + index, amount);
        }
        else if(!insert && row)
        {
            //Удаляю строку
            curtab->removeRows(curtabcell.row() +  index, amount);
        }
        else if(!insert && !row)
        {
            //Удаляю колонку
            curtab->removeColumns(curtabcell.column() + index, amount);
        }
    }
}

//Метод, который настраивает поле ввода
void SheetOfNotes::setFormatSheet(QString textLabvieweditor2, int widthSheet, int heightSheet)
{
    //Указываю, какой вид обзора установлен
    ui->labvieweditor2->setText(textLabvieweditor2);

    //Устанавливаю вид поля ввода
    ui->winNote->setMinimumSize(widthSheet, heightSheet);

    //Если сигнал включен - удаляю подсветку и выключаю
    //сигнал, иначе - создаю подсветку и включаю сигнал
    if(viewCODE)
    {
        delete(s_highlighter);
        viewCODE = false;
    }
    else if(ui->viewcode->isChecked())
    {
        s_highlighter = new highlight_syntax(ui->winNote->document());
        viewCODE = true;
    }
}

//-------------------------------------------------------//

//Место для следующих слотов, которые будут выполнять функционал следующих
//обновлений и буду реализованы, но пока неизвестно, что будет добавлено
//и как оно должно располагаться в коде, поэтому слоты будут добавляться здесь
//и уже потом получать своё место




