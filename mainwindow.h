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
