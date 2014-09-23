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
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QRegExp>
#include <QtGui/QTextCharFormat>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include "cpphighlighter.h"

// state
enum 
{
    IN_MACRO = 0x01,
    IN_MULTILINE_COMMENT = 0x02,
    IN_SINGLELINE_COMMENT = 0x04,
    IN_STRING = 0x08,
    IN_SINGLE_STRING = 0x10,
    IN_FUNCTION_NAME = 0x12
};

//
//
CppHighlighter::CppHighlighter(QTextDocument* document):
    QSyntaxHighlighter(document), 
    m_reInclude("^\\s*#\\s*include\\s*(<.+>|\\\".+\\\")"),
    m_reMacro("^\\s*#.*$"),
    m_reMultilineMacro("^\\s*#\\s*(define|if|elif|pragma|warning|error)"),
    m_reSpecial("//|\\\"|'|/\\*")
{
    setupRegexTable();
    //do_highlight_tags=false;
    do_highlight_tags=true;
    do_highlight=true;
	tags_to_highlight=0;

	//m_formatSingleLineComment.setForeground(Qt::darkGreen); 
	//m_formatSingleLineComment.setFontFamily(family_name); 
	//m_formatSingleLineComment.setFontPointSize(point_size); 
	
	//m_formatKeyword.setForeground(Qt::blue);
	//m_formatKeyword.setFontFamily(family_name); 
	//m_formatKeyword.setFontPointSize(point_size); 
	
	//m_formatUserKeyword.setForeground(Qt::darkMagenta);
	//m_formatUserKeyword.setFontWeight(QFont::Bold);
	//m_formatUserKeyword.setFontFamily(family_name); 
	//m_formatUserKeyword.setFontPointSize(point_size); 
	
	//m_formatOperator.setForeground(Qt::black);
	//m_formatOperator.setFontFamily(family_name); 
	//m_formatOperator.setFontPointSize(point_size); 
	
	//m_formatNumber.setForeground(Qt::black);
	//m_formatNumber.setFontFamily(family_name); 
	//m_formatNumber.setFontPointSize(point_size); 
	
	//m_formatEscapeChar.setForeground(Qt::darkBlue);
	//m_formatEscapeChar.setFontFamily(family_name); 
	//m_formatEscapeChar.setFontPointSize(point_size); 
	
	//m_formatMacro.setForeground(Qt::blue);
	//m_formatMacro.setFontFamily(family_name); 
	//m_formatMacro.setFontPointSize(point_size); 
	
	//m_formatMultiLineComment.setForeground(Qt::darkGreen);
	//m_formatMultiLineComment.setFontFamily(family_name); 
	//m_formatMultiLineComment.setFontPointSize(point_size); 
	
	//m_formatString.setForeground(Qt::red);
	//m_formatString.setFontFamily(family_name); 
	//m_formatString.setFontPointSize(point_size); 

}

void CppHighlighter::set_text_formats(QMap<QString,QTextCharFormat> data) {
	m_formatSingleLineComment= data["comment"]; 
	m_formatKeyword          = data["cpp_keyword"]; 
	m_formatUserKeyword      = data["qt_keyword"];
	m_formatOperator         = data["operator"];
	m_formatNumber           = data["number"];
	m_formatEscapeChar       = data["escape_char"];
	m_formatMacro            = data["macro"];
	m_formatMultiLineComment = data["comment"];
	m_formatString           = data["string"];
	m_formatFunctionName     = data["function_name"];
}

//
CppHighlighter::~CppHighlighter()
{
}

//
QTextCharFormat const& CppHighlighter::formatFor(SyntaxType type)
{
    switch (type)
    {
        case TEXT:
        default:
            break;
        case SINGLE_LINE_COMMENT:
            return m_formatSingleLineComment;
        case KEYWORD:
            return m_formatKeyword;
        case USER_KEYWORD:
            return m_formatUserKeyword;
        case OPERATOR:
            return m_formatOperator;
        case NUMBER:
            return m_formatNumber;
        case ESCAPE_CHAR:
            return m_formatEscapeChar;
        case MACRO:
            return m_formatMacro;
        case MULTI_LINE_COMMENT:
            return m_formatMultiLineComment;
        case STRING:
            return m_formatString;
        case FUNCTION_NAME:
            return m_formatFunctionName;
    }
    static QTextCharFormat format;
    return format;
}

//
void CppHighlighter::doRegexMatch(QString const& str, int startPos)
{
    foreach (RegexItem item, m_regexItems)
    {
        int index = 0;
        int start = startPos;

        for (;;)
        {
            index = str.indexOf(item.regex, start);
            if (index == -1)
                break;
            int length = item.regex.matchedLength();
            start = index + length;
            QString cap = item.regex.cap();
            if (item.type == TEXT)
            {
                if (/*(cap.at(0) == 'Q') || */m_userKeywords.contains(cap))
                    setFormat(index, length, formatFor(USER_KEYWORD));
                else if (m_keywords.contains(cap))
                    setFormat(index, length, formatFor(KEYWORD));
                if ((tags_to_highlight)&&(do_highlight_tags)) {
                	if (tags_to_highlight->contains(cap)) {
                		QTextCharFormat formt=format(index);
                		formt.setFontWeight(QFont::Bold);
                		setFormat(index,length, formt);
                	}
                }
                continue;
            }
            setFormat(index, length, formatFor(item.type));
        }
    }
}

