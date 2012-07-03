/**
 * @file /src/takeoff/takeoff_widget/menu/MenuWidget.cpp
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
 * @class  TakeoffPrivate::MenuWidget
 */
#include "MenuWidget.h"
#include <QtGui/QGraphicsLinearLayout>
#include <QKeyEvent>
#include <KDE/Plasma/TabBar>
#include "PanelArea.h"
#include "../Launcher.h"
#include "../../model/config/Config.h"
#include "../../model/favorites/Favorites.h"
using namespace TakeoffPrivate;

// ************************************************************************** //
// **********              CONSTRUCTORS AND DESTRUCTOR             ********** //
// ************************************************************************** //

MenuWidget::MenuWidget(QGraphicsWidget *parent)
        : QGraphicsWidget(parent),
          menuBar(new Plasma::TabBar)
{
    // Only show categories if more than one is available
    this->menuBar->setTabBarShown(false);

    // Add the widget
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(this);
    l->addItem(this->menuBar);
    this->setLayout(l);
}

// ************************************************************************** //
// **********                    PUBLIC METHODS                    ********** //
// ************************************************************************** //

void MenuWidget::addMenuCategory(const QIcon &icon, const QString &title)
{
    PanelArea *panelArea = new PanelArea(this);

    connect(panelArea, SIGNAL(clicked()), this, SIGNAL(clicked()));
    connect(panelArea, SIGNAL(addedToFavorites()),
            this, SIGNAL(addedToFavorites()));
    connect(panelArea, SIGNAL(removedFromFavorites()),
            this, SIGNAL(removedFromFavorites()));
    connect(this, SIGNAL(signalArrowPressed(QKeyEvent*)),
            panelArea, SLOT(slotArrowPressed(QKeyEvent*)));

    this->menuBar->addTab(icon, title, panelArea);

    // Only show categories if more than one is available
    if (this->menuBar->count() > 1)
        this->menuBar->setTabBarShown(true);
}

void MenuWidget::addMenuLauncher(int tabIndex, Takeoff::Launcher *launcher)
{
    // Add the launcher
    PanelArea *panelArea = (PanelArea*)this->menuBar->tabAt(tabIndex);

    if (panelArea == NULL || launcher == NULL)
        return;

    panelArea->addLauncher(launcher);
}

void MenuWidget::reloadFavorites()
{
    Config *cfg = Config::getInstance();
    if (cfg->getSettings(Config::SHOW_FAVORITES).toBool()) {
        PanelArea *panelArea = (PanelArea*)this->menuBar->tabAt(0);
        panelArea->removeAllLaunchers();

        Favorites *favorites = Favorites::getInstance();
        QList<Takeoff::Launcher*> favoritesList = favorites->getFavorites();

        foreach (Takeoff::Launcher *launcher, favoritesList) {
            this->addMenuLauncher(0, launcher);
        }
    }
}


// ************************************************************************** //
// **********                      GET/SET/IS                      ********** //
// ************************************************************************** //

int MenuWidget::getNumMenuCategories() const
{
    return this->menuBar->count();
}

void MenuWidget::setFirstCategory()
{
    this->menuBar->setCurrentIndex(0);
}

void MenuWidget::setCurrentIndex(int index)
{
    if (index >= 0 && index < this->menuBar->count())
    {
        this->menuBar->setCurrentIndex(index);
    }
}

int MenuWidget::tabCount() const
{
    return this->menuBar->count();
}

int MenuWidget::currentIndex() const
{
    return this->menuBar->currentIndex();
}

void MenuWidget::keyPressed(QKeyEvent *event)
{
    PanelArea *panelArea = dynamic_cast<PanelArea*>(menuBar->tabAt(menuBar->currentIndex()));
    panelArea->slotArrowPressed(event);
}
