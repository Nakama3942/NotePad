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

#include <QApplication>
#include <QLibrary>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Беру разрешение экрана
    QScreen *screen = QApplication::screens().at(0);
    QSize sizeScreen = screen->size();
    int width = sizeScreen.width(), height = sizeScreen.height();

    //Если экран >= 1280*1024 - открываю программу
    //Иначе - вывожу ошибку и возвращаю 0
    if(width >= 1280 && height >= 1024)
    {
        qDebug() << width << height;

        //Устанавливаю перевод
        QTranslator translator;
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages) {
            const QString baseName = "NotePad_" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                a.installTranslator(&translator);
                break;
            }
        }

        SheetOfNotes w;
        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox Crit;
        Crit.setIcon(QMessageBox::Critical);
        Crit.setText("Ошибка");
        Crit.setInformativeText("Программа работает только с разрешением 1280*1024 и больше");
        Crit.setStandardButtons(QMessageBox::Ok);
        Crit.setDefaultButton(QMessageBox::Ok);
        int clearer = Crit.exec();
        switch (clearer)
        {
        case QMessageBox::Ok:
            break;
        }
    }
    return 0;
}
