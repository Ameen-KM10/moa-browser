#pragma once

#include <QMainWindow>
#include <QWebEngineView>
#include <QTabWidget>

class QLineEdit;
class QPushButton;

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

private:
    QTabWidget *tabWidget;
    QLineEdit *addressBar;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *reloadButton;
    QPushButton *newTabButton;
    
    QWebEngineView* currentWebView() const;
};