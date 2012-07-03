/**
 * @file /src/takeoff/takeoff_widget/Launcher.cpp
 *
 * This file is part of Takeoff.
 *
 * Takeoff is free software:  you can redistribute it and/or modify it under the
 * terms of the GNU General Public License  as  published by  the  Free Software
 * Foundation,  either version 3 of the License,  or (at your option)  any later
 * version.
 *
 * Takeoff is distributed in  the hope that it will be useful,  but  WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the  GNU General Public License  for more details.
 *
 * You should have received a copy of the  GNU General Public License along with
 * Takeoff. If not, see <http://www.gnu.org/licenses/>.
 *
 * @author José Expósito <jose.exposito89@gmail.com> (C) 2011
 * @class  Takeoff::Launcher
 */
#include "Launcher.h"
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QMenu>
#include <KDE/KRun>
#include <KDE/Plasma/IconWidget>
#include <KDE/Plasma/ToolTipContent>
#include <KDE/Plasma/ToolTipManager>
#include <KDE/KIcon>
#include "../model/config/Config.h"
#include "../model/favorites/Favorites.h"
using namespace Takeoff;

// ************************************************************************** //
// **********              CONSTRUCTORS AND DESTRUCTOR             ********** //
// ************************************************************************** //

Launcher::Launcher(const QIcon &icon, const QString &name,
        const QString &desktopFile)
        : icon(icon),
          name(name),
          desktopFile(desktopFile)
{
    this->init();
}

Launcher::Launcher(const Launcher &launcher)
        : QGraphicsWidget(),
          icon(launcher.icon),
          name(launcher.name),
          desktopFile(launcher.desktopFile)
{
    this->init();
}


// ************************************************************************** //
// **********                   PRIVATE METHODS                    ********** //
// ************************************************************************** //

void Launcher::init()
{
    // Set the icon
    iconWidget = new Plasma::IconWidget(this->icon, "", this);

    Config *cfg = Config::getInstance();
    if (cfg->getSettings(Config::SHOW_ICON_TEXT).toBool())
        iconWidget->setText(this->name);

    connect(iconWidget, SIGNAL(clicked()), this, SLOT(runApplication()));
    connect(iconWidget, SIGNAL(clicked()), this, SIGNAL(clicked()));

    iconWidget->setDrawBackground(true);
    // Set the tooltip
    Plasma::ToolTipContent data;
    data.setMainText(this->name);
    data.setImage(this->icon.pixmap(32, 32));
    Plasma::ToolTipManager::self()->setContent(iconWidget, data);

    // Add the icon to the layout
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(this);
    l->addItem(iconWidget);
    this->setLayout(l);
}


// ************************************************************************** //
// **********                    PUBLIC SLOTS                      ********** //
// ************************************************************************** //

void Launcher::runApplication() const
{
    new KRun(KUrl(this->desktopFile), NULL);
}


// ************************************************************************** //
// **********                    PRIVATE SLOTS                     ********** //
// ************************************************************************** //

void Launcher::addToFavorites() const
{
    Favorites *favorites = Favorites::getInstance();
    favorites->addToFavorites(this);
    emit this->addedToFavorites();
}

void Launcher::removeFromFavorites() const
{
    Favorites *favorites = Favorites::getInstance();
    favorites->removeFromFavorites(this);
    emit this->removedFromFavorites();
}


// ************************************************************************** //
// **********                    PUBLIC METHODS                    ********** //
// ************************************************************************** //

void Launcher::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QMenu menu;

        Favorites *favorites = Favorites::getInstance();
        if (favorites->isfavorite(this)) {
            menu.addAction(KIcon("list-remove"), i18n("Remove from favorites"),
                    this, SLOT(removeFromFavorites()));
        } else {
            menu.addAction(KIcon("favorites"), i18n("Add to favorites"),
                    this, SLOT(addToFavorites()));
        }


        menu.exec(QCursor::pos());
    }
}

// ************************************************************************** //
// **********                      GET/SET/IS                      ********** //
// ************************************************************************** //

QString Launcher::getName() const
{
    return this->name;
}

QString Launcher::getDesktopFile() const
{
    return this->desktopFile;
}

void Launcher::setBackground()
{
}
