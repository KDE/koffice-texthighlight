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

#include "HighlightPlugin.h"
#include "HighlightFactory.h"

#include <KPluginFactory>
#include <KPluginLoader>

#include <KTextEditingRegistry.h>

K_PLUGIN_FACTORY( HighlightPluginFactory, registerPlugin<HighlightPlugin>(); )
K_EXPORT_PLUGIN( HighlightPluginFactory("HighlightPlugin") )

HighlightPlugin::HighlightPlugin( QObject *parent, const QVariantList& )
    : QObject(parent)
{
    KTextEditingRegistry::instance()->add( new HighlightFactory(parent));
}

#include "HighlightPlugin.moc"
