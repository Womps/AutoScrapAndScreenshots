#ifndef MAINWINDOWWIDGET_H
#define MAINWINDOWWIDGET_H

#pragma warning(push, 0)
#include <QtWidgets/QWidget>
#include <QtCore/QHash>
#include <QtCore/QString>
#pragma warning(pop)

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
    int _nScreenShotTaken = 0;
    QHash<QString, QString> _hScreenShotsParamsByName;
    QString _sExportTmpDirPath;
    QProcess* const _pCrawlRunner = nullptr;
    QLineEdit* const _pUrlLineEdit = nullptr;
    QTreeView* const _pCrawledUrlTreeView = nullptr;
    QTreeView* const _pUrlToScreenTreeView = nullptr;
    QNetworkAccessManager* const _pNetMgr = nullptr;
    UniqueUrlStandardModel* const _pCrawledUrlModel = nullptr;
    UniqueUrlStandardModel* const _pUrlToScreenModel = nullptr;
    QProgressDialog* _pProgressDialog = nullptr;
    QLineEdit* _pScreenShotsDirectory = nullptr;
    QPushButton* _pStartScreenShotsButton = nullptr;
    QLineEdit* _pScreenShotAccessKeyApi = nullptr;
};

#endif // MAINWINDOWWIDGET_H
