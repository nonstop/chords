
#include <QtGui>
#include "Highlighter.h"



Highlighter::Highlighter(QTextEdit *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    chordFormat.setForeground(Qt::darkBlue);
    chordFormat.setFontWeight(QFont::Bold);

    QStringList keywordPatterns;

    const QString fileName("chords.dat");
    QFile fl(fileName);
    if (!fl.open(QIODevice::ReadOnly))
    {
        QString msg = tr("Can't open file: ") + fileName;
        QMessageBox::critical(NULL, tr("Error"), msg);
        return;
    }
    QTextStream txt(&fl);
    QString chordsStr = txt.readAll();
    keywordPatterns = chordsStr.split(QRegExp("\\s+"));


    foreach (QString pattern, keywordPatterns)
    {
        if (pattern == "")
            continue;
        pattern = "\\b" + pattern + "\\b";
        rule.pattern = QRegExp(pattern);
        rule.format = chordFormat;
        highlightingRules.append(rule);
    }
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (HighlightingRule rule, highlightingRules) 
    {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) 
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }
    setCurrentBlockState(0);
}


