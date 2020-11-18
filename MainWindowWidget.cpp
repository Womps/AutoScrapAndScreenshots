#include "MainWindowWidget.h"
#include "CsvParser.h"
#include "UniqueUrlStandardModel.h"
#pragma warning(push)
#pragma warning(disable : 4251)
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFileDialog>
#include <QtCore/QProcess>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkReply>
#pragma warning(pop)
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QProgressDialog>
#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QtCore/QPair>
#include <QtCore/QStandardPaths>
#include <QtCore/QSettings>
#include <QtGui/QIcon>
#include <QtGui/QRegularExpressionValidator>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <iostream>
#include <string>

MainWindowWidget::MainWindowWidget(QWidget* const pParent)
    : QWidget(pParent)
    , _pCrawlRunner(new QProcess(this))
    , _pNetMgr(new QNetworkAccessManager(this))
{
    _hUserAgentsAndStandardWidth =
    {
        {QStringLiteral("Galaxy S9/S9+ Android 7.0"), QPair(QStringLiteral("Mozilla/5.0 (Linux; Android 7.0; SM-G892A Build/NRD90M; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/67.0.3396.87 Mobile Safari/537.36"), 360)},
        {QStringLiteral("iPhone X/XS iOS 12"), QPair(QStringLiteral("Mozilla/5.0 (iPhone; CPU iPhone OS 12_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/12.0 Mobile/15A372 Safari/604.1"), 375)},
        {QStringLiteral("iPad"), QPair(QStringLiteral("Mozilla/5.0 (iPad; CPU OS 11_0 like Mac OS X) AppleWebKit/604.1.34 (KHTML, like Gecko) Version/11.0 Mobile/15A5341f Safari/604.1"), 768)},
        {QStringLiteral("Nexus 10 Android 6.0.1"), QPair(QStringLiteral("Mozilla/5.0 (Linux; Android 6.0.1; Nexus 10 Build/MOB31T) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.87 Safari/537.36"), 800)}
    };

    const QSettings settings(QSettings(QStringLiteral("Kick-Digital"), QStringLiteral("AutoScrapAndScreenshots")));
    //---------------------------------------------------------------------------------------------------------------------------
    //              Screen Shots Parameters Setup
    //---------------------------------------------------------------------------------------------------------------------------
    QGroupBox* const pScreenShotParamsGroupBox = new QGroupBox(tr("Screenshots Parameters"), this);
    QGridLayout* const pScreenShotsParamsLayout = new QGridLayout(pScreenShotParamsGroupBox);
    QCheckBox* const pScreenShotFullPage = new QCheckBox(tr("Full Page Screenshot"), pScreenShotParamsGroupBox);
    QCheckBox* const pScreenShotScrollPage = new QCheckBox(tr("Scroll before screenshot"), pScreenShotParamsGroupBox);
    QCheckBox* const pScreenShotForceFresh = new QCheckBox(tr("Force fresh capture"), pScreenShotParamsGroupBox);
    QSpinBox* const pScreenShotWidthSpinBox = new QSpinBox(pScreenShotParamsGroupBox);
    QSpinBox* const pScreenShotHeightSpinBox = new QSpinBox(pScreenShotParamsGroupBox);
    QSpinBox* const pScreenShotDelaySpinBox = new QSpinBox(pScreenShotParamsGroupBox);
    QComboBox* const pScreenShotFormatComboBox = new QComboBox(pScreenShotParamsGroupBox);
    _pScreenShotsDirectory = new QLineEdit(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), pScreenShotParamsGroupBox);
    QPushButton* const pBrowseOutputDirButton = new QPushButton(tr("Browse..."), pScreenShotParamsGroupBox);
    _pStartScreenShotsButton = new QPushButton(tr("Start Screen shots"), pScreenShotParamsGroupBox);
    _pScreenShotAccessKeyApi = new QLineEdit(pScreenShotParamsGroupBox);
    QComboBox* const pUserAgentSelector = new QComboBox(pScreenShotParamsGroupBox);
    pScreenShotParamsGroupBox->setObjectName(QStringLiteral("screenShotsParamsGroupBox"));
    pScreenShotsParamsLayout->setObjectName(QStringLiteral("screenShotsParamsLayout"));
    pScreenShotFullPage->setObjectName(QStringLiteral("screenShotsParamsFullPageCheckBox"));
    pScreenShotScrollPage->setObjectName(QStringLiteral("screenShotsParamsScrollPageCheckBox"));
    pScreenShotForceFresh->setObjectName(QStringLiteral("screenShotsParamsForceCheckBox"));
    pScreenShotWidthSpinBox->setObjectName(QStringLiteral("screenShotsParamsWidthSpinBox"));
    pScreenShotHeightSpinBox->setObjectName(QStringLiteral("screenShotsParamsHeightSpinBox"));
    pScreenShotDelaySpinBox->setObjectName(QStringLiteral("screenShotsParamsDelaySpinBox"));
    pScreenShotFormatComboBox->setObjectName(QStringLiteral("screenShotsParamsFormatComboBox"));
    _pScreenShotsDirectory->setObjectName(QStringLiteral("screenShotsParamsDirectoryLineEdit"));
    pBrowseOutputDirButton->setObjectName(QStringLiteral("screenShotsParamsOutputDirButton"));
    _pStartScreenShotsButton->setObjectName(QStringLiteral("screenShotsParamsStartButton"));
    _pScreenShotAccessKeyApi->setObjectName(QStringLiteral("screenShotsParamsAccessKeyApi"));
    pUserAgentSelector->setObjectName(QStringLiteral("screenShotsParamsUserAgentSelector"));

    connect(pScreenShotFullPage, &QCheckBox::stateChanged, [this, pScreenShotHeightSpinBox](const int nState)
    {
        const bool bChecked = nState == Qt::CheckState::Checked;
        if(bChecked)
        {
            _hScreenShotsParamsByName[QStringLiteral("full_page")] = QStringLiteral("true");
            _hScreenShotsParamsByName.remove(QStringLiteral("height"));
        }
        else
        {
            _hScreenShotsParamsByName[QStringLiteral("full_page")] = QStringLiteral("false");
            _hScreenShotsParamsByName[QStringLiteral("height")] = QString::number(pScreenShotHeightSpinBox->value());
        }
        pScreenShotHeightSpinBox->setDisabled(bChecked);
    });
    connect(pScreenShotScrollPage, &QCheckBox::stateChanged, [this](const int nState){_hScreenShotsParamsByName[QStringLiteral("scroll_page")] = (nState == Qt::CheckState::Checked ? QStringLiteral("true") : QStringLiteral("false"));});
    connect(pScreenShotForceFresh, &QCheckBox::stateChanged, [this](const int nState){_hScreenShotsParamsByName[QStringLiteral("fresh")] = (nState == Qt::CheckState::Checked ? QStringLiteral("true") : QStringLiteral("false"));});
    connect(pScreenShotWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this, pUserAgentSelector](const int nWidth) {_hScreenShotsParamsByName[QStringLiteral("width")] = QString::number(nWidth);});
    connect(pScreenShotHeightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](const int nHeight){_hScreenShotsParamsByName[QStringLiteral("height")] = QString::number(nHeight);});
    connect(pScreenShotFormatComboBox, &QComboBox::currentTextChanged, [this](const QString& sNewText){_hScreenShotsParamsByName[QStringLiteral("format")] = sNewText;});
    connect(pBrowseOutputDirButton, &QPushButton::clicked, this, &MainWindowWidget::onBrowseOutputDirectory);
    connect(_pStartScreenShotsButton, &QPushButton::clicked, this, &MainWindowWidget::onStartScreenShots);
    connect(_pScreenShotAccessKeyApi, &QLineEdit::textChanged, [](const QString& sNewText)
    {
        QSettings settings(QSettings(QStringLiteral("Kick-Digital"), QStringLiteral("AutoScrapAndScreenshots")));
        settings.setValue(QStringLiteral("API_Access_Key"), sNewText);
    });
    connect(pUserAgentSelector, &QComboBox::currentTextChanged, [this, pScreenShotWidthSpinBox](const QString& sNewDevice)
    {
        const auto& UAAndWidthIterator = _hUserAgentsAndStandardWidth.constFind(sNewDevice);

        if (UAAndWidthIterator != _hUserAgentsAndStandardWidth.constEnd())
        {
            const QPair<QString, int>& pairUAAndWidth = UAAndWidthIterator.value();
            _hScreenShotsParamsByName[QStringLiteral("user_agent")] = pairUAAndWidth.first;
            pScreenShotWidthSpinBox->setValue(pairUAAndWidth.second);
        }
        else
        {
            _hScreenShotsParamsByName.remove(QStringLiteral("user_agent"));
            pScreenShotWidthSpinBox->setValue(1920);
        }
    });

    pScreenShotFullPage->setToolTip(tr("Enable to take a full height screenshot."));
    pScreenShotScrollPage->setToolTip(tr("Enable to scroll through the entire page before capturing a screenshot. This is useful to trigger animations, or lazy loaded elements."));
    pScreenShotForceFresh->setToolTip(tr("Enable to refresh cached screenshot."));
    pScreenShotWidthSpinBox->setMaximum(9999);
    pScreenShotWidthSpinBox->setMinimum(128);
    pScreenShotWidthSpinBox->setValue(1920);
    pScreenShotWidthSpinBox->setToolTip(tr("In pixels"));
    pScreenShotHeightSpinBox->setMaximum(9999);
    pScreenShotHeightSpinBox->setMinimum(128);
    pScreenShotHeightSpinBox->setToolTip(tr("In pixels"));
    pScreenShotDelaySpinBox->setMaximum(10);
    pScreenShotDelaySpinBox->setMinimum(0);
    pScreenShotDelaySpinBox->setToolTip(tr("In seconds"));
    pScreenShotFormatComboBox->addItems({QStringLiteral("jpeg"), QStringLiteral("png")});
    _pScreenShotsDirectory->setReadOnly(true);
    _pScreenShotsDirectory->setDisabled(true);
    _pStartScreenShotsButton->setDisabled(true);
    pScreenShotFullPage->setCheckState(Qt::CheckState::Checked);
    _pScreenShotAccessKeyApi->setText(settings.value(QStringLiteral("API_Access_Key")).toString());
    pUserAgentSelector->addItems({QString(), QStringLiteral("Galaxy S9/S9+ Android 7.0"), QStringLiteral("iPhone X/XS iOS 12"), QStringLiteral("iPad"), QStringLiteral("Nexus 10 Android 6.0.1")});

    pScreenShotsParamsLayout->addWidget(new QLabel(tr("API Acess Key :"), pScreenShotParamsGroupBox), 0, 0);
    pScreenShotsParamsLayout->addWidget(_pScreenShotAccessKeyApi, 0, 1, 1, 2);
    pScreenShotsParamsLayout->addWidget(new QLabel(tr("Device :"), pScreenShotParamsGroupBox), 0, 3);
    pScreenShotsParamsLayout->addWidget(pUserAgentSelector, 0, 4);
    pScreenShotsParamsLayout->addWidget(pScreenShotFullPage, 1, 0);
    pScreenShotsParamsLayout->addWidget(pScreenShotScrollPage, 1, 1);
    pScreenShotsParamsLayout->addWidget(pScreenShotForceFresh, 1, 2);
    pScreenShotsParamsLayout->addWidget(new QLabel(tr("Delay :"), pScreenShotParamsGroupBox), 1, 3);
    pScreenShotsParamsLayout->addWidget(pScreenShotDelaySpinBox, 1, 4);
    pScreenShotsParamsLayout->addWidget(new QLabel(tr("Width :"), pScreenShotParamsGroupBox), 2, 0);
    pScreenShotsParamsLayout->addWidget(pScreenShotWidthSpinBox, 2, 1);
    pScreenShotsParamsLayout->addWidget(new QLabel(tr("Height :"), pScreenShotParamsGroupBox), 2, 2);
    pScreenShotsParamsLayout->addWidget(pScreenShotHeightSpinBox, 2, 3);
    pScreenShotsParamsLayout->addWidget(pScreenShotFormatComboBox, 2, 4);
    pScreenShotsParamsLayout->addWidget(_pScreenShotsDirectory, 3, 0, 1, 3);
    pScreenShotsParamsLayout->addWidget(pBrowseOutputDirButton, 3, 3);
    pScreenShotsParamsLayout->addWidget(_pStartScreenShotsButton, 3, 4);

    //---------------------------------------------------------------------------------------------------------------------------
    //              URL crawler Setup
    //---------------------------------------------------------------------------------------------------------------------------
    QGridLayout* const pGlobalGridLayout = new QGridLayout(this);
    QVBoxLayout* const pAddRemoveUrlButtonsLayout = new QVBoxLayout(); // Must add into another layout => will be added to the global grid layout.
    QHBoxLayout* const pCrawlerLayout = new QHBoxLayout(); // Must add into another layout
    QPushButton* const pStartScanPushButton = new QPushButton(tr("Start Scan"), this);
    QPushButton* const pBrowseCsvFile = new QPushButton(tr("Browse..."), this);
    QPushButton* const pAddUrlToScreenShot = new QPushButton(this);
    QPushButton* const pAddAllUrlToScreenShot = new QPushButton(this);
    QPushButton* const pRemoveUrlFromScreenShot = new QPushButton(this);
    QPushButton* const pRemoveAllUrlFromScreenShot = new QPushButton(this);
    _pUrlLineEdit = new QLineEdit(this);
    _pCrawledUrlTreeView = new QTreeView(this);
    _pUrlToScreenTreeView = new QTreeView(this);
    _pCrawledUrlModel = new UniqueUrlStandardModel(this);
    _pUrlToScreenModel = new UniqueUrlStandardModel(this);
    pGlobalGridLayout->setObjectName(QStringLiteral("centralWidgetGlobaGridLayout"));
    pAddRemoveUrlButtonsLayout->setObjectName(QStringLiteral("addOrRemoveButtonsVBoxLayout"));
    pStartScanPushButton->setObjectName(QStringLiteral("urlCrawlerStartScanButton"));
    pBrowseCsvFile->setObjectName(QStringLiteral("urlCrawlerBrowseCsvFileButton"));
    pAddUrlToScreenShot->setObjectName(QStringLiteral("urlCrawlerAddUrlToScreenShotButton"));
    pAddAllUrlToScreenShot->setObjectName(QStringLiteral("urlCrawlerAddAllUrlToScreenShotButton"));
    pRemoveUrlFromScreenShot->setObjectName(QStringLiteral("urlCrawlerRemoveUrlFromScreenShotsButton"));
    pRemoveAllUrlFromScreenShot->setObjectName(QStringLiteral("urlCrawlerRemoveAllUrlFromScreenShotsButton"));
    _pUrlLineEdit->setObjectName(QStringLiteral("urlCrawlerUrlLineEdit"));
    _pUrlToScreenTreeView->setObjectName(QStringLiteral("urlCrawlerUrlToScreenShotTreeView"));
    _pCrawledUrlTreeView->setObjectName(QStringLiteral("urlCrawlerCrawledUrlsTreeView"));

    connect(_pUrlLineEdit, &QLineEdit::returnPressed, this, &MainWindowWidget::onStartScan);
    connect(pStartScanPushButton, &QPushButton::clicked, this, &MainWindowWidget::onStartScan);
    connect(pBrowseCsvFile, &QPushButton::clicked, this, &MainWindowWidget::onBrowseCSVFile);
    connect(_pNetMgr, &QNetworkAccessManager::finished, this, &MainWindowWidget::onScreenShotReceived);
    connect(pAddUrlToScreenShot, &QPushButton::clicked, this, &MainWindowWidget::onAddSelectedUrlButtonClicked);
    connect(pAddAllUrlToScreenShot, &QPushButton::clicked, this, &MainWindowWidget::onAddAllUrlButtonClicked);
    connect(pRemoveUrlFromScreenShot, &QPushButton::clicked, this, &MainWindowWidget::onRemoveSelectedUrlButtonClicked);
    connect(pRemoveAllUrlFromScreenShot, &QPushButton::clicked, this, &MainWindowWidget::onRemoveAllUrlButtonClicked);

    pAddUrlToScreenShot->setIcon(QIcon(QStringLiteral(":/Resources/Icons/forward-32.png")));
    pAddAllUrlToScreenShot->setIcon(QIcon(QStringLiteral(":/Resources/Icons/double-forward-32.png")));
    pRemoveUrlFromScreenShot->setIcon(QIcon(QStringLiteral(":/Resources/Icons/backward-32.png")));
    pRemoveAllUrlFromScreenShot->setIcon(QIcon(QStringLiteral(":/Resources/Icons/double-backward-32.png")));

    _pCrawledUrlTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    _pCrawledUrlTreeView->setAlternatingRowColors(true);
    _pCrawledUrlTreeView->setRootIsDecorated(false);
    _pCrawledUrlTreeView->setModel(_pCrawledUrlModel);
    _pCrawledUrlModel->setHorizontalHeaderLabels({tr("Crawled URL")});

    _pUrlToScreenTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    _pUrlToScreenTreeView->setAlternatingRowColors(true);
    _pUrlToScreenTreeView->setRootIsDecorated(false);
    _pUrlToScreenTreeView->setModel(_pUrlToScreenModel);
    _pUrlToScreenModel->setHorizontalHeaderLabels({tr("URL to Screenshot")});

    _pUrlLineEdit->setPlaceholderText(tr("Type (or paste) an URL here..."));
    _pUrlLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(QStringLiteral("^https?:\\/\\/\\S+"), QRegularExpression::CaseInsensitiveOption), this));

    connect(_pCrawlRunner, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [this](const int, QProcess::ExitStatus exitStatus)
    {
        if(_pProgressDialog)
        {
            if(exitStatus == QProcess::NormalExit)
            {
                _pProgressDialog->done(QDialog::Accepted);
                openFileAndLoadData(_sExportTmpDirPath + QDir::separator() + QStringLiteral("internal_html.csv"));
            }

            disconnect(_pProgressDialog, &QProgressDialog::canceled, _pCrawlRunner, &QProcess::kill);
            delete _pProgressDialog;
            _pProgressDialog = nullptr;
        }
    });

    pCrawlerLayout->addWidget(_pUrlLineEdit);
    pCrawlerLayout->addWidget(pStartScanPushButton);
    pCrawlerLayout->addWidget(new QLabel(tr("Or"), this));
    pCrawlerLayout->addWidget(pBrowseCsvFile);

    pAddRemoveUrlButtonsLayout->addWidget(pAddUrlToScreenShot);
    pAddRemoveUrlButtonsLayout->addWidget(pAddAllUrlToScreenShot);
    pAddRemoveUrlButtonsLayout->addWidget(pRemoveAllUrlFromScreenShot);
    pAddRemoveUrlButtonsLayout->addWidget(pRemoveUrlFromScreenShot);

    pGlobalGridLayout->addLayout(pCrawlerLayout, 0, 0, 1, -1);
    pGlobalGridLayout->addWidget(_pCrawledUrlTreeView, 1, 0);
    pGlobalGridLayout->addLayout(pAddRemoveUrlButtonsLayout, 1, 1);
    pGlobalGridLayout->addWidget(_pUrlToScreenTreeView, 1, 2);
    pGlobalGridLayout->addWidget(pScreenShotParamsGroupBox, 2, 0, 1, -1);

    const QString& sConfiguredCrawlerExecutable = settings.value(QStringLiteral("ScreamingFrogSEOSpider_Executable")).toString();
    const QFileInfo configuredCrawler(sConfiguredCrawlerExecutable);
    if(configuredCrawler.exists())
    {
        initRunner(configuredCrawler.absoluteFilePath());
    }
}

