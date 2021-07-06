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

#include "highlight_search.h"

highlight_search::highlight_search(QTextDocument *parent) :
    QSyntaxHighlighter(parent) { }

//Выделяет данный текстовый блок. Эта функция вызывается при
//необходимости механизмом форматированного текста, т.е. для
//текстовых блоков, которые были изменены
void highlight_search::highlightBlock(const QString &text)
{
    //Если строка поиска пуста - возвращаюсь
    if(m_HighlightString.isEmpty()) return;

    //Устанавливаю формат выделения текста
    colorForm.setBackground(Qt::yellow);

    //Нахожу длину строки
    const int LEN = m_HighlightString.length();

    //indexOf() - возвращает позицию индекса последнего вхождения строки

    //Пока индекс последнего вхождения строки поиска больше либо
    //равен ноля - устанавливаю форматирование для текущего
    //индекса и увеличиваю его на длину строки поиска
    for(int index = text.indexOf(m_HighlightString);
         0 <= index;
         index = text.indexOf(m_HighlightString, index + LEN))
    {
        setFormat(index, LEN, colorForm);
    }
}

//Эта функция применяет к текущему текстовому блоку средства выделения
//синтаксиса (т.е. к тексту, который передается в функцию highlightBlock())
void highlight_search::setHighlightString(const QString &str)
{
    m_HighlightString = str; //Принимаю текст, который будет искаться
    rehighlight(); //Повторно применяет выделение ко всему документу
}
