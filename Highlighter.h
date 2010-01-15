#ifndef HIGHLIGHTER_H_
#define HIGHLIGHTER_H_


#include <QSyntaxHighlighter>

#include <QRegExp>
#include <QVector>
#include <QTextCharFormat>

class QTextDocument;

class Highlighter 
    : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextEdit *parent);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat chordFormat;
};


#endif /* HIGHLIGHTER_H_ */

