#ifndef WINSYSTEMCOMMAND_H
#define WINSYSTEMCOMMAND_H

#include <QString>

namespace WinSystemCommand
{
    enum Command
    {
        Force,         // Выключает питание компьютера(компьютер должен поддерживать данную функцию).
        PowerOff,      // Перезагружает систему.
        Reboot,        // Форсировать выключение компьютера. Если задан этот флаг, то Windows не посылает сообщения WM_QUERYENDSESSION и WM_ENDSESSION выполняющимся приложениям. Следовательно, эти приложения не смогут сделать запросов пользователю о сохранении данных и могут потерять последние изменения документов, с которыми они работали. Так что использовать этот флаг надо очень осторожно, только в каких-то аварийных ситуациях.
        Shutdown,      // Завершает работу компьютера до того места, где он может быть безопасно выключен: сброшенны все файловые буферы на диск, завершает работу всех процессов.
        LogOff,        // Завершает сессию текущего пользователя.
        Hibernate,     // Спящий режим.
        Suspend,       // Ждущий режим.
        ScreenOff      // Выключение дисплея.
    };

    bool execute( Command command );
    QString getTextError( Command command );
}

#endif // WINSYSTEMCOMMAND_H