void MainWindowWidget::onConfigureCrawler()
{
#ifdef _WIN32
    const QString& sBinaryExtension = QStringLiteral("exe");
#elif __APPLE__
    const QString& sBinaryExtension = QStringLiteral("dmg");
#endif
    QSettings settings(QStringLiteral("Kick-Digital"), QStringLiteral("AutoScrapAndScreenshots"));
    QFileInfo absPathToBinaryFile(settings.value(QStringLiteral("ScreamingFrogSEOSpider_Executable")).toString());

    if(!absPathToBinaryFile.exists())
    {
        absPathToBinaryFile = QFileDialog::getOpenFileName(this, tr("Select crawler executable"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), tr("*Cli.%1").arg(sBinaryExtension));
    }

    if(absPathToBinaryFile.exists())
    {
        const QString& sAbsPathToCrawlerBinary = absPathToBinaryFile.absoluteFilePath();
        settings.setValue(QStringLiteral("ScreamingFrogSEOSpider_Executable"), sAbsPathToCrawlerBinary);
        initRunner(sAbsPathToCrawlerBinary);
    }
    else
    {
        QMessageBox::critical(this, tr("Crawler Installation Error"), tr("Can't find crawler binary in given directory."));
    }
}

void MainWindowWidget::onStartScan()
{
    const QString& sEnteredUrl = _pUrlLineEdit->text();
    const QUrl urlValidator(sEnteredUrl);

    if(urlValidator.isValid())
    {
        startScan(sEnteredUrl);
    }
    else
    {
        QMessageBox::critical(this, tr("URL Error"), tr("You must provide a valid URL (check appropriate field)."));
    }
}

