#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

#include "winsystemcommand.h"

enum Option
{
    ToQuitProgram // Закрытие Планировщика и отсылка определенного сигнала.
};

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
        ui->options->addItem( tr( "Power off" ),           WinSystemCommand::PowerOff );
        ui->options->addItem( tr( "Reboot" ),              WinSystemCommand::Reboot  );
        ui->options->addItem( tr( "Force" ),               WinSystemCommand::Force );
        ui->options->addItem( tr( "Shutdown" ),            WinSystemCommand::Shutdown );
        ui->options->addItem( tr( "Log off" ),             WinSystemCommand::LogOff );
        ui->options->addItem( tr( "Hibernate" ),           WinSystemCommand::Hibernate );
        ui->options->addItem( tr( "Suspend" ),             WinSystemCommand::Suspend );
        ui->options->addItem( tr( "Screen off" ),          WinSystemCommand::ScreenOff );

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
    const QDialogButtonBox::ButtonRole role = ui->buttonBox->buttonRole( button );
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
    int index = ui->options->currentIndex();
    int option = ui->options->itemData( index ).toInt();
    if ( option == ToQuitProgram )
    {
        emit about_QuitProgram();
        close();

    } else
    {
        WinSystemCommand::Command command = static_cast < WinSystemCommand::Command > ( option );
        bool successful = WinSystemCommand::execute( command );
        if ( !successful )
        {
            const QString & message = WinSystemCommand::getTextError( command );
            QMessageBox::warning( this, tr( "Warning" ), message );
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
