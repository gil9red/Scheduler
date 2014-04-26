#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QTimer>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum Option
    {
        ToQuitProgram, // Закрытие Планировщика и отсылка определенного сигнала.
        Force,         // Выключает питание компьютера(компьютер должен поддерживать данную функцию).
        PowerOff,      // Перезагружает систему.
        Reboot,        // Форсировать выключение компьютера. Если задан этот флаг, то Windows не посылает сообщения WM_QUERYENDSESSION и WM_ENDSESSION выполняющимся приложениям. Следовательно, эти приложения не смогут сделать запросов пользователю о сохранении данных и могут потерять последние изменения документов, с которыми они работали. Так что использовать этот флаг надо очень осторожно, только в каких-то аварийных ситуациях.
        Shutdown,      // Завершает работу комьпьютера до того места, где он может быть безопасно выключен: сброшенны все файловые буферы на диск, завершает работу всех процессов.
        LogOff,        // Завершает сессию текущего пользователя.
        ScreenOff      // Выключение дисплея.
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void sl_UpdateStates();
    void on_completionOfWork_toggled(bool checked);
    void on_buttonBox_clicked(QAbstractButton *button);
    void sl_tick();

private:
    Ui::MainWindow *ui;
    QTimer timer;

protected:
    bool eventFilter( QObject * object, QEvent * event );

signals:
    void about_QuitProgram();
};

#endif // MAINWINDOW_H
