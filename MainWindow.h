#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>

// Forward declarations for UI components
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

private:
    QWebEngineView* webView;
    QLineEdit* addressBar;
    QPushButton *backButton, *forwardButton, *reloadButton;
};

#endif // MAINWINDOW_H