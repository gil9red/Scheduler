#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <windows.h>

BOOL SystemControl( MainWindow::Option option )
{
    HANDLE hToken; // указатель на идентификатор процесса
    TOKEN_PRIVILEGES tkp; // указатель на идентифицирующую структуру
    // получение идентификатора текущего процесса, для получения прав на отключение
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return FALSE;

    // Полчение LUID
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1; // установка одной привелегии
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    //Установка привелегии отключения для этого процесса.
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES) NULL, 0);

    // Если ошибка то выход.
    if (GetLastError() != ERROR_SUCCESS)
        return FALSE;

    if ( option == MainWindow::PowerOff )
        ExitWindowsEx( EWX_POWEROFF, 0 );

    //перезагрузка
    if ( option == MainWindow::Reboot )
        ExitWindowsEx(EWX_REBOOT, 0);

    //выключение
    if ( option == MainWindow::Shutdown )
        ExitWindowsEx(EWX_SHUTDOWN, 0);

    // Отменить привилегии отключения.
    tkp.Privileges[0].Attributes = 0;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
    return TRUE;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->completionOfWork->setChecked( false );
    ui->label_completionOfWork->installEventFilter( this );

    // Заполнение вариантов действий.
    {
        ui->options->addItem( tr( "To quit the program" ), ToQuitProgram );
        ui->options->addItem( tr( "Power off" ),           PowerOff );
        ui->options->addItem( tr( "Reboot" ),              Reboot  );
        ui->options->addItem( tr( "Force" ),               Force );
        ui->options->addItem( tr( "Shutdown" ),            Shutdown );
        ui->options->addItem( tr( "Log off" ),             LogOff );
        ui->options->addItem( tr( "Screen off" ),          ScreenOff );

        ui->options->setCurrentIndex( ui->options->findData( ToQuitProgram ) );
    }

    timer.setSingleShot( true );
    timer.setInterval( 333 );
    QObject::connect( &timer, SIGNAL(timeout()), SLOT(sl_tick()) );

    sl_UpdateStates();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sl_UpdateStates()
{
    ui->frame_completionOfWork->setEnabled( ui->completionOfWork->isChecked() );
}
void MainWindow::on_completionOfWork_toggled(bool checked)
{
    sl_UpdateStates();
}
void MainWindow::on_buttonBox_clicked(QAbstractButton * button)
{
    QDialogButtonBox::ButtonRole role = ui->buttonBox->buttonRole( button );
    switch ( role )
    {
    case QDialogButtonBox::ApplyRole:
    {
        bool completionOfWork = ui->completionOfWork->isChecked();
        if ( completionOfWork )
        {
           timer.start();

        } else
        {
            timer.stop();
            ui->remained->clear();
        }

        break;
    }

    case QDialogButtonBox::RejectRole:
        close();
        break;
    }
}
void MainWindow::sl_tick()
{
/*
if (!ExitWindowsEx(EWX_LOGOFF, 0))
ShowMessage("Не удалось сменить пользователя");
Смена пользователя - единственный режим, не требующий особых привилегий. Все остальные режимы не будут выполняться, если приложение не имеет соответствующих привилегий. Например, оператор
if (!ExitWindowsEx(EWX_REBOOT, 0))
ShowMessage("Не удалось перезагрузить компьютер");
*/
    int index = ui->options->currentIndex();
    Option option = static_cast < Option > ( ui->options->itemData( index ).toInt() );
    switch ( option )
    {
    case ToQuitProgram:
    {
        emit about_QuitProgram();
        close();
        break;
    }

    case Force:
    {
        ExitWindowsEx( EWX_FORCE, 0 );
        break;
    }

    case PowerOff:
    case Reboot:
    case Shutdown:
    {
        SystemControl( option );
        break;
    }

    case LogOff:
    {
        ExitWindowsEx( EWX_LOGOFF, 0 );
        break;
    }

    case ScreenOff:
    {
/*
//Погасить монитор:
SendMessage(Application->Handle, WM_SYSCOMMAND, SC_MONITORPOWER, 0);
//Перевести монитор в состояние Low Power:
SendMessage(Application->Handle, WM_SYSCOMMAND, SC_MONITORPOWER, 1);
//Перевести монитор в состояние Power Off:
SendMessage(Application->Handle, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
//Активировать монитор:
SendMessage(Application->Handle, WM_SYSCOMMAND, SC_MONITORPOWER, -1);
*/
        SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
        break;
    }

    }
}

bool MainWindow::eventFilter( QObject * object, QEvent * event )
{
    if ( object == ui->label_completionOfWork )
    {
        if ( event->type() == QEvent::MouseButtonRelease )
            ui->completionOfWork->setChecked( !ui->completionOfWork->isChecked() );        

        return false;
    }

    return QMainWindow::eventFilter( object, event );
}
