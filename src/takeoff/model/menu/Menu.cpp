/**
 * @file /src/takeoff/model/Menu.cpp
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
#include "Menu.h"
#include <KDE/KIcon>
#include "qtxdg/xdgmenu.h"

// ************************************************************************** //
// **********             STATIC METHODS AND VARIABLES             ********** //
// ************************************************************************** //

Menu* Menu::instance = NULL;

Menu* Menu::getInstance()
{
    if (Menu::instance == NULL)
        Menu::instance = new Menu();

    return Menu::instance;
}

void Menu::loadMenu()
{
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
    Menu::getInstance();
}



// ************************************************************************** //
// **********              CONSTRUCTORS AND DESTRUCTOR             ********** //
// ************************************************************************** //

Menu::Menu()
        : allApplications(new QList<Takeoff::Launcher*>),
          categories(new QList< QPair<QString, KIcon>* >),
          categoriesApplications(new QList< QList<Takeoff::Launcher*>* >)
{
    QString menuFile = XdgMenu::getMenuFileName();
    XdgMenu xdgMenu;
    xdgMenu.environments() << "KDE";

    bool res = xdgMenu.read(menuFile);
    if (!res)
        qFatal("Error loading xdg-menu");

    QDomNode rootNode = xdgMenu.xml().firstChild();
    QDomNode categorieNode = rootNode.firstChild();

    for (categorieNode.firstChild(); !categorieNode.isNull();
            categorieNode=categorieNode.nextSibling()) {
        QDomElement categorieElem = categorieNode.toElement();
        if (categorieElem.isNull())
            continue;

        // Category
        if (categorieElem.tagName() == "Menu"
                && !categorieElem.attribute("title").startsWith(".")) {
            // Save the category
            QPair<QString, KIcon> *category = new QPair<QString, KIcon>;
            category->first  = categorieElem.attribute("title");
            category->second = KIcon(categorieElem.attribute("icon"));
            this->categories->append(category);

            // Add the category to the categoriesApplications list
            QList<Takeoff::Launcher*>* l = new QList<Takeoff::Launcher*>;
            this->categoriesApplications->append(l);

            // Save the category applications
            this->saveApplications(categorieNode.firstChild(),
                    this->categories->length()-1);
        }
    }
}

Menu::~Menu()
{
    qDeleteAll(this->categories->begin(), this->categories->end());
    delete this->allApplications;
    delete this->categories;
    delete this->categoriesApplications;
}


// ************************************************************************** //
// **********                   PRIVATE METHODS                    ********** //
// ************************************************************************** //

void Menu::saveApplications(QDomNode node, int categoryIndex)
{
    for (node.firstChild(); !node.isNull(); node=node.nextSibling()) {
        QDomElement elem = node.toElement();
        if(elem.isNull())
            continue;

        // Launcher
        if(elem.tagName() == "AppLink") {
            Takeoff::Launcher* launcher = new Takeoff::Launcher(
                    KIcon(elem.attribute("icon")),
                    elem.attribute("title"),
                    elem.attribute("desktopFile"));

            this->allApplications->append(launcher);
            this->categoriesApplications->at(categoryIndex)->append(launcher);

        // Submenu
        } else {
            this->saveApplications(node.firstChild(), categoryIndex);
        }
    }
}


// ************************************************************************** //
// **********                      GET/SET/IS                      ********** //
// ************************************************************************** //

QList<Takeoff::Launcher*> *Menu::getAllApplications() const
{
    return this->allApplications;
}

QList< QPair<QString, KIcon>* > *Menu::getCategories() const
{
    return this->categories;
}

QList<Takeoff::Launcher*> *Menu::getCategoriesApplications(int categoryIndex)
        const
{
    return this->categoriesApplications->at(categoryIndex);
}
