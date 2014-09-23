/*
* This file is part of QDevelop, an open-source cross-platform IDE
* Copyright (C) 2006  Jean-Luc Biord
*
* The file has been modified for use in HaiQ
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*
*/
#ifndef CPP_HIGHLIGHTER_H
#define CPP_HIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QTextCharFormat>
#include <QtCore/QVector>
#include <QtCore/QSet>
#include <QTextBlockUserData>
#include <QMenu>

class QTextDocument;

/*class CppHighlighterBlockData : public QTextBlockUserData {
public:
	bool modified_since_last_highlight;
	CppHighlighterBlockData() {
		modified_since_last_highlight=true;
	}
};*/

class CppHighlighter: public QSyntaxHighlighter
{
public:
    CppHighlighter(QTextDocument* document = NULL);
    ~CppHighlighter();
    void addUserKeyword(QString const& regex);
    void addUserKeywords(const QSet<QString> &keywords); //jfm
    bool removeUserKeyword(QString const& regex);
    void setupRegexTable();
    QSet<QString> *tags_to_highlight;
    bool do_highlight_tags;
    bool do_highlight;
    void set_text_formats(QMap<QString,QTextCharFormat> data);
private:

protected:
    void highlightBlock(QString const& text);
private:
    enum SyntaxType
    {
        TEXT, // normal text

        // following items can be fetched by a simple pattern matching
        SINGLE_LINE_COMMENT,
        KEYWORD, 
        USER_KEYWORD,
        OPERATOR,
        NUMBER,
        ESCAPE_CHAR,

        // following items need states on lines
        MACRO, MULTI_LINE_COMMENT, STRING, 
        FUNCTION_NAME
    };
    
    

    QTextCharFormat const& formatFor(SyntaxType type);
    void doRegexMatch(QString const& str, int startPos = 0);
    bool handleState(QString const& text, int& startPos, int& endPos);
    int searchStringEnd(QString const& text, int startPos = 0, QChar strChar = '"');
    int searchMultilineCommentEnd(QString const& text, int startPos = 0);
    bool handlePreprocessor(QString const& text);
    void handleEscapeChar(QString const& text, int start = 0, int len = 0);
private:
    QRegExp m_reInclude;
    QRegExp m_reMacro;
    QRegExp m_reMultilineMacro;
    QRegExp m_reSpecial;
    QRegExp m_reWord;

    struct RegexItem
    {
        RegexItem() {}
        RegexItem(RegexItem const& other):regex(other.regex), type(other.type) {}
        QRegExp regex;
        SyntaxType type;
    };
    QSet<QString> m_keywords;
    QSet<QString> m_userKeywords;
    QVector<RegexItem> m_regexItems;

    // formats
    QTextCharFormat m_formatSingleLineComment;
    QTextCharFormat m_formatMultiLineComment;
    QTextCharFormat m_formatKeyword;
    QTextCharFormat m_formatUserKeyword;
    QTextCharFormat m_formatOperator;
    QTextCharFormat m_formatNumber;
    QTextCharFormat m_formatEscapeChar;
    QTextCharFormat m_formatMacro;
    QTextCharFormat m_formatString;
    QTextCharFormat m_formatFunctionName;
public:
    QTextCharFormat keywordFormat() { return m_formatKeyword; }
    void setKeywordFormat(QTextCharFormat const& f) { m_formatKeyword = f; }

    // class format? I guess you mean user keyword...
    QTextCharFormat classFormat() { return m_formatUserKeyword; }
    void setClassFormat(QTextCharFormat const& f) { m_formatUserKeyword = f; }

    QTextCharFormat singleLineCommentFormat() { return m_formatSingleLineComment; };
    void setSingleLineCommentFormat(QTextCharFormat const& f) { m_formatSingleLineComment = f; }

    QTextCharFormat multiLineCommentFormat() { return m_formatMultiLineComment; };
    void setMultiLineCommentFormat(QTextCharFormat const& f) { m_formatMultiLineComment = f; }

    QTextCharFormat quotationFormat() { return m_formatString; }
    void setQuotationFormat(QTextCharFormat const& f) { m_formatString = f; }

    // Function format? Ignore it...
    QTextCharFormat functionFormat() { return QTextCharFormat(); }
    void setFunctionFormat(QTextCharFormat const& f) { Q_UNUSED(f) }

    QTextCharFormat preprocessorFormat() { return m_formatMacro; }
    void setPreprocessorFormat(QTextCharFormat const& f) { m_formatMacro = f; }

    QTextCharFormat userKeywordFormat() { return m_formatUserKeyword; }
    void setUserKeywordFormat(QTextCharFormat const& f) { m_formatUserKeyword = f; }

    QTextCharFormat operatorFormat() { return m_formatOperator; }
    void setOperatorFormat(QTextCharFormat const& f) { m_formatOperator = f; }

    QTextCharFormat escapeCharFormat() { return m_formatEscapeChar; }
    void setEscapeCharFormat(QTextCharFormat const& f) { m_formatEscapeChar = f; }
};

#endif
