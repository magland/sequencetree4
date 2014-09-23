#include "streconstructionhighlighter.h"
#include <QSet>

STReconstructionHighlighter::STReconstructionHighlighter(QWidget *parent) : QSyntaxHighlighter(parent) 
{
	m_function_format.setForeground(Qt::darkMagenta);
	m_function_format.setFontWeight(QFont::Bold);
	m_action_format.setForeground(Qt::darkBlue);
	m_action_format.setFontWeight(QFont::Bold);
	m_script_format.setForeground(Qt::darkCyan);
	m_script_format.setFontWeight(QFont::Bold);
	m_comment_format.setForeground(Qt::darkGreen);
	m_quote_format.setForeground(Qt::darkRed);
	
	m_quote_exp.setMinimal(true);
	//be sure not to confuse with transpose operator!
	QString pattern="[^)\'\\]\\}0-9A-Za-z]\'.*[^\\\\]\'";
	pattern = pattern + "|" + "[^)\'\\]\\}0-9A-Za-z]\'.*[\\\\][\\\\]\'"; //tricky escaped backslash at end of string
	pattern = pattern + "|" + "[^)\'\\]\\}0-9A-Za-z]\'\'"; //tricky empty string
	///////beginning of line...
	pattern = pattern + "|" + "^\'.*[^\\\\]\'";
	pattern = pattern + "|" + "^\'.*[\\\\][\\\\]\'"; //tricky escaped backslash at end of string
	pattern = pattern + "|" + "^\'\'"; //tricky empty string
	m_quote_exp.setPattern(pattern); 
}

STReconstructionHighlighter::~STReconstructionHighlighter()
{
}

void STReconstructionHighlighter::highlightBlock ( const QString & text_in ) {
	QString text=text_in;
	static QSet<QString> special_keywords;
	if (special_keywords.isEmpty()) {
		special_keywords << "function" << "varargin" << "nargin" << "else" << "elseif" << "end";
	}
	
	//first find the quotes, and ignore comments in those
	int quote_index=text.indexOf(m_quote_exp);
	while ((quote_index>=0)&&(m_quote_exp.matchedLength()>0)) {
		QString section=text.mid(quote_index,m_quote_exp.matchedLength());
		if (section.contains("%")) {
			section.replace("%","P");
			text=text.mid(0,quote_index)+section+text.mid(quote_index+m_quote_exp.matchedLength());
		}
		quote_index=text.indexOf(m_quote_exp,quote_index+m_quote_exp.matchedLength());
	}
	
	QRegExp commentExp=QRegExp("%[^\n]*");
	int comment_index=text.indexOf(commentExp);
	if (comment_index>=0) {
		setFormat(comment_index,text.count()-comment_index,m_comment_format);
	}
	
	QRegExp exp("\\b[A-Za-z0-9_]+\\b");
	int index = text.indexOf(exp);
	while ((index >= 0)&&((index<comment_index)||(comment_index<0))&&(exp.matchedLength()>0)) {
		int length = exp.matchedLength();
		QString word=text.mid(index,length);
		if (special_keywords.contains(word))
			setFormat(index,length,m_action_format);
		index = text.indexOf(exp, index + length);
	}
	
	quote_index=text.indexOf(m_quote_exp);
	while ((quote_index>=0)&&(m_quote_exp.matchedLength()>0)) {
		setFormat(quote_index,m_quote_exp.matchedLength(),m_quote_format);
		quote_index=text.indexOf(m_quote_exp,quote_index+m_quote_exp.matchedLength());
	}
}

