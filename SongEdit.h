#ifndef SONGEDIT_H_
#define SONGEDIT_H_

#include <QTextEdit>

class Highlighter;

class SongEdit
    : public QTextEdit
{
    Q_OBJECT
public:
    SongEdit(QWidget *parent = 0);
    virtual ~SongEdit();
    void setSongText(const QString& text);
private:
    Highlighter *m_hl;
};

#endif /* SONGEDIT_H_ */

