#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma warning(push)
#pragma warning(disable : 5054)
#include <QtWidgets/QMainWindow>
#pragma warning(pop)

class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* const pParent = nullptr);
    ~MainWindow() = default;
};

#endif // MAINWINDOW_H
