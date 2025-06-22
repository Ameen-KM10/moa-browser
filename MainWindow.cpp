#include "MainWindow.h"
#include <QToolBar>
#include <QHBoxLayout>
#include <QUrl>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    // Optimize global settings (Qt 6 style)
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, false);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, false);

    // Create tab widget
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);

    // Address bar
    addressBar = new QLineEdit(this);
    addressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Create new tab button
    newTabButton = new QPushButton("+", this);


    // Navigation buttons
    backButton = new QPushButton("←", this);
    forwardButton = new QPushButton("→", this);
    reloadButton = new QPushButton("⟳", this);
    devToolsButton = new QPushButton("DevTools", this);

    // Connect signals
    connect(addressBar, &QLineEdit::returnPressed, this, &MainWindow::loadPage);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);
    connect(forwardButton, &QPushButton::clicked, this, &MainWindow::onForwardButtonClicked);
    connect(reloadButton, &QPushButton::clicked, this, &MainWindow::onReloadButtonClicked);
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::switchTab);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(newTabButton, &QPushButton::clicked, this, &MainWindow::addNewTab);
    connect(devToolsButton, &QPushButton::clicked, this, [this]() {
        QWebEngineView* view = currentWebView();
        if (!view) return;

        // Create DevTools view and tab
        QWebEngineView* devToolsView = new QWebEngineView;
        int devToolsTabIndex = tabWidget->addTab(devToolsView, "DevTools");
        tabWidget->setCurrentIndex(devToolsTabIndex);

        // Link DevTools to the current page
        view->page()->setDevToolsPage(devToolsView->page());

        // Optional: set an icon for the DevTools tab
        // tabWidget->setTabIcon(devToolsTabIndex, QIcon(":/icons/devtools.svg"));
    });

    QShortcut* devToolsShortcut = new QShortcut(QKeySequence(Qt::Key_F12), this);
    connect(devToolsShortcut, &QShortcut::activated, this, [this]() {
        QWebEngineView* view = currentWebView();
        if (!view) return;

        QWebEngineView* devToolsView = new QWebEngineView;
        int devToolsTabIndex = tabWidget->addTab(devToolsView, "DevTools");
        tabWidget->setCurrentIndex(devToolsTabIndex);

        view->page()->setDevToolsPage(devToolsView->page());
    });

    // // Keyboard shortcuts
    // new QShortcut(QKeySequence(Qt::CTRL + Qt::T), this, SLOT(addNewTab()));
    // new QShortcut(QKeySequence(Qt::CTRL + Qt::W), this, SLOT(closeCurrentTab()));

    // Toolbar layout
    QWidget* toolWidget = new QWidget(this);
    QHBoxLayout* toolLayout = new QHBoxLayout(toolWidget);
    toolLayout->setSpacing(0);
    toolLayout->setContentsMargins(4, 4, 4, 4);
    toolLayout->addWidget(backButton);
    toolLayout->addWidget(forwardButton);
    toolLayout->addWidget(reloadButton);
    toolLayout->addWidget(addressBar);
    toolLayout->addWidget(newTabButton);
    toolLayout->addWidget(devToolsButton);

    QToolBar* toolBar = new QToolBar("Navigation");
    toolBar->addWidget(toolWidget);
    toolBar->setMovable(false);
    addToolBar(toolBar);

    // Set central widget
    setCentralWidget(tabWidget);
    resize(1024, 768);
    setWindowTitle("Moa Browser");

    // Add initial tab
    addNewTab();
}

MainWindow::~MainWindow() {}

void MainWindow::addNewTab() {
    QWebEngineView *webView = new QWebEngineView(this);
    webView->setUrl(QUrl("https://google.com"));

    // Connect URL change signal for this tab
    connect(webView, &QWebEngineView::urlChanged, this, &MainWindow::updateAddressBar);
    connect(webView, &QWebEngineView::titleChanged, [this, webView](const QString &title) {
        int index = tabWidget->indexOf(webView);
        if (index != -1) {
            tabWidget->setTabText(index, title.isEmpty() ? webView->url().host() : title);
        }
    });

    int index = tabWidget->addTab(webView, "New Tab");
    tabWidget->setCurrentIndex(index);
    
    // Update address bar immediately for the new tab
    addressBar->setText(webView->url().toString());
}

void MainWindow::closeTab(int index) {
    if (tabWidget->count() > 1) {
        QWidget* widget = tabWidget->widget(index);
        tabWidget->removeTab(index);
        delete widget;
    }
}

void MainWindow::switchTab(int index) {
    if (QWebEngineView* view = currentWebView()) {
        // Update address bar with current tab's URL
        addressBar->setText(view->url().toString());
    }
}

QWebEngineView* MainWindow::currentWebView() const {
    return qobject_cast<QWebEngineView*>(tabWidget->currentWidget());
}

void MainWindow::loadPage() {
    if (QWebEngineView* view = currentWebView()) {
        QString url = addressBar->text();
        if (!url.startsWith("http")) {
            url = "https://" + url;
        }
        view->setUrl(QUrl(url));
    }
}

void MainWindow::updateAddressBar(const QUrl &url) {
    // Only update if the signal comes from the current tab
    if (sender() == currentWebView()) {
        addressBar->setText(url.toString());
        // Update tab title with the page hostname
        int index = tabWidget->currentIndex();
        tabWidget->setTabText(index, url.host().isEmpty() ? "New Tab" : url.host());
    }
}

void MainWindow::onBackButtonClicked() {
    if (QWebEngineView* view = currentWebView()) {
        view->back();
    }
}

void MainWindow::onForwardButtonClicked() {
    if (QWebEngineView* view = currentWebView()) {
        view->forward();
    }
}

void MainWindow::onReloadButtonClicked() {
    if (QWebEngineView* view = currentWebView()) {
        view->reload();
    }
}

WebPage::WebPage(MainWindow *mainWindow) 
    : QWebEnginePage(mainWindow), m_mainWindow(mainWindow) {
}

QWebEnginePage* WebPage::createWindow(QWebEnginePage::WebWindowType type) {
    QWebEngineView *newView = new QWebEngineView();
    WebPage *newPage = new WebPage(m_mainWindow);
    newView->setPage(newPage);
    m_mainWindow->createWindowForTab(newPage);
    return newPage;
}

void MainWindow::setupWebPage(QWebEngineView *webView) {
    WebPage *page = new WebPage(this);
    webView->setPage(page);
}

void MainWindow::createWindowForTab(QWebEnginePage *page) {
    QWebEngineView *newView = new QWebEngineView(this);
    newView->setPage(page);

    connect(newView, &QWebEngineView::urlChanged, this, &MainWindow::updateAddressBar);
    connect(page, &QWebEnginePage::windowCloseRequested, [this, newView]() {
        int index = tabWidget->indexOf(newView);
        if (index >= 0) {
            closeTab(index);
        }
    });

    int index = tabWidget->addTab(newView, "New Tab");
    tabWidget->setCurrentIndex(index);
}