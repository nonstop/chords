#ifndef SONGDLG_H_
#define SONGDLG_H_

#include <QString>
#include <QVector>
#include <QDialog>
#include <QTextDocument>
#include <QCompleter>

class QWidget;
class QLineEdit;
class QPushButton;
class SongEdit;

class SongDlg
    : public QDialog
{
    Q_OBJECT
public:
    SongDlg(QWidget *parent, const QString& artistName, const QStringList& artistNames, const QStringList& songNames);
    virtual ~SongDlg();
    
    QString getArtistName() const;
    QString getSongName() const;
    QString getSongText() const;
private slots:
    void onOk();
    void loadFromFile();
private:
    QCompleter m_artistNames;
    QCompleter m_songNames;
    
    QPushButton *m_bOk;
    QPushButton *m_bCancel;
    QPushButton *m_bFromFile;
    QLineEdit *m_leArtistName;
    QLineEdit *m_leSongName;
    SongEdit *m_teText;
    QTextDocument m_tdText;
};

#endif /* SONGDLG_H_ */

