/**
 * @file /src/takeoff/config/Config.h
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
 * @class  Config
 */
#include "Config.h"
#include <QtCore/QVariant>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <KDE/KStandardDirs>
#include <KDE/KIcon>

// ************************************************************************** //
// **********             STATIC METHODS AND VARIABLES             ********** //
// ************************************************************************** //

Config *Config::instance = NULL;

Config *Config::getInstance()
{
    if (Config::instance == NULL)
        Config::instance = new Config();

    return Config::instance;
}

void Config::loadConfig()
{
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
    Config::getInstance();
}

const char *Config::SHOW_FAVORITES        = "Takeoff/ShowFavorites";
const char *Config::SHOW_ALL_APPLICATIONS = "Takeoff/ShowAllApplications";
const char *Config::SHOW_XDG_MENU         = "Takeoff/ShowXdgMenu";
const char *Config::LAUNCHER_SIZE         = "Takeoff/LauncherSize";
const char *Config::SEPARATION_BETWEEN_LAUNCHERS
        = "Takeoff/SeparationBetweenLaunchers";
const char *Config::NUM_ROWS              = "Takeoff/NumRows";
const char *Config::NUM_COLUMNS           = "Takeoff/NumColumns";
const char *Config::ICON                  = "Takeoff/Icon";
const char *Config::REMEMBER_LAST_TAB     = "Takeoff/RememberLastTab";
const char *Config::SHOW_ICON_TEXT        = "Takeoff/ShowIconText";
const char *Config::FULL_SCREEN           = "Takeoff/FullScreen";
const char *Config::SHOW_BACKGROUND_IMAGE = "Takeoff/ShowBackgroundImage";
const char *Config::BACKGROUND_IMAGE      = "Takeoff/BackgroundImage";


// ************************************************************************** //
// **********              CONSTRUCTORS AND DESTRUCTOR             ********** //
// ************************************************************************** //

Config::Config()
{
    // Get the distro icon if is available, else put the default icon
    QString icon = KIcon("start-here-branding").isNull()
            ? "start-here-kde"
            : "start-here-branding";

    // Calculate the aspect ratio of the icons (size, separation, rows...)
    int launcherSize       = 80;
    int launcherSeparation = 60;
    int screenWidth        = QApplication::desktop()->width();
    int screenHeight       = QApplication::desktop()->height();
    int numRows    = screenHeight / (launcherSize + launcherSeparation) - 2;
    int numColumns = screenWidth  / (launcherSize + launcherSeparation) - 2;

    // Adjust columns and rows size
    if (numRows <= 2)     numRows = 2;
    if (numRows >= 10)    numRows = 10;
    if (numColumns <= 3)  numColumns = 3;
    if (numColumns >= 20) numColumns = 20;

    // Load the configuration into the QHash
    this->settings = new QSettings(this->getConfigFilePath(),
            QSettings::IniFormat, this);

    this->loadProperty(SHOW_FAVORITES, true);
    this->loadProperty(SHOW_ALL_APPLICATIONS, true);
    this->loadProperty(SHOW_XDG_MENU, true);
    this->loadProperty(LAUNCHER_SIZE, launcherSize);
    this->loadProperty(SEPARATION_BETWEEN_LAUNCHERS, launcherSeparation);
    this->loadProperty(NUM_ROWS, numRows);
    this->loadProperty(NUM_COLUMNS, numColumns);
    this->loadProperty(ICON, icon);
    this->loadProperty(REMEMBER_LAST_TAB, false);
    this->loadProperty(SHOW_ICON_TEXT, true);
    this->loadProperty(FULL_SCREEN, true);
    this->loadProperty(SHOW_BACKGROUND_IMAGE, false);
    this->loadProperty(BACKGROUND_IMAGE, "");
}


// ************************************************************************** //
// **********                   PRIVATE METHODS                    ********** //
// ************************************************************************** //

void Config::loadProperty(const char *property, const QVariant &defaultValue)
{
    if (!this->settings->contains(property))
        this->settings->setValue(property, defaultValue);

    this->hashSettings.insert(property, this->settings->value(property));
}


// ************************************************************************** //
// **********                      GET/SET/IS                      ********** //
// ************************************************************************** //

QVariant Config::getSettings(const char *propertyName) const
{
    return this->hashSettings.value(propertyName);
}

void Config::setSettings(const char *propertyName, const QVariant &value)
{
    this->settings->setValue(propertyName, value);
}

QString Config::getConfigFilePath() const
{
    return KStandardDirs::locateLocal("config", "takeoffrc", true);
}
