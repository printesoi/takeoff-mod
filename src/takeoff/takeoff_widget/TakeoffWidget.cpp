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

using namespace Takeoff;
using namespace TakeoffPrivate;
using namespace Plasma;

#define IS_ARROW_OR_RETURN(x) ((x) == Qt::Key_Up \
                               || (x) == Qt::Key_Down \
                               || (x) == Qt::Key_Left \
                               || (x) == Qt::Key_Right \
                               || (x) == Qt::Key_Enter \
                               || (x) == Qt::Key_Return)

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
    if (event->key() == Qt::Key_Tab)
    {
        // Switch to the next tab when CTRL + TAB is hit
        int currIdx = this->menuWidget->currentIndex();
        int nextIdx = currIdx + 1 == this->menuWidget->tabCount() ? 0 : currIdx + 1;
        this->menuWidget->setCurrentIndex(nextIdx);
    }
    else if (event->key() == Qt::Key_Backtab)
    {
        // Switch to previous tab when CTRL + Shift + TAB is hit
        int currIdx = this->menuWidget->currentIndex();
        int nextIdx = currIdx ? currIdx - 1 : this->menuWidget->tabCount() - 1;
        this->menuWidget->setCurrentIndex(nextIdx);
    }
    else if (this->tabBar->currentIndex() != 1 && IS_ARROW_OR_RETURN(event->key()))
    {
        // Return or an Arrow was hit in the menu tab
        emit keyPressedInMenu(event);
    }
    else if (this->tabBar->currentIndex() == 1 && IS_ARROW_OR_RETURN(event->key()))
    {
        // Return or an Arrow was hit in the search tab
        emit keyPressedInSearch(event);
    }
    else if (this->tabBar->currentIndex() != 1
            && event->key() != Qt::Key_Control
            && event->key() != Qt::Key_Shift
            && event->key() != Qt::Key_Alt
            && event->key() != Qt::Key_AltGr
            && event->key() != Qt::Key_Meta
            && event->key() != Qt::Key_Escape)
    {
        // Show the search area
        this->searchWidget->clearSearchText();
        this->searchWidget->addSearchText(event);
        this->tabBar->setCurrentIndex(1);
    }
    else if (this->tabBar->currentIndex() == 1
             && event->key() == Qt::Key_Escape)
    {
        // Show the menu area
        this->tabBar->setCurrentIndex(0);

    }
    else if (event->key() == Qt::Key_Escape)
    {
        // The applet must be hidden when Escaped is pressed in the menu tab
        emit hideWindow();
        return;
    }
    else
    {
        // Add text to the search box
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
