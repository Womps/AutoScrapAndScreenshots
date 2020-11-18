#ifndef MAINWINDOWWIDGET_H
#define MAINWINDOWWIDGET_H

#pragma warning(push)
#pragma warning(disable : 5054)
#include <QtWidgets/QWidget>
#pragma warning(pop)
#include <QtCore/QHash>
#include <QtCore/QString>

class QProcess;
class QLineEdit;
class QTreeView;
class QProgressDialog;
class QPushButton;
class QNetworkAccessManager;
class QNetworkReply;
class UniqueUrlStandardModel;

class MainWindowWidget final : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowWidget(QWidget* const pParent = nullptr);
    ~MainWindowWidget() = default;

signals:
    void LogMessage(const QString& sMessage, const int nLogLevel);

public slots:
    void onConfigureCrawler();

private slots:
    void onStartScan();
    void onBrowseCSVFile();
    void onAddSelectedUrlButtonClicked() const;
    void onAddAllUrlButtonClicked() const;
    void onRemoveSelectedUrlButtonClicked() const;
    void onRemoveAllUrlButtonClicked() const;
    void onStartScreenShots();
    void onBrowseOutputDirectory();
    void onScreenShotReceived(QNetworkReply* const pReply);

private:
    void initRunner(const QString& sCrawlerAbsolutePath);
    void openFileAndLoadData(const QString& sAbsoluteFilePath);
    void startScan(const QString& sUrlToScan);
    void updateStartScreenShotButton() const;

private:
    QHash<QString, QString> _hScreenShotsParamsByName;
    QString _sExportTmpDirPath;
    QProcess* _pCrawlRunner = nullptr;
    QLineEdit* _pUrlLineEdit = nullptr;
    QTreeView* _pCrawledUrlTreeView = nullptr;
    QTreeView* _pUrlToScreenTreeView = nullptr;
    QNetworkAccessManager* _pNetMgr = nullptr;
    UniqueUrlStandardModel* _pCrawledUrlModel = nullptr;
    UniqueUrlStandardModel* _pUrlToScreenModel = nullptr;
    QProgressDialog* _pProgressDialog = nullptr;
    QLineEdit* _pScreenShotsDirectory = nullptr;
    QPushButton* _pStartScreenShotsButton = nullptr;
    QLineEdit* _pScreenShotAccessKeyApi = nullptr;
    int _nScreenShotTaken = 0;
    QHash<QString, QPair<QString, int>> _hUserAgentsAndStandardWidth;
};

#endif // MAINWINDOWWIDGET_H
