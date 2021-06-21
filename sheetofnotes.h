#ifndef SHEETOFNOTES_H
#define SHEETOFNOTES_H

#include <QMainWindow>

#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QTranslator>
#include <QClipboard>
#include <QFontDatabase>
#include <QFont>
#include <QtPrintSupport/QPrinter>
#include <QTextDocumentWriter>
#include <QProcess>

#include "createfile.h"
#include "aboutprog.h"
#include "deletefile.h"
#include "imageinserter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SheetOfNotes; }
QT_END_NAMESPACE

class SheetOfNotes : public QMainWindow
{
    Q_OBJECT

public:
    SheetOfNotes(QWidget *parent = nullptr);
    ~SheetOfNotes();

private slots:

    // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- //
    //                    -----Слоты элементов меню-----                    //
    // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- //

    //Слоты меню файла
    void on_actCreate_triggered(); //Создание файла
    void on_actOpen_triggered(); //Открытие файла

    void on_actSaveText_triggered(); //Сохранение текстового файла
    void on_actSaveHTML_triggered(); //Сохранение файла в HTML

    void on_actSaveTextClose_triggered(); //Сохранение и закрытие текстового файла
    void on_actSaveHTMLClose_triggered(); //Сохранение и закрытие файла в HTML
    void on_actCloseNoSave_triggered(); //Закрытие файла без сохранения

    void on_actRemove_triggered(); //Удаление текущего файла
    void on_actRemOther_triggered(); //Удаление файда с директории программы

    void on_actExp_triggered(); //Отображение файла в файловом менеджере

    void on_actCloseProg_triggered(); //Выход из программы

    //Слоты меню генератора
    void on_gen_doc_triggered(); //Генерация вордовского файла
    void on_gen_odt_triggered(); //Генерация odt-шки
    void on_gen_pdf_triggered(); //Генерация pdf-ки

    //Слоты меню правки
    void on_actUndo_triggered(); //Отменить
    void on_actRedo_triggered(); //Повторить

    void on_actCut_triggered(); //Вырезать
    void on_actCopy_triggered(); //Копировать
    void on_actPaste_triggered(); //Вставить

    void on_actClear_triggered(); //Очистить

    void on_timedate_triggered(); //Вставка даты-времени

    // -Раньше слоты элементов настройки шрифта были здесь, после меню правки

    //Изначально я планировал, что настройка шрифта будет минимальная и буде
    //производится из меню, как в стандартном блокне Windows, но с потребностью
    //расширить функционал, из-за нежелание выводить это в отдельное окно и через
    //невозможность реализации этого в меню, я решил вынести это в основное окно,
    //а слоты элементов редактирования шрифта оставались здесь в своём порядке, как
    //бы они были в меню. Потом я это исправил и вынес отдельно после слотов
    //меню. В cpp файлы те слоты находятся в этом месте

    //Слоты меню выбора языка
    void on_actRus_triggered(); //Выбор русского языка
    void on_actEng_triggered(); //Выбор английского языка

    //Слоты меню документации
    void on_aboutProg_triggered(); //Про программу
    void on_aboutQt_triggered(); //Про Qt

    // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- //
    //           -----Слоты элементов пользовательского окна-----           //
    // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- //

    //Слоты элементов настройки шрифта
    void on_fontComboBox_currentFontChanged(const QFont &f); //Шрифт

    void on_fontsize_activated(int index); //Размер

    void on_bold_toggled(bool checked); //Жирный шрифт
    void on_italicfont_toggled(bool checked); //Курсивный шрифт
    void on_underline_toggled(bool checked); //Подчёркнутый шрифт
    void on_clearformat_released(); //Очистка формата

    void on_alignL_released(); //Выравнивание по левой стороне
    void on_alignC_released(); //Выравнивание по центру
    void on_alignR_released(); //Выравнивание по правой стороне
    void on_alignJ_released(); //Выравнивание по ширине

    void on_buttaccept_released(); //Создание таблицы

    void on_buttloadimage_released(); //Вставка картинки

    void on_winNote_cursorPositionChanged(); //Очистка галочек при изменении курсора

    //-----//

    //Место для следующих слотов, которые будут выполнять функционал следующих
    //обновлений и буду реализованы, но пока неизвестно, что будет добавлено
    //и как оно должно располагаться в коде, поэтому слоты будут добавляться здесь
    //и уже потом получать своё место



private:
    Ui::SheetOfNotes *ui;
    QTranslator qtLanguageTranslator;
    QString userNote, *path, *filename, Date, Time, TimeDate;
    QStringList work;

    //Методы для меню файла
    void createFile(QString pathtoFile, QString namefile);
    void openFile(QString pathtoFile, QString namefile);
    void winload(bool create);
    bool note(QString text);
    void save();
    void closesave(bool text);
    void closenosave();
    void winCrit(QString cause);
    void enableded();
    void disableded();

    //Метод для загрузки картинки
    void loadImage(QString pathtoFile);
};
#endif // SHEETOFNOTES_H
