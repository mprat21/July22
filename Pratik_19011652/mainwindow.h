#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LPT/Layeredprovingtree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_MainWindow_iconSizeChanged(const QSize &iconSize);
    void startVerification();

    void on_MainWindow_tabifiedDockWidgetActivated(QDockWidget *dockWidget);

    void on_MainWindow_toolButtonStyleChanged(const Qt::ToolButtonStyle &toolButtonStyle);

private:
    Ui::MainWindow *ui;
    LPT::LayeredProvingTree *lpt;
};
#endif // MAINWINDOW_H
