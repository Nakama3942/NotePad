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

#ifndef HIGHLIGHT_SEARCH_H
#define HIGHLIGHT_SEARCH_H

#include <QSyntaxHighlighter>

class highlight_search : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit highlight_search(QTextDocument *parent = nullptr);

    //Переопределённый метод для определения блока выделения
    void highlightBlock(const QString &text);

    //Переопределённый метод для применения выделения текущему блоку
    void setHighlightString(const QString &str);

private:
    //Переменная, которая хранит искомый текст
    QString m_HighlightString;

    //Переменная, которая хранит формат выделения
    QTextCharFormat colorForm;
};

#endif // HIGHLIGHT_SEARCH_H
