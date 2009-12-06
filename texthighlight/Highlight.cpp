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

#include <KTextEditor/Editor>
#include <KTextEditor/Document>
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
	
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(m_categories);
	layout->addWidget(m_modes);
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
    m_cursor = QTextCursor(document);
    m_cursor.setPosition(startPosition);
    m_cursor.setPosition(endPosition, QTextCursor::KeepAnchor);
    m_document = document;
	
	m_dialog->open();
}

void Highlight::process()
{
	emit startMacro(i18n("Highlight"));
	
	QString text(m_cursor.selectedText());
	text.replace(QChar::ParagraphSeparator, '\n');
	kDebug() << text;
	m_kateDocument->setText(text);
	m_kateDocument->setHighlightingMode(m_modes->currentItem()->text());
	
	HighlightInterface *highlight = qobject_cast<HighlightInterface*>(m_kateDocument);
	Q_ASSERT(highlight != 0);
	int end = m_kateDocument->lines();
	for(int i = 0; i != end; ++i)
	{
		QList<HighlightInterface::AttributeBlock> attributes = highlight->lineAttributes(i);
		foreach(const HighlightInterface::AttributeBlock& attr, attributes)
		{
			kDebug() << attr.attribute->foreground() << attr.start << attr.length;
			QTextCursor cursor(m_cursor);
			cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, i);
			cursor.setPosition(cursor.position() + attr.start);
			kDebug() << cursor.position();
			cursor.setPosition(cursor.position() + attr.length, QTextCursor::KeepAnchor);
			kDebug() << cursor.anchor();
			cursor.setCharFormat(*attr.attribute);
		}
	}
	
	emit stopMacro();
	
	m_dialog->close();
}

void Highlight::changeCategory(const QModelIndex& index)
{
	m_modes->clear();
	m_modes->addItems(m_modeTree[index.row()]);
	m_modes->setCurrentIndex(m_modes->model()->index(0, 0));
}

#include "Highlight.moc"
