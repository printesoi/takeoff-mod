/**
 * @file /src/takeoff/takeoff_widget/TakeoffWidget.cpp
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
 * @class  Takeoff::TakeoffWidget
 */
#include "TakeoffWidget.h"
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QPainter>
#include <KDE/Plasma/TabBar>
#include "Launcher.h"
#include "menu/MenuWidget.h"
#include "search/SearchWidget.h"
#include "../model/config/Config.h"

#include <kdebug.h>
using namespace Takeoff;
using namespace TakeoffPrivate;
using namespace Plasma;

// ************************************************************************** //
// **********              CONSTRUCTORS AND DESTRUCTOR             ********** //
// ************************************************************************** //

TakeoffWidget::TakeoffWidget(QGraphicsWidget *parent)
        : Plasma::Applet(parent, "plasma-applet-takeoff.desktop"),
          tabBar(NULL),
          menuWidget(NULL),
          searchWidget(NULL)
{
    this->reset();
}


// ************************************************************************** //
// **********                    PUBLIC METHODS                    ********** //
// ************************************************************************** //

void TakeoffWidget::addMenuCategory(const QIcon &icon, const QString &title)
{
    this->menuWidget->addMenuCategory(icon, title);
}

void TakeoffWidget::addMenuLauncher(int tabIndex, Launcher *launcher)
{
    this->menuWidget->addMenuLauncher(tabIndex, launcher);
}

void TakeoffWidget::reset()
{
    Config::loadConfig();

    // Contruct widgets
    delete this->tabBar;
    this->tabBar       = new TabBar(this);
    this->menuWidget   = new MenuWidget(this->tabBar);
    this->searchWidget = new SearchWidget(this->tabBar);

    // Signals and slots
    connect(this->menuWidget, SIGNAL(clicked()), this, SIGNAL(clicked()));
    connect(this->menuWidget, SIGNAL(addedToFavorites()),
            this, SLOT(reloadFavorites()));
    connect(this->menuWidget, SIGNAL(removedFromFavorites()),
            this, SLOT(reloadFavorites()));
    connect(this, SIGNAL(keyPressedInMenu(QKeyEvent*)),
            this->menuWidget, SLOT(keyPressed(QKeyEvent*)));

    connect(this->searchWidget, SIGNAL(clicked()), this, SIGNAL(clicked()));
    connect(this->searchWidget, SIGNAL(exitSearch()), this, SLOT(exitSearch()));
    connect(this->searchWidget, SIGNAL(addedToFavorites()),
            this, SLOT(reloadFavorites()));
    connect(this->searchWidget, SIGNAL(removedFromFavorites()),
            this, SLOT(reloadFavorites()));
    connect(this, SIGNAL(keyPressedInSearch(QKeyEvent*)),
            this->searchWidget, SLOT(keyPressed(QKeyEvent*)));

    // Never show the tabBar bar
    this->tabBar->setTabBarShown(false);

    // Set the categoriesBar at first tab
    this->tabBar->addTab("Menu area",   this->menuWidget);
    this->tabBar->addTab("Search area", this->searchWidget);

    // Add the widgets
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(this);
    l->addItem(this->tabBar);
    this->setLayout(l);
}

//------------------------------------------------------------------------------

void TakeoffWidget::keyPressEvent(QKeyEvent *event)
{
    kDebug() << event->key();
    if (event->key() == Qt::Key_Tab)
    {
        int currIdx = this->menuWidget->currentIndex();
        int nextIdx = currIdx + 1 == this->menuWidget->tabCount() ? 0 : currIdx + 1;
        this->menuWidget->setCurrentIndex(nextIdx);
    }
    else if (event->key() == Qt::Key_Backtab)
    {
        int currIdx = this->menuWidget->currentIndex();
        int nextIdx = currIdx ? currIdx - 1 : this->menuWidget->tabCount() - 1;
        this->menuWidget->setCurrentIndex(nextIdx);
    }
    // TODO arrow keys
    else if (this->tabBar->currentIndex() != 1 &&
               (event->key() == Qt::Key_Up
                || event->key() == Qt::Key_Down
                || event->key() == Qt::Key_Left
                || event->key() == Qt::Key_Right
                || event->key() == Qt::Key_Return
                || event->key() == Qt::Key_Enter))
    {
        emit keyPressedInMenu(event);
    }
    else if (this->tabBar->currentIndex() == 1 &&
               (event->key() == Qt::Key_Up
                || event->key() == Qt::Key_Down
                || event->key() == Qt::Key_Left
                || event->key() == Qt::Key_Right
                || event->key() == Qt::Key_Return
                || event->key() == Qt::Key_Enter))
    {
        emit keyPressedInSearch(event);
    }
    // Show the search area
    else if (this->tabBar->currentIndex() != 1
            && event->key() != Qt::Key_Control
            && event->key() != Qt::Key_Shift
            && event->key() != Qt::Key_Alt
            && event->key() != Qt::Key_AltGr
            && event->key() != Qt::Key_Meta
            && event->key() != Qt::Key_Escape)
    {
        this->searchWidget->clearSearchText();
        this->searchWidget->addSearchText(event);
        this->tabBar->setCurrentIndex(1);
    }
    // Show the menu area
    else if (this->tabBar->currentIndex() == 1
             && event->key() == Qt::Key_Escape) {
        this->tabBar->setCurrentIndex(0);

    // Add text to the search box
    }
    else if (event->key() == Qt::Key_Escape)
    {
        kDebug() << "Esc";
        emit hideWindow();
        return;
    }
    else {
        this->searchWidget->addSearchText(event);
    }

    this->setFocus();
}

void TakeoffWidget::paint(QPainter *painter,
            const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    Config *cfg = Config::getInstance();
    if (cfg->getSettings(Config::SHOW_BACKGROUND_IMAGE).toBool()) {
        QPixmap img(cfg->getSettings(Config::BACKGROUND_IMAGE).toString());
        QRect imgSize(0, 0, this->size().width(), this->size().height());
        painter->drawImage(imgSize, img.toImage());
    }
}


// ************************************************************************** //
// **********                      GET/SET/IS                      ********** //
// ************************************************************************** //

int TakeoffWidget::getNumMenuCategories() const
{
    return this->menuWidget->getNumMenuCategories();
}

void TakeoffWidget::setFirstTab(bool changeMenuTab)
{
    this->tabBar->setCurrentIndex(0);

    if (changeMenuTab)
        this->menuWidget->setFirstCategory();
}


// ************************************************************************** //
// **********                    PRIVATE SLOTS                     ********** //
// ************************************************************************** //

void TakeoffWidget::exitSearch()
{
    this->tabBar->setCurrentIndex(0);
}

#include <QDebug>
void TakeoffWidget::reloadFavorites()
{
    qDebug() << "OK";
    this->menuWidget->reloadFavorites();
}
