#ifndef CHWINDOW_H_
#define CHWINDOW_H_

#include <QMainWindow>

class QWidget;
class QTabWidget;
class QListWidget;
class QStatusBar;
class QLineEdit;
class QPushButton;

class SongsTree;
class RightPaneWidget;

class ChWindow
    : public QMainWindow
{
    Q_OBJECT
public:
    ChWindow();
    virtual ~ChWindow();

private slots:
    void onQuit();
private:
    QTabWidget *createLeftPane();
    QWidget *createSearchPane();

    SongsTree *m_trSongs;
    QListWidget *m_lSearch;
    QPushButton *m_bSearch;
    QLineEdit *m_leSearch;
    QTabWidget *m_tabLeft;
    RightPaneWidget *m_wRight;
    QStatusBar *statusBar;    
};

#endif /* CHWINDOW_H_ */