//
void CppHighlighter::addUserKeyword(QString const& keyword)
{
    m_userKeywords.insert(keyword);
}

void CppHighlighter::addUserKeywords(const QSet<QString> &keywords) {
	m_userKeywords += keywords;
}

//
bool CppHighlighter::removeUserKeyword(QString const& keyword)
{
    if (m_userKeywords.contains(keyword))
    {
        m_userKeywords.remove(keyword);
        return true;
    }
    return false;
}

//
void CppHighlighter::setupRegexTable()
{
    RegexItem item;

    m_regexItems.clear();

    // number
    item.regex = QRegExp("\\b([0-9]+|0[xX][0-9a-fA-F]+|0[0-7]+)(\\.[0-9]+)?([eE][0-9]+)?\\b");
    item.type = NUMBER;
    m_regexItems.push_back(item);

    // keywords
    QStringList keywords;
    keywords << "asm" << "auto" << "bool" << "break" << "case"
             << "catch" << "char" << "class" << "const" << "const_cast"
             << "continue" << "default" << "delete" << "do" << "double"
             << "dynamic_cast" << "else" << "enum" << "explicit" << "export"
             << "extern" << "false" << "float" << "for" << "friend"
             << "goto" << "if" << "inline" << "int" << "long"
             << "mutable" << "namespace" << "new" << "operator" << "private"
             << "protected" << "public" << "register" << "reinterpret_cast" << "return"
             << "short" << "signed" << "sizeof" << "static" << "static_cast"
             << "struct" << "switch" << "template" << "this" << "throw"
             << "true" << "try" << "typedef" << "typeid" << "typename"
             << "union" << "unsigned" << "using" << "virtual" << "void"
             << "volatile" << "wchar_t" << "while"
             << "signals" << "slots" << "SIGNAL" << "SLOT";
					 

    // function names and all operators
    item.regex = QRegExp("[A-Za-z_]+[A-Za-z0-9_]*\\(|[();\\<\\>\\|\\&\\+\\-\\=\\%\\?\\*\\/\\!\\^\\[\\]:\\.]+");
    item.type = FUNCTION_NAME;
    m_regexItems.push_back(item);

    // text
    item.regex = QRegExp("\\b[a-zA-Z_][a-zA-Z0-9_]+\\b");
    item.type = TEXT;
    m_regexItems.push_back(item);

    m_keywords = QSet<QString>::fromList(keywords);
    
    // user keywords
    QStringList userKeywords;
	m_userKeywords = QSet<QString>::fromList(userKeywords);    
    
}

bool CppHighlighter::handleState(QString const& text, int& startPos, int& endPos)
{
    int prevState = previousBlockState();
    startPos = 0;
    endPos = text.length();

    if (prevState == -1)
        prevState = 0;

    if ((prevState & IN_STRING) || (prevState & IN_SINGLE_STRING))
    {
        // this line is in a string, find for end of the string
        endPos = searchStringEnd(text, startPos, 
                (prevState & IN_SINGLE_STRING)? '\'':'"');
        if (endPos == -1)
        {
            // this is still in string
            setFormat(0, text.length(), formatFor(STRING));
            setCurrentBlockState(IN_STRING);
            return true;
        }
        else
        {
            endPos += 1; // strlen("\"");
            setFormat(0, endPos - startPos, formatFor(STRING));
            startPos = endPos;
        }
    }
    else if (prevState & IN_MULTILINE_COMMENT)
    {
        // this line is in a multiline comment, find for the end
        endPos = searchMultilineCommentEnd(text, startPos);
        if (endPos == -1)
        {
            // this is still in comment
            setFormat(0, text.length(), formatFor(MULTI_LINE_COMMENT));
            // maybe previous line is in a macro...
            setCurrentBlockState(previousBlockState());
            return true;
        }
        else
        {
            endPos += 2; // strlen("*/");
            setFormat(0, endPos - startPos, formatFor(MULTI_LINE_COMMENT));
            startPos = endPos;
        }
    }
    else if (prevState & IN_SINGLELINE_COMMENT)
    {
        setFormat(0, text.length(), formatFor(SINGLE_LINE_COMMENT));
        if (text.endsWith("\\"))
        {
            setCurrentBlockState(IN_SINGLELINE_COMMENT);
        }
        return true;
    }
    else if (prevState & IN_MACRO)
    {
        setFormat(0, text.length(), formatFor(MACRO));
        if (text.endsWith("\\"))
        {
            setCurrentBlockState(IN_MACRO);
        }
        // we can't just return...
        // consider the following code:
        // #define a /*
        //             multiline comment
        //           */ something
    }
    return false;
}

