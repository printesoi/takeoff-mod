/**
 * @file /src/takeoff/MainWindow.cpp
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
 * @class  MainWindow
 */
#include "MainWindow.h"
#include <QtCore/QList>
#include <QtCore/QTranslator>
#include <QtCore/QProcess>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QAction>
#include <KDE/KIcon>
#include <KDE/KConfigDialog>
#include <KDE/Plasma/ToolTipContent>
#include <KDE/Plasma/ToolTipManager>
#include "takeoff_widget/TakeoffWidget.h"
#include "takeoff_widget/Launcher.h"
#include "model/favorites/Favorites.h"
#include "model/menu/Menu.h"
#include "model/config/Config.h"
#include "model/config/ConfigForm.h"

using namespace Plasma;
using namespace Takeoff;

// ************************************************************************** //
// **********                        OTHERS                        ********** //
// ************************************************************************** //

// Links the applet name (X-KDE-PluginInfo-Name in the desktop file) with this
// class
K_EXPORT_PLASMA_APPLET(takeoff, MainWindow)


// ************************************************************************** //
// **********              CONSTRUCTORS AND DESTRUCTOR             ********** //
// ************************************************************************** //

MainWindow::MainWindow(QObject *parent, const QVariantList &args)
        : Plasma::PopupApplet(parent, args),
          takeoff(new TakeoffWidget(this))
{
    // Plasmoid aspect
    this->setBackgroundHints(DefaultBackground);
    this->setAspectRatioMode(Plasma::IgnoreAspectRatio);

    // Hide the popup when an application is launched
    connect(this->takeoff, SIGNAL(clicked()), this, SLOT(hidePopup()));

    // Load the configuration
    this->loadConfig();

    // Tooltip
    Plasma::ToolTipContent data;
    data.setMainText(i18n("Takeoff Launcher"));
    data.setSubText(i18n("Launch your applications with elegance"));
    data.setImage(this->popupIcon().pixmap(IconSize(KIconLoader::Desktop)));
    Plasma::ToolTipManager::self()->setContent(this, data);

    connect(this->takeoff, SIGNAL(hideWindow()),
            this, SLOT(slotHide()));
}


// ************************************************************************** //
// **********                    PRIVATE SLOTS                     ********** //
// ************************************************************************** //

void MainWindow::loadConfig()
{
    Config::loadConfig();
    Menu::loadMenu();

    this->takeoff->reset();

    Config *cfg = Config::getInstance();

    // Set the popup icon
    this->setPopupIcon(KIcon(cfg->getSettings(Config::ICON).toString()));

    // Set fullscreen mode or not
    if (cfg->getSettings(Config::FULL_SCREEN).toBool()) {
        this->takeoff->setMinimumWidth(QApplication::desktop()->width());
        this->takeoff->setMinimumHeight(QApplication::desktop()->height());
    } else {
        int numRows      = cfg->getSettings(Config::NUM_ROWS).toInt();
        int numColumns   = cfg->getSettings(Config::NUM_COLUMNS).toInt();
        int launcherSize = cfg->getSettings(Config::LAUNCHER_SIZE).toInt();
        int sep =cfg->getSettings(Config::SEPARATION_BETWEEN_LAUNCHERS).toInt();

        this->takeoff->setMinimumWidth((numColumns+2)*(launcherSize+sep));
        this->takeoff->setMaximumWidth((numColumns+2)*(launcherSize+sep));

        this->takeoff->setMinimumHeight(numRows*(launcherSize+sep) + 200);
        this->takeoff->setMaximumHeight(numRows*(launcherSize+sep) + 200);
    }

    // Load different categories
    if (cfg->getSettings(Config::SHOW_FAVORITES).toBool())
        this->loadFavorites();

    if (cfg->getSettings(Config::SHOW_ALL_APPLICATIONS).toBool())
        this->loadAllApplications();

    if (cfg->getSettings(Config::SHOW_XDG_MENU).toBool())
        this->loadXdgMenu();
}

void MainWindow::launchMenuEditor() const
{
    QProcess* myProcess = new QProcess();
    connect(myProcess, SIGNAL(finished(int)), this, SLOT(loadConfig()));
    myProcess->start("kmenuedit --nofork");
}

void MainWindow::slotHide()
{
    hidePopup();
}


// ************************************************************************** //
// **********                    PUBLIC METHODS                    ********** //
// ************************************************************************** //

QGraphicsWidget *MainWindow::graphicsWidget()
{
    // If "remember last tab" is not checked go to the first tab (favorites)
    Config *cfg = Config::getInstance();
    this->takeoff->setFirstTab(!cfg->getSettings(
            Config::REMEMBER_LAST_TAB).toBool());

    return this->takeoff;
}

void MainWindow::createConfigurationInterface(KConfigDialog *parent)
{
    parent->addPage(new ConfigForm, i18n("General settings"), "start-here-kde");
    parent->enableButtonApply(true);

    connect(parent, SIGNAL(applyClicked()), this, SLOT(loadConfig()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(loadConfig()));
}

QList<QAction*> MainWindow::contextualActions()
{
    QAction* launchMenuEditor = new QAction(i18n("Edit applications..."), this);
    connect(launchMenuEditor, SIGNAL(triggered()),
            this, SLOT(launchMenuEditor()));

    QList<QAction*> ret;
    ret.append(launchMenuEditor);
    return ret;
}


// ************************************************************************** //
// **********                   PRIVATE METHODS                    ********** //
// ************************************************************************** //

void MainWindow::loadFavorites()
{
    this->takeoff->addMenuCategory(KIcon("favorites"), i18n("Favorites"));

    Favorites *favorites = Favorites::getInstance();
    QList<Launcher*> favoritesList = favorites->getFavorites();

    foreach (Launcher *launcher, favoritesList) {
        this->takeoff->addMenuLauncher(this->takeoff->getNumMenuCategories()-1,
                launcher);
    }
}

void MainWindow::loadAllApplications()
{
    // Add the "All Applications" tab
    this->takeoff->addMenuCategory(KIcon("applications-other"),
            i18n("All Applications"));

    // Load the launchers
    Menu* menu = Menu::getInstance();
    QList<Launcher*> *allApplications = menu->getAllApplications();

    for (int n=0; n<allApplications->length(); n++) {
        Launcher *launcher = new Launcher(*(allApplications->at(n)));
        this->takeoff->addMenuLauncher(this->takeoff->getNumMenuCategories()-1,
                launcher);
    }
}

void MainWindow::loadXdgMenu()
{
    Menu* menu = Menu::getInstance();
    QList< QPair<QString, KIcon>* > *categories = menu->getCategories();

    for (int n=0; n<categories->length(); n++) {
        // Load the categories tabs
        QPair<QString, KIcon> *pair = categories->at(n);
        this->takeoff->addMenuCategory(pair->second, pair->first);

        // Load the launchers
        QList<Launcher*> *launchers = menu->getCategoriesApplications(n);
        for (int l=0; l<launchers->length(); l++) {
            Launcher *launcher = new Launcher(*(launchers->at(l)));
            this->takeoff->addMenuLauncher(
                    this->takeoff->getNumMenuCategories()-1, launcher);
        }
    }
}