void MainWindowWidget::onBrowseCSVFile()
{
    const QString& sCsvFilePath = QFileDialog::getOpenFileName(this, tr("Select CSV file"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), tr("*.csv"));

    if(!sCsvFilePath.isEmpty())
    {
        openFileAndLoadData(sCsvFilePath);
    }
}

void MainWindowWidget::onAddSelectedUrlButtonClicked() const
{
    const QItemSelection& userSelection = _pCrawledUrlTreeView->selectionModel()->selection();

    for(const QItemSelectionRange& selectionRange : userSelection)
    {
        const QModelIndexList& selectedIndexes = selectionRange.indexes();
        for(const QModelIndex& selectedIndex : selectedIndexes)
        {
            const QString& sUrl = selectedIndex.data(Qt::DisplayRole).toString();

            if(!_pUrlToScreenModel->contains(sUrl))
            {
                _pUrlToScreenModel->appendRow(new QStandardItem(sUrl));
            }
        }
    }

    updateStartScreenShotButton();
}

void MainWindowWidget::onAddAllUrlButtonClicked() const
{
    const int nUrlCount = _pCrawledUrlModel->rowCount();

    for(int nUrl = 0; nUrl < nUrlCount; ++nUrl)
    {
        const QModelIndex& crawledUrlIndex = _pCrawledUrlModel->index(nUrl, 0);
        const QString& sUrl = crawledUrlIndex.data(Qt::DisplayRole).toString();

        if(!_pUrlToScreenModel->contains(sUrl))
        {
            _pUrlToScreenModel->appendRow(new QStandardItem(sUrl));
        }
    }

    updateStartScreenShotButton();
}

