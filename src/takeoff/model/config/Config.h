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
#ifndef CONFIG_CONFIG_H
#define CONFIG_CONFIG_H

#include <QtCore/QObject>
#include <QtCore/QHash>
class QSettings;

/**
 * Singleton that provides access to the configuration.
 */
class Config : public QObject
{

public:

    /**
     * Only method to get an instance of the class.
     * @return The single instance of the class.
     */
    static Config* getInstance();

    /**
     * (Re)Loads the configuration.
     */
    static void loadConfig();

    //--------------------------------------------------------------------------

    // Strings to access to the configuration
    static const char *SHOW_FAVORITES;
    static const char *SHOW_ALL_APPLICATIONS;
    static const char *SHOW_XDG_MENU;
    static const char *LAUNCHER_SIZE;
    static const char *SEPARATION_BETWEEN_LAUNCHERS;
    static const char *NUM_ROWS;
    static const char *NUM_COLUMNS;
    static const char *ICON;
    static const char *REMEMBER_LAST_TAB;
    static const char *SHOW_ICON_TEXT;
    static const char *FULL_SCREEN;
    static const char *SHOW_BACKGROUND_IMAGE;
    static const char *BACKGROUND_IMAGE;

    /**
     * Returns the specified property.
     * @param  propertyName The property to get.
     * @return The property.
     */
    QVariant getSettings(const char *propertyName) const;

    /**
     * Sets the specified property.
     * @param propertyName The name of the property to set.
     * @param value        The value to set.
     */
    void setSettings(const char *propertyName, const QVariant &value);

    /**
     * Returns the path of the configuration file.
     * @return The path.
     */
    QString getConfigFilePath() const;

private:

    /**
     * Load the specified property. If is not available, puts the default value.
     * @param property The property to load. Is one of the Config::Property.
     * @param defaultValue The value to set if the property doesn't exist.
     */
    void loadProperty(const char *property, const QVariant &defaultValue);

    //--------------------------------------------------------------------------

    /// Object to access to the settings
    QSettings *settings;

    /// Hash with the configuration
    QHash<QString, QVariant> hashSettings;

    /// Single instance of the class.
    static Config* instance;

    // Hide constructors
    Config();
    Config(const Config&);
    const Config &operator = (const Config&);
};

#endif // CONFIG_CONFIG_H
