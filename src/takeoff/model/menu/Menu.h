/**
 * @file /src/takeoff/model/Menu.h
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
 * @class  Menu
 */
#ifndef MODEL_MENU_H
#define MODEL_MENU_H

#include <QtCore/QList>
#include <QtCore/QPair>
#include "../../takeoff_widget/Launcher.h"
class QDomNode;
class KIcon;

/**
 * Class to access to the xdg-menu standard.
 */
class Menu
{

public:

    /**
     * Only method to get an instance of the class.
     * @return The single instance of the class.
     */
    static Menu* getInstance();

    /**
     * (Re)Loads the menu.
     */
    static void loadMenu();

    /**
     * Destructor.
     */
    virtual ~Menu();

    //--------------------------------------------------------------------------

    /**
     * Returns the a list with all the available applications.
     * @return The list.
     */
    QList<Takeoff::Launcher*> *getAllApplications() const;

    /**
     * Returns a list with all the categories, identified by their name and
     * their icon.
     * @return The list.
     */
    QList< QPair<QString, KIcon>* > *getCategories() const;

    /**
     * Returns a list with all applications belonging to the specified category.
     * @param  categoryIndex The position of the category in the getCategories()
     *         list.
     * @return The list.
     */
    QList<Takeoff::Launcher*> *getCategoriesApplications(int categoryIndex)
            const;

private:

    /**
     * Auxiliary function to save the launchers in the allApplications and
     * categoriesApplications lists.
     * @param node The node of the category
     * @param categoryIndex The index of the category where save the launcher.
     */
    void saveApplications(QDomNode node, int categoryIndex);

    //--------------------------------------------------------------------------

    /// List with all applications.
    QList<Takeoff::Launcher*> *allApplications;

    /// List with all categories (name and icon).
    QList< QPair<QString, KIcon>* > *categories;

    /// List with all applications belonging to one category.
    QList< QList<Takeoff::Launcher*>* > *categoriesApplications;

    //--------------------------------------------------------------------------

    /// Single instance of the class.
    static Menu* instance;

    // Hide constructors
    Menu();
    Menu(const Menu&);
    const Menu &operator = (const Menu&);

};

#endif // MODEL_MENU_H
