/***************************************************************************
 *   This file is part of the KOffice-Highlighting-Plugin                  *
 *   Copyright 2009 Jonathan Schmidt-Dominé <devel@the-user.org>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 3 of        *
 *   the License or (at your option) any later version accepted.           *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include <QStringList>
#include <QTextCursor>

#include <KTextEditingPlugin.h>

class QTextDocument;

class QModelIndex;

namespace KTextEditor
{
    class Document;
    class View;
}

class KDialog;
class KPushButton;
class KListWidget;

class Highlight : public KTextEditingPlugin
{
    Q_OBJECT
public:
    Highlight();
    ~Highlight();
	
    void finishedWord(QTextDocument *document, int cursorPosition);
    void finishedParagraph(QTextDocument *document, int cursorPosition);
    void checkSection(QTextDocument *document, int startPosition, int endPosition);

private slots:
    void changeCategory(const QModelIndex& index);
    void changeMode(const QModelIndex& mode);
    void process();
	
private:
    int m_begin, m_end;
    QTextDocument *m_document;
    KTextEditor::Document *m_kateDocument;

    KDialog *m_dialog;
    KListWidget *m_categories;
    KListWidget *m_modes;
    KTextEditor::View *m_view;
    QList<QStringList> m_modeTree;
};

#endif
