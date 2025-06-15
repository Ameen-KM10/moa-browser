#include "MainWindow.h"
#include <QToolBar>
#include <QHBoxLayout>
#include <QUrl>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QLineEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    // Optimize global settings (Qt 6 style)
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, false);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, false);

    // Create web view
    webView = new QWebEngineView(this);
    webView->setUrl(QUrl("https://example.com")); 

    // Address bar
    addressBar = new QLineEdit(this);
    addressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Navigation buttons
    backButton = new QPushButton("←", this);
    forwardButton = new QPushButton("→", this);
    reloadButton = new QPushButton("⟳", this);

    // Connect signals
    connect(webView, &QWebEngineView::urlChanged, this, &MainWindow::updateAddressBar);
    connect(addressBar, &QLineEdit::returnPressed, this, &MainWindow::loadPage);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);
    connect(forwardButton, &QPushButton::clicked, this, &MainWindow::onForwardButtonClicked);
    connect(reloadButton, &QPushButton::clicked, this, &MainWindow::onReloadButtonClicked);

    // Toolbar layout
    QWidget* toolWidget = new QWidget(this);
    QHBoxLayout* toolLayout = new QHBoxLayout(toolWidget);
    toolLayout->setSpacing(0);
    toolLayout->setContentsMargins(4, 4, 4, 4);
    toolLayout->addWidget(backButton);
    toolLayout->addWidget(forwardButton);
    toolLayout->addWidget(reloadButton);
    toolLayout->addWidget(addressBar);

    QToolBar* toolBar = new QToolBar("Navigation");
    toolBar->addWidget(toolWidget);
    addToolBar(toolBar);

    // Set central widget
    setCentralWidget(webView);
    resize(1024, 768);
    setWindowTitle("Lightweight Browser");
}

MainWindow::~MainWindow() {}

void MainWindow::loadPage() {
    QString url = addressBar->text();
    if (!url.startsWith("http")) {
        url = "https://"  + url;
    }
    webView->setUrl(QUrl(url));
}

void MainWindow::updateAddressBar(const QUrl &url) {
    addressBar->setText(url.toString());
}

void MainWindow::onBackButtonClicked() {
    webView->back();
}

void MainWindow::onForwardButtonClicked() {
    webView->forward();
}

void MainWindow::onReloadButtonClicked() {
    webView->reload();
}