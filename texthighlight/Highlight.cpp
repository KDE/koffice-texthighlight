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

#include "Highlight.h"

#include <QTextBlock>
#include <QTextDocument>

#include <QBoxLayout>

#include <KLocale>
#include <KDebug>

#include <KDialog>
#include <KPushButton>
#include <KListWidget>

#include <KoTextEditor.h>
#include <KoTextDocument.h>
#include <styles/KoCharacterStyle.h>

#include <KTextEditor/Editor>
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <KTextEditor/HighlightInterface>

using namespace KTextEditor;

Highlight::Highlight()
    : m_kateDocument(KTextEditor::editor("katepart")->createDocument(this))
{
    m_dialog = new KDialog;
    QWidget *widget = new QWidget(m_dialog);
    m_dialog->setCaption(i18n("Kate Highlighting"));
    m_dialog->setButtons(KDialog::Ok | KDialog::Cancel);
    m_categories = new KListWidget(widget);
    m_modes = new KListWidget(widget);
    m_view = m_kateDocument->createView(widget);
    
    m_categories->resize(150, 300);
    m_modes->resize(150, 300);
    m_view->resize(300, 300);
    m_categories->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    m_modes->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    QSizePolicy viewPolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
//    viewPolicy.setHorizontalStretch(2);
    m_view->setSizePolicy(viewPolicy); 
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_categories);
    layout->addWidget(m_modes);
    layout->addWidget(m_view);
    widget->setLayout(layout);
    m_dialog->setMainWidget(widget);
    
    QStringList modes(m_kateDocument->highlightingModes());
    for(int i = 0; i != modes.size(); ++i)
    {
        QString section = m_kateDocument->highlightingModeSection(i);
        int pos = 0;
        for(; pos != m_categories->count(); ++pos)
            if(m_categories->item(pos)->text() == section)
                break;
        if(pos == m_categories->count())
        {
            m_categories->addItem(section);
            m_modeTree.append(QStringList());
        }
        m_modeTree[pos].append(modes[i]);
    }
    
    m_modes->addItems(m_modeTree[0]);
    m_categories->setCurrentIndex(m_categories->model()->index(0, 0));
    m_modes->setCurrentIndex(m_modes->model()->index(0, 0));
    
    connect(m_categories, SIGNAL(activated(const QModelIndex&)), this, SLOT(changeCategory(const QModelIndex&)));
    connect(m_modes, SIGNAL(activated(const QModelIndex&)), this, SLOT(changeMode(const QModelIndex&)));
    connect(m_modes, SIGNAL(doubleClicked(const QModelIndex&)), m_dialog, SLOT(accept()));
    connect(m_dialog, SIGNAL(accepted()), this, SLOT(process()));
}

Highlight::~Highlight()
{
    delete m_dialog;
}

void Highlight::finishedWord(QTextDocument *document, int cursorPosition)
{
    Q_UNUSED(document);
    Q_UNUSED(cursorPosition);
}

void Highlight::finishedParagraph(QTextDocument *document, int cursorPosition)
{
    Q_UNUSED(document);
    Q_UNUSED(cursorPosition);
}

void Highlight::checkSection(QTextDocument *document, int startPosition, int endPosition)
{
    m_begin = startPosition;
    m_end = endPosition;

    QTextCursor cursor(document);
    cursor.setPosition(startPosition);
    cursor.setPosition(endPosition, QTextCursor::KeepAnchor);
    
    QString text(cursor.selectedText());
    text.replace(QChar::ParagraphSeparator, '\n');
    m_kateDocument->setText(text);

    m_document = document;

    m_dialog->open();
}

void Highlight::process()
{
    emit startMacro(i18n("Highlight"));
    
    HighlightInterface *highlight = qobject_cast<HighlightInterface*>(m_kateDocument);
    Q_ASSERT(highlight != 0);
    int end = m_kateDocument->lines();
    KoTextEditor *cursor = KoTextDocument(m_document).textEditor();
    for(int i = 0; i != end; ++i)
    {
        QList<HighlightInterface::AttributeBlock> attributes = highlight->lineAttributes(i);
        foreach(const HighlightInterface::AttributeBlock& attr, attributes)
        {
            kDebug() << attr.attribute->foreground() << attr.start << attr.length;
            cursor->clearSelection(); 
            cursor->setPosition(m_begin);
            cursor->movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, i);
            cursor->setPosition(cursor->position() + attr.start, QTextCursor::MoveAnchor);
            kDebug() << cursor->position();
            cursor->setPosition(cursor->position() + attr.length, QTextCursor::KeepAnchor);
            kDebug() << cursor->anchor() << cursor->position();
            kDebug() << cursor->selectedText();
            cursor->setStyle(&KoCharacterStyle(*attr.attribute));
        }
    }
    //delete cursor;
    
    emit stopMacro();
    
    m_dialog->close();
}

void Highlight::changeCategory(const QModelIndex& index)
{
    // Clicking at the selected category won't reset m_modes
    if(m_modeTree[index.row()][0] != m_modes->item(0)->text())
    {
        m_modes->clear();
        m_modes->addItems(m_modeTree[index.row()]);
        m_modes->setCurrentIndex(m_modes->model()->index(0, 0));
    }
}

void Highlight::changeMode(const QModelIndex& index)
{
    Q_UNUSED(index);
    // Kate should check if m_kateDocument->highlightingMode() == m_modes->currentItem()->text()
    m_kateDocument->setHighlightingMode(m_modes->currentItem()->text());
}

#include "Highlight.moc"
