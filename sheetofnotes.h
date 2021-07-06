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

#ifndef SHEETOFNOTES_H
#define SHEETOFNOTES_H

#include <QMainWindow>

#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>

#include <QMessageBox>
#include <QTranslator>
#include <QClipboard>
#include <QtPrintSupport/QPrinter>
#include <QTextDocumentWriter>
#include <QProcess>
#include <QTextTable>
#include <QTextList>
#include <QDesktopServices>

#include "highlight_search.h"
#include "highlight_syntax.h"

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
    void on_actCreatecode_triggered(); //Создать код
    void on_actAdd_triggered(); //Добавление файла
    void on_actOpen_triggered(); //Открытие файла
    void on_actOpencode_triggered(); //Открыть код
    void on_actOpenbin_triggered(); //Открытие бинарного файла

    void on_actSave_triggered(); //Сохранение текстового файла
    void on_actSaveHTML_triggered(); //Сохранение файла в HTML

    void on_actSaveTextClose_triggered(); //Сохранение и закрытие текстового файла
    void on_actSaveHTMLClose_triggered(); //Сохранение и закрытие файла в HTML
    void on_actCloseNoSave_triggered(); //Закрытие файла без сохранения

    void on_actRemove_triggered(); //Удаление текущего файла
    void on_actRemOther_triggered(); //Удаление файда с директории программы

    void on_actExpFile_triggered(); //Отображение файла в файловом менеджере
    void on_actExpDir_triggered(); //Отображение файловой директории программы

    void on_actCloseProg_triggered(); //Выход из программы

    //Слоты меню экспорта
    void on_gen_doc_triggered(); //Генерация вордовского файла
    void on_gen_odt_triggered(); //Генерация odt-шки
    void on_gen_pdf_triggered(); //Генерация pdf-ки
    void on_gen_zip_triggered(); //Генерация архива
    void on_gen_bin_triggered(); //Генерация бинарника

    //Слоты меню правки
    void on_actUndo_triggered(); //Отменить
    void on_actRedo_triggered(); //Повторить

    void on_actCut_triggered(); //Вырезать
    void on_actCopy_triggered(); //Копировать
    void on_actPaste_triggered(); //Вставить

    void on_actHighlightall_triggered(); //Выделить всё

    void on_actClear_triggered(); //Очистить

    //Слоты меню вставки
    void on_timedate_triggered(); //Вставка даты-времени
    void on_loadimage_triggered(); //Вставка картинки

    // -Раньше слоты элементов настройки шрифта были здесь, после меню правки

    //Изначально я планировал, что настройка шрифта будет минимальная и будет
    //производится из меню, как в стандартном блокне Windows, но с потребностью
    //расширить функционал, из-за нежелание выводить это в отдельное окно и через
    //невозможность реализации этого в меню, я решил вынести это в основное окно,
    //а слоты элементов редактирования шрифта оставались здесь в своём порядке, как
    //бы они были в меню. Потом я это исправил и вынес отдельно после слотов
    //меню. В cpp файлы те слоты находятся в этом месте

    //Слоты меню выбора языка
    void on_actEngUS_triggered(); //Выбор английского языка (США)
    void on_actRusRU_triggered(); //Выбор русского языка (Россия)
    void on_actRusUA_triggered(); //Выбор русского языка (Украина)
    void on_actUkr_triggered(); //Выбор украинского языка
    void on_actJap_triggered(); //Выбор японского языка

    //Слоты меню документации
    void on_aboutProg_triggered(); //Про программу
    void on_CODEPROG_triggered(); //Открываю ГитХаб проекта
    void on_aboutQt_triggered(); //Про Qt

    // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- //
    //           -----Слоты элементов пользовательского окна-----           //
    // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- // -+-+- //

    // ----Поле ввода----
    void on_winNote_cursorPositionChanged(); //Очистка галочек при изменении курсора

    // ----Элементы редактирования текста----
    void on_fontComboBox_currentFontChanged(const QFont &f); //Шрифт
    void on_fontsize_valueChanged(int index); //Размер
    void on_alignL_released(); //Выравнивание по левой стороне
    void on_alignC_released(); //Выравнивание по центру
    void on_alignR_released(); //Выравнивание по правой стороне
    void on_alignJ_released(); //Выравнивание по ширине

    void on_boldf_released(); //Жирный шрифт
    void on_italicf_released(); //Курсивный шрифт
    void on_underlinef_released(); //Подчёркнутый шрифт
    void on_strikeoutf_released(); //Зачёркнутый шрифт
    void on_overlinef_released(); //Надчёркнутый шрифт
    void on_highindexf_released(); //Верхний индекс
    void on_lowindexf_released(); //Нижний индекс
    void on_colorfont_released(); //Цвет текста
    void on_colorbackground_released(); //Цвет фона
    void on_bulletedlistf_released(); //Маркированный список
    void on_numberedlistf_released(); //Нумерованный список
    void on_checkedCheck_released(); //Отмеченный элемент списка
    void on_emptyCheck_released(); //Не отмеченный элемент списка

    void on_boxregister_activated(int index); //Настройка регистра текста

    void on_leftmarginspinbox_valueChanged(); //Установка левого отступа
    void on_upmarginspinbox_valueChanged(); //Установка верхнего отступа
    void on_rightmarginspinbox_valueChanged(); //Установка правого отступа
    void on_downmarginspinbox_valueChanged(); //Установка нижнего отступа
    void on_increaseparagraphspacing_released(); //Увеличить межабзацный интервал
    void on_reduceparagraphspacing_released(); //Уменьшить межабзацный интервал
    void on_delmargin_released(); //Очистить абзацный отступ интервал
    void on_spinparagraphindent_valueChanged(); //Установка отступа абзаца
    void on_increaseindent_released(); //Увеличить отступ абзаца
    void on_reduceindent_released(); //Уменьшить отступ абзаца

    void on_increaselinespacing_released(); //Увеличить межстрочный интервал
    void on_reducelinespacing_released(); //Уменьшить межстрочный интервал
    void on_spinlineheight_valueChanged(); //Установка межстрочного интервала

    void on_sizefield_valueChanged(); //Установка поля для документа

    void on_buttaccept_released(); //Создание таблицы
    void on_mergecells_released(); //Объединение выделенных ячеек
    void on_severalrowcells_released(); //Разбитие ячейки на колонки
    void on_severalcolumncells_released(); //Разбитие ячейки на строки
    void on_insertup_released(); //Вставка строки сверху
    void on_insertdown_released(); //Вставка строки снизу
    void on_insertright_released(); //Вставка колонки после
    void on_insertleft_released(); //Вставка колонки перед
    void on_deleteup_released(); //Удаление строки сверху
    void on_deletedown_released(); //Удаление строки снизу
    void on_deleteright_released(); //Удаление строки после
    void on_deleteleft_released(); //Удаление строки перед
    void on_deletetable_released(); //Удаление таблицы

    void on_findline_textChanged(const QString &arg1); //Поиск по файлу

    void on_viewnotice_released(); //Вид редактора - заметка
    void on_viewdocument_released(); //Вид редактора - документ
    void on_viewcode_released(); //Вид редактора - код

    //-----//

    //Место для следующих слотов, которые будут выполнять функционал следующих
    //обновлений и буду реализованы, но пока неизвестно, что будет добавлено
    //и как оно должно располагаться в коде, поэтому слоты будут добавляться здесь
    //и уже потом получать своё место



protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    Ui::SheetOfNotes *ui;

    QTranslator qtLanguageTranslator; //Объект перевода окна
    QString userNote, *path, *filename; //Объекты текста заметки, его пути и названия

    highlight_search *m_highlighter; //Создаю объект класса поиска введённого текста
    highlight_syntax *s_highlighter; //Создаю объект класса подсветки синтексиса
    bool viewCODE = false; //Сигнал, показывающий, не создан ли объект подсветки

    //bool isFirstTime; //Используется для поиска

    void createFile(QString pathtoFile, QString namefile); //Создание файла
    void openFile(QString pathtoFile, QString namefile); //Открытие файла
    bool note(QString text); //Запускает окно создания/открытия файла
    void save(); //Сохраняет файл
    void closesave(bool text); //Вызывает save и закрывает файл
    void closenosave(); //Закрывает файл без сохранения
    void deleteFile(); //Удаление файла
    void winCrit(QString cause); //Строит окно ошибки
    void enableded(); //Включает элементы редактора
    void disableded(); //Выключает элементы редактора

    void optionIndentHeight(); //Устанавливает настройки абзацев
    void loadOption(); //Считывает настройки абзацев

    void createlist(QTextListFormat::Style listform); //Создаёт списки
    void checkinglist(QTextBlockFormat::MarkerType type); //Отмечает элемент списка

    void severalcells(bool rows); //Разбиваю ячейки
    void modificationCells(bool insert, bool row, int index, int amount); //Работаю с ячейками

    void setFormatSheet(QString textLabvieweditor2, int widthSheet, int heightSheet); //Настройка поля ввода
};
#endif // SHEETOFNOTES_H
