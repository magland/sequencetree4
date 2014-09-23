#ifndef streconstructionhighlighter_H
#define streconstructionhighlighter_H
#include <QSyntaxHighlighter>
#include <QWidget>
#include <QTextCharFormat>
#include <QRegExp>

class STReconstructionHighlighter : public QSyntaxHighlighter {
public:
	STReconstructionHighlighter(QWidget *parent=0);
	virtual ~STReconstructionHighlighter();
	
	void highlightBlock ( const QString & text );
private:
	QTextCharFormat m_action_format;
	QTextCharFormat m_function_format;
	QTextCharFormat m_script_format;
	QTextCharFormat m_comment_format;
	QTextCharFormat m_quote_format;
	QRegExp m_quote_exp;
};

#endif