void MainWindowWidget::onRemoveSelectedUrlButtonClicked() const
{
    const QItemSelection& userSelection = _pUrlToScreenTreeView->selectionModel()->selection();

    for(const QItemSelectionRange& selectionRange : userSelection)
    {
        const QModelIndexList& selectedIndexes = selectionRange.indexes();
        for(const QModelIndex& selectedIndex : selectedIndexes)
        {
            if(selectedIndex.isValid())
            {
                _pUrlToScreenModel->removeRow(selectedIndex.row());
            }
        }
    }

    updateStartScreenShotButton();
}

void MainWindowWidget::onRemoveAllUrlButtonClicked() const
{
    _pUrlToScreenModel->removeRows(0, _pUrlToScreenModel->rowCount());
    updateStartScreenShotButton();
}

void MainWindowWidget::onStartScreenShots()
{
    const QString& sApiAccessKey = _pScreenShotAccessKeyApi->text();
    if (sApiAccessKey.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("API Access Key is empty."));
        return;
    }
    _pStartScreenShotsButton->setEnabled(false);
    _nScreenShotTaken = 0;
    QUrl url(QStringLiteral("https://api.apiflash.com/v1/urltoimage"));
    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    const auto& screenShotsParamsBeginIt = _hScreenShotsParamsByName.constBegin();
    const auto& screenShotsParamsEndIt = _hScreenShotsParamsByName.constEnd();
    const int nUrlToScreenShotCount = _pUrlToScreenModel->rowCount();
    const QDir outputDir(_pScreenShotsDirectory->text());
    QStringList lInvalidUrls;
    _pProgressDialog = new QProgressDialog(tr("Capturing screenshots..."), tr("Cancel"), 0, nUrlToScreenShotCount, this);
    _pProgressDialog->setWindowModality(Qt::WindowModal);
    connect(_pProgressDialog, &QProgressDialog::finished, [this](const int){_pStartScreenShotsButton->setEnabled(true);_pProgressDialog->deleteLater();});

    for(int nUrl = 0; nUrl < nUrlToScreenShotCount; ++nUrl)
    {
        const QUrl urlToScreenShot = _pUrlToScreenModel->index(nUrl, 0).data(Qt::DisplayRole).toString();
        const QString& sUrlToScreenShot = urlToScreenShot.toString();
        const QString& sUrlHostName = urlToScreenShot.host();

        if(!urlToScreenShot.isValid() || sUrlHostName.isEmpty())
        {
            lInvalidUrls << sUrlToScreenShot;
            continue;
        }

        if (outputDir.mkpath(sUrlHostName))
        {
            QUrlQuery query({QPair(QStringLiteral("access_key"), sApiAccessKey), QPair(QStringLiteral("url"), QUrl::toPercentEncoding(sUrlToScreenShot)), QPair(QStringLiteral("fail_on_status"), QStringLiteral("400-456,444,495-499,500-511"))});

            for(auto screenShotsParamsIt =  screenShotsParamsBeginIt; screenShotsParamsIt != screenShotsParamsEndIt; ++screenShotsParamsIt)
            {
                const QString& sParamValue = screenShotsParamsIt.value();

                if (!sParamValue.isEmpty())
                {
                    query.addQueryItem(screenShotsParamsIt.key(), sParamValue);
                }
                
                qDebug() << screenShotsParamsIt.key() << ":" << sParamValue;
            }
            url.setQuery(query);
            request.setUrl(url);
            _pNetMgr->get(request);
        }
    }

    if(!lInvalidUrls.isEmpty())
    {
        QString sWarningMessage = tr("Some URL from your screenshot list couldn't be captured.");

        for(const QString& sInvalidUrl : lInvalidUrls)
        {
            sWarningMessage.append(QStringLiteral("\n") + sInvalidUrl);
        }
        QMessageBox::warning(this, tr("Warning"), sWarningMessage);
    }
}

