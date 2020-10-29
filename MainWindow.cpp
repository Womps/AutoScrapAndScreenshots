#include "MainWindow.h"
#include "Application.h"
#include "MainWindowWidget.h"
#include "ConsoleLogsDisplay.h"
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QShortcut>
#include <QtWidgets/QDockWidget>
#pragma warning(push)
#pragma warning(disable : 4251)
#include <QtWidgets/QVBoxLayout>
#pragma warning(pop)

MainWindow::MainWindow(QWidget* const pParent)
    : QMainWindow(pParent)
{
    setObjectName(QStringLiteral("mainWindow"));
    setWindowTitle(tr("Auto Scrap and Screenshots"));

    MainWindowWidget* const pCentralWidget = new MainWindowWidget(this);
    QDockWidget* const pConsoleLogsDockWidget = new QDockWidget(tr("Console"), this);
    QWidget* const pConsoleLogsWidget = new QWidget(pConsoleLogsDockWidget);
    QVBoxLayout* const pConsoleLogsWidgetLayout = new QVBoxLayout(pConsoleLogsWidget);
    ConsoleLogsDisplay* const pConsoleLogsTextBrowser = new ConsoleLogsDisplay(pConsoleLogsWidget);

    pCentralWidget->setObjectName(QStringLiteral("mainWindowCentralWidget"));
    pConsoleLogsDockWidget->setObjectName(QStringLiteral("consoleLogsDockWidget"));
    pConsoleLogsWidget->setObjectName(QStringLiteral("consoleLogsWidget"));
    pConsoleLogsWidgetLayout->setObjectName(QStringLiteral("consoleLogsWidgetLayout"));
    pConsoleLogsTextBrowser->setObjectName(QStringLiteral("consoleLogsTextBrowser"));

    pConsoleLogsTextBrowser->setLineWrapMode(QTextEdit::NoWrap);
    pConsoleLogsWidgetLayout->addWidget(pConsoleLogsTextBrowser);
    pConsoleLogsDockWidget->setWidget(pConsoleLogsWidget);
    pConsoleLogsDockWidget->setVisible(false);

    setCentralWidget(pCentralWidget);
    addDockWidget(Qt::BottomDockWidgetArea, pConsoleLogsDockWidget);

    connect(qApp, &Application::logMessage, pConsoleLogsTextBrowser, &ConsoleLogsDisplay::insertMessage);

    const QShortcut* const pShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_D), this);
    connect(pShortcut, &QShortcut::activated, [pConsoleLogsDockWidget]{pConsoleLogsDockWidget->setVisible(!pConsoleLogsDockWidget->isVisible());});

    QMenu* const pToolsMenu = new QMenu(tr("Tools"), this);
    menuBar()->addMenu(pToolsMenu);
    pToolsMenu->addAction(tr("Configure Crawler Installation"), pCentralWidget, &MainWindowWidget::onConfigureCrawler);
}
