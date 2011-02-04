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

#include "HighlightFactory.h"
#include "Highlight.h"

#include <KLocale>

HighlightFactory::HighlightFactory(QObject *parent)
    : KoTextEditingFactory(parent, "kotexthighlight")
{
    setShowInMenu(true);
    setTitle(i18n ("Highlight...") );
}

KoTextEditingPlugin *HighlightFactory::create(KoResourceManager *) const
{
    return new Highlight();
}