void MainWindowWidget::onBrowseOutputDirectory()
{
    const QString& sOutputDirectory = QFileDialog::getExistingDirectory(this, tr("Select directory where to save screenshots"), _pScreenShotsDirectory->text());

    if(!sOutputDirectory.isEmpty())
    {
        _pScreenShotsDirectory->setText(sOutputDirectory);
    }
}

void MainWindowWidget::onScreenShotReceived(QNetworkReply* const pReply)
{
    const QNetworkReply::NetworkError eError = pReply->error();
    const QUrl& urlRequested = pReply->request().url();
    std::string sErrorMessage;

    switch(eError)
    {
        case QNetworkReply::NoError:
        {
            const QByteArray& imageData = pReply->readAll();
            QPixmap screenShot;
            screenShot.loadFromData(imageData);
            const QUrl& urlToCaptureRequested = QUrlQuery(urlRequested).queryItemValue(QStringLiteral("url"), QUrl::FullyDecoded);
            QString sRequestedPath = urlToCaptureRequested.path();
            const QString& sHostName = urlToCaptureRequested.host();
            const QString& sUrlPath = sRequestedPath.remove(QRegularExpression(QStringLiteral("[\\/\\\\]+")));
            const QString& sUrlPathCleaned = sUrlPath.isEmpty() ? QStringLiteral("home") : sUrlPath;
            const QString& sAbsoluteFileName = _pScreenShotsDirectory->text() + QDir::separator() + sHostName + QDir::separator() + sUrlPathCleaned + QStringLiteral(".%1").arg(_hScreenShotsParamsByName[QStringLiteral("format")]);
            screenShot.save(sAbsoluteFileName);
            std::cout << "INFO : " << sAbsoluteFileName.toStdString() << " wrote." << std::endl;
            break;
        }
        case QNetworkReply::AuthenticationRequiredError:
        {
            sErrorMessage = "Wrong access key provided.";
            break;
        }
        default:
        {
            sErrorMessage = pReply->readAll().toStdString() + "\n\t => Request sent : " + urlRequested.toString().toStdString();
            break;
        }
    }

    if(!sErrorMessage.empty())
    {
        std::cerr << "ERROR : " << sErrorMessage << std::endl;
    }

    _pProgressDialog->setValue(++_nScreenShotTaken);
    pReply->deleteLater();
}

