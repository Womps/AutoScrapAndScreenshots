#include "MainWindow.h"
#include "Application.h"
#include "MainWindowWidget.h"
#include "ConsoleLogsDisplay.h"
#pragma warning(push, 0)
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QShortcut>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QVBoxLayout>
#pragma warning(pop)

MainWindow::MainWindow(QWidget* const pParent)
    : QMainWindow(pParent)
{
    setObjectName(QStringLiteral("mainWindow"));
    setWindowTitle(tr("Auto Scrap and Screenshots"));
    QMenu* const pToolsMenu = new QMenu(tr("Tools"), this);
    menuBar()->addMenu(pToolsMenu);
    MainWindowWidget* const pCentralWidget = new MainWindowWidget(this);
    pCentralWidget->setObjectName(QStringLiteral("mainWindowCentralWidget"));
    pToolsMenu->addAction(tr("Configure Crawler Installation"), pCentralWidget, &MainWindowWidget::onConfigureCrawler);
    setCentralWidget(pCentralWidget);

    QDockWidget* const pConsoleLogsDockWidget = new QDockWidget(tr("Console"), this);
    QWidget* const pConsoleLogsWidget = new QWidget(pConsoleLogsDockWidget);
    QVBoxLayout* const pConsoleLogsWidgetLayout = new QVBoxLayout(pConsoleLogsWidget);
    ConsoleLogsDisplay* const pConsoleLogsTextBrowser = new ConsoleLogsDisplay(pConsoleLogsWidget);

    pConsoleLogsDockWidget->setObjectName(QStringLiteral("consoleLogsDockWidget"));
    pConsoleLogsWidget->setObjectName(QStringLiteral("consoleLogsWidget"));
    pConsoleLogsWidgetLayout->setObjectName(QStringLiteral("consoleLogsWidgetLayout"));
    pConsoleLogsTextBrowser->setObjectName(QStringLiteral("consoleLogsTextBrowser"));

    pConsoleLogsTextBrowser->setLineWrapMode(QTextEdit::NoWrap);

    pConsoleLogsWidgetLayout->addWidget(pConsoleLogsTextBrowser);
    pConsoleLogsDockWidget->setWidget(pConsoleLogsWidget);
    pConsoleLogsDockWidget->setVisible(false);

    addDockWidget(Qt::BottomDockWidgetArea, pConsoleLogsDockWidget);

    connect(qApp, &Application::logMessage, pConsoleLogsTextBrowser, &ConsoleLogsDisplay::insertMessage);

    const QShortcut* const pShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_D), this);
    connect(pShortcut, &QShortcut::activated, [pConsoleLogsDockWidget]{pConsoleLogsDockWidget->setVisible(!pConsoleLogsDockWidget->isVisible());});
}
