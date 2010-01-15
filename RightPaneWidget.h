#ifndef RIGHTPANEWIDGET_H_
#define RIGHTPANEWIDGET_H_

#include <QWidget>

#include "ChordsTypes.h"

class QTabWidget;
class QPushButton;
class SongText;

class RightPaneWidget
    : public QWidget
{
    Q_OBJECT
public:
    RightPaneWidget();
    virtual ~RightPaneWidget();
signals:
    void setText(int textIndex, const QString& text);
    void deleteText(int textIndex);
private slots:
    void songSelected(const Song& song);
    void noSongSelected();
    void onSaveText();
    void onDeleteText();
private:
    SongText *m_twTexts;
    QPushButton *m_bSave;
    QPushButton *m_bDelete;
};





#endif /* RIGHTPANEWIDGET_H_ */