void MainWindowWidget::initRunner(const QString& sCrawlerAbsolutePath)
{
    _pCrawlRunner->setProgram(sCrawlerAbsolutePath);

    const QString& sTmpDirPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if(!sTmpDirPath.isEmpty())
    {
        const QString& sExportTmpDirPath = QDir::toNativeSeparators(sTmpDirPath) + QDir::separator() + QStringLiteral("AutoCrawlExportDir");

        const QDir tmpDir;
        if(!tmpDir.mkpath(sExportTmpDirPath))
        {
            QMessageBox::critical(this, tr("Couldn't make export path"), tr("Error while creating temporary export directory."));
            return;
        }

        _sExportTmpDirPath = sExportTmpDirPath;
    }
}

void MainWindowWidget::openFileAndLoadData(const QString& sAbsoluteFilePath)
{
    QFile csvFileToLoad(sAbsoluteFilePath);

    if(csvFileToLoad.open(QIODevice::ReadOnly))
    {
        _pCrawledUrlModel->removeRows(0, _pCrawledUrlModel->rowCount());
        _pUrlToScreenModel->removeRows(0, _pUrlToScreenModel->rowCount());
        updateStartScreenShotButton();
        QStringList vFields;
        CSV::fillListWithDataFromColumn(vFields, csvFileToLoad.readAll(), 0);

        for(const QString& sData : vFields)
        {
            if(sData.startsWith(QStringLiteral("http"), Qt::CaseInsensitive) && !_pCrawledUrlModel->contains(sData))
            {
                _pCrawledUrlModel->appendRow(new QStandardItem(sData));
            }
        }
    }
}

void MainWindowWidget::startScan(const QString& sUrlToScan)
{
    if(!_sExportTmpDirPath.isEmpty() && !_pCrawlRunner->program().isEmpty())
    {
        if(_pCrawlRunner->state() == QProcess::NotRunning)
        {
            _pCrawlRunner->setNativeArguments(QStringLiteral("--crawl ") + sUrlToScan + QStringLiteral(" --headless --export-tabs \"Internal:HTML\" --overwrite --output-folder ") + _sExportTmpDirPath);
            _pCrawlRunner->start();
            _pProgressDialog = new QProgressDialog(tr("Crawling URL..."), tr("Cancel"), 0, 0, this);
            connect(_pProgressDialog, &QProgressDialog::canceled, _pCrawlRunner, &QProcess::kill);
            _pProgressDialog->setWindowModality(Qt::WindowModal);
            _pProgressDialog->exec();
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Can't start URL scan"), tr("Error while starting URL scan. Configure Screaming Frog SEO Spider installation (Tools menu)."));
    }
}

void MainWindowWidget::updateStartScreenShotButton() const
{
    _pStartScreenShotsButton->setEnabled(_pUrlToScreenModel->rowCount() > 0);
}
