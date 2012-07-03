/**
 * @file /src/takeoff/takeoff_widget/menu/PanelArea.cpp
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
 * @class  TakeoffPrivate::PanelArea
 */
#include "PanelArea.h"

//#include <QtGui/QGraphicsLinearLayout>
//{ HACK
#include <QtGui/QGraphicsGridLayout>
#include <KDE/Plasma/Label>
//}

#include <KDE/Plasma/TabBar>
#include <KDE/Plasma/FrameSvg>
#include "../Launcher.h"
#include "../util/Panel.h"
using namespace Takeoff;
using namespace TakeoffPrivate;

// ************************************************************************** //
// **********              CONSTRUCTORS AND DESTRUCTOR             ********** //
// ************************************************************************** //

PanelArea::PanelArea(QGraphicsWidget *parent)
        : QGraphicsWidget(parent),
          panelTabBar(new Plasma::TabBar(this)),
          panelSelector(new Plasma::TabBar(this))
{
    // TODO For the moment Plasma::TabBar don't allow RoundedSouth (line 183 in
    //      http://api.kde.org/4.x-api/kdelibs-apidocs/plasma/html/tabbar_8cpp_source.html)
    //      Change this in the future and remove the panelSelector

    //this->panelTabBar->setTabBarShown(false);
    //KTabBar *ktab = this->panelTabBar->nativeWidget();
    //ktab->setShape(QTabBar::RoundedSouth);

    // Set the layout
    //QGraphicsLinearLayout *l = new QGraphicsLinearLayout(this);
    //l->addItem(this->panelTabBar);
    //this->setLayout(l);

    //{
    this->panelTabBar->setTabBarShown(false);
    this->panelSelector->setTabBarShown(false);

    connect(this->panelSelector, SIGNAL(currentChanged(int)),
            this->panelTabBar, SLOT(setCurrentIndex(int)));

    QGraphicsGridLayout *l = new QGraphicsGridLayout(this);
    Plasma::Label *separator1 = new Plasma::Label;
    Plasma::Label *separator2 = new Plasma::Label;

    l->setRowMaximumHeight(1, 10);
    l->addItem(this->panelTabBar, 0, 0, 1, 3);
    l->addItem(separator1, 1, 0);
    l->addItem(this->panelSelector, 1, 1);
    l->addItem(separator2, 1, 2);
    this->setLayout(l);
    //}
}


// ************************************************************************** //
// **********                   PRIVATE METHODS                    ********** //
// ************************************************************************** //

void PanelArea::addPanel()
{
    Panel *newPanel = new Panel(this);
    connect(newPanel, SIGNAL(clicked()), this, SIGNAL(clicked()));
    connect(newPanel, SIGNAL(addedToFavorites()),
            this, SIGNAL(addedToFavorites()));
    connect(newPanel, SIGNAL(removedFromFavorites()),
            this, SIGNAL(removedFromFavorites()));

    this->panelTabBar->addTab("", newPanel);
    this->panelSelector->addTab(
            QString::number(this->panelTabBar->count()));
}

// ************************************************************************** //
// **********                    PUBLIC METHODS                    ********** //
// ************************************************************************** //

void PanelArea::addLauncher(Launcher *launcher)
{
    // If the panel area is empty add a new panel
    if (this->panelTabBar->count() == 0)
        this->addPanel();

    Panel *lastPanel = (Panel*)this->panelTabBar->tabAt(
            this->panelTabBar->count()-1);

    // If the panel is NOT full add the launcher
    if (!lastPanel->isFull()) {
        lastPanel->addLauncher(launcher);

    // If the panel is full create a new panel to add the launcher
    } else {
        // Add a new panel
        this->addPanel();

        // Add the launcher to the new panel
        Panel *lastPanel = (Panel*)this->panelTabBar->tabAt(
                this->panelTabBar->count()-1);
        lastPanel->addLauncher(launcher);

        // Show the selection bar
        this->panelSelector->setTabBarShown(true);

        // Resize the bar
        ((QGraphicsGridLayout*)this->layout())->setColumnMaximumWidth(1,
                this->panelTabBar->count()*50);
    }
}

void PanelArea::removeAllLaunchers()
{
    int count = this->panelTabBar->count()-1;
    for (int n=count; n>=0; n--) {
        Panel *panel = (Panel*)this->panelTabBar->tabAt(n);
        this->panelTabBar->removeTab(n);
        this->panelSelector->removeTab(n);
        delete panel;
    }

    this->panelTabBar->setTabBarShown(false);
    this->panelSelector->setTabBarShown(false);
}

void PanelArea::slotArrowPressed(QKeyEvent *event)
{
    Panel* panel = dynamic_cast<Panel*>(panelTabBar->tabAt(panelTabBar->currentIndex()));
    panel->keyPressed(event);
}
