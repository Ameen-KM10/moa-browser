#pragma once

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QTabWidget>

class QLineEdit;
class QPushButton;
class MainWindow;

// Custom WebPage class to handle popups
class WebPage : public QWebEnginePage {
    Q_OBJECT
public:
    explicit WebPage(MainWindow *mainWindow);

protected:
    QWebEnginePage *createWindow(WebWindowType type) override;

private:
    MainWindow *m_mainWindow;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadPage();
    void updateAddressBar(const QUrl &url);
    void onBackButtonClicked();
    void onForwardButtonClicked();
    void onReloadButtonClicked();
    void addNewTab();
    void closeTab(int index);
    void closeCurrentTab() { closeTab(tabWidget->currentIndex()); }
    void switchTab(int index);

public:
    void createWindowForTab(QWebEnginePage *page);  // Add this line

private:
    QTabWidget *tabWidget;
    QLineEdit *addressBar;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *reloadButton;
    QPushButton *newTabButton;
    QPushButton *devToolsButton;

    QWebEngineView* currentWebView() const;
    void setupWebPage(QWebEngineView *webView);     // Add this line
};