//
void CppHighlighter::highlightBlock(QString const& text)
{
	if (!do_highlight)
		return;
		
    int startPos;
    int endPos;
    setCurrentBlockState(0);

    // 1. handle state specific highlighting
    if (handleState(text, startPos, endPos)) {
        return;
	}

    endPos = startPos; // backup startPos
    // 2. Highlight macro
    if (handlePreprocessor(text))
    {
        // 3. Highlight keywords
        /*CppHighlighterBlockData *userdata=(CppHighlighterBlockData *)currentBlockUserData();
        if (!userdata) {
        	userdata=new CppHighlighterBlockData;
        	setCurrentBlockUserData(userdata);
        }
        if (userdata) {
        	if (userdata->modified_since_last_highlight)
	        	doRegexMatch(text, startPos);
	        userdata->modified_since_last_highlight=false;
        }*/
        doRegexMatch(text, startPos);
    }

    // 4. Highlight strings, comments...
    // a. search for first occurrences of ", //, /*
    startPos = endPos;
    while (true)
    {
        startPos = text.indexOf(m_reSpecial, startPos);
        if (startPos == -1)
            break;

        QString cap = m_reSpecial.cap();
        if (cap == "//")
        {
            setFormat(startPos, text.length() - startPos, formatFor(SINGLE_LINE_COMMENT));
            if (text.endsWith("\\"))
                setCurrentBlockState(IN_SINGLELINE_COMMENT);
            return;
        }
        else if ((cap == "\"") || (cap == "'"))
        {
            endPos = searchStringEnd(text, startPos + 1, cap.at(0));
            // handle unicode string
            if ((startPos > 0) && text.at(startPos - 1) == QChar('L'))
                --startPos;
            if (endPos == -1)
            {
                // this is a multiline string
                setFormat(startPos, text.length() - startPos, formatFor(STRING));
                handleEscapeChar(text, startPos, text.length() - startPos);
                setCurrentBlockState(cap.at(0) == QChar('"')? IN_STRING: IN_SINGLE_STRING);
                return;
            }
            else
            {
                endPos += 1;
                setFormat(startPos, endPos - startPos, formatFor(STRING));
                handleEscapeChar(text, startPos, endPos - startPos);
                startPos = endPos;
            }
        }
        else if (cap == "/*")
        {
            endPos = searchMultilineCommentEnd(text, startPos + 2);
            if (endPos == -1)
            {
                // this comment covers multi line
                setFormat(startPos, text.length() - startPos, formatFor(MULTI_LINE_COMMENT));
                setCurrentBlockState(IN_MULTILINE_COMMENT);
                return;
            }
            else
            {
                endPos += 2;
                setFormat(startPos, endPos - startPos, formatFor(MULTI_LINE_COMMENT));
                startPos = endPos;
            }
        }
    }
}

//
// Return true if we still need to highlight keywords
bool CppHighlighter::handlePreprocessor(QString const& text)
{
    if (text.indexOf(m_reMultilineMacro) != -1)
    {
        setFormat(0, text.length(), formatFor(MACRO));
        // FIXME: This can't handle the following code:
        //  #define some this is  /*
        //    blabla... */ a macro definition...
        //
        if (text.endsWith("\\"))
            setCurrentBlockState(IN_MACRO);
    }
    else if (text.indexOf(m_reInclude) != -1)
    {
        // TODO: we can highlight it in a different format
        setFormat(0, text.length(), formatFor(MACRO));
        int pos = m_reInclude.pos(1);
        if (pos > 0)
        {
            QString cap = m_reInclude.cap(1);
            setFormat(pos, cap.length(), formatFor(STRING));
        }
    }
    else if (text.indexOf(m_reMacro) != -1)
    {
        setFormat(0, text.length(), formatFor(MACRO));
        return false;
    }
    return true;
}

//
int CppHighlighter::searchStringEnd(QString const& text, int startPos, QChar strChar)
{
    for (int pos = startPos; pos < text.length(); pos++)
    {
        if (text.at(pos) == QChar('\\'))
            ++pos;
        else if (text.at(pos) == strChar)
            return pos;
    }
    return -1;
}

//
int CppHighlighter::searchMultilineCommentEnd(QString const& text, int startPos)
{
    return text.indexOf("*/", startPos);
}

//
void CppHighlighter::handleEscapeChar(QString const& text, int start, int len)
{
    for (int pos = start; pos < start + len; pos++)
    {
        if (text.at(pos) == QChar('\\'))
        {
            int endPos = pos;
            for (int i = 1; i <= 3; i++)
            {
                if ((pos + i < text.length()) && 
                        (text.at(pos + i) >= QChar('0')) &&
                        (text.at(pos + i) <= QChar('7')) )
                    endPos = pos + i;
                else
                    break;
            }
            if (endPos == pos)
            {
                endPos = pos + 1;
                if ((endPos < start + len) && (text.at(endPos).toLower() == QChar('x')))
                {
                    while (++endPos < start + len)
                    {
                        QChar c = text.at(endPos).toLower();
                        if (c.isNumber() || ((c >= QChar('a')) && (c <= QChar('f'))))
                            continue;
                        else
                            break;
                    }
                    --endPos;
                }
            }
            setFormat(pos, endPos - pos + 1, formatFor(ESCAPE_CHAR));
            pos = endPos;
        }
    }
}

