/**
 * @file /src/takeoff/takeoff_widget/menu/MenuWidget.h
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
#ifndef TAKEOFFWIDGET_PARTS_PANEL_H
#define TAKEOFFWIDGET_PARTS_PANEL_H

#include <QtGui/QGraphicsWidget>
class QGraphicsGridLayout;
namespace Plasma  { class TabBar; }
namespace Takeoff { class Launcher; }
namespace TakeoffPrivate  {

/**
 * Widget that contains a Plasma::TabBar to show the differents menu categories
 * and launchers. Each tab contains a PanelArea.
 */
class MenuWidget : public QGraphicsWidget
{
    Q_OBJECT

public:

    /**
     * Default constructor.
     * @param parent Parent of the widget.
     */
    MenuWidget(QGraphicsWidget *parent = 0);

    //--------------------------------------------------------------------------

    /**
     * Adds a new tab with the specified icon and title.
     * @param icon  The icon that will appear at the left of the title. Pass a
     *        null-icon to not display icon.
     * @param title The text to display next to the icon.
     */
    void addMenuCategory(const QIcon &icon, const QString &title);

    /**
     * Adds a new launcher to the specified tab. If the index is incorrect or
     * the launcher is NULL it won't effect.
     * @param tabIndex The index of the tab where the launcher will be added.
     * @param launcher The launcher to add.
     */
    void addMenuLauncher(int tabIndex, Takeoff::Launcher *launcher);

    /**
     * Reloads the favorites.
     */
    void reloadFavorites();

    //--------------------------------------------------------------------------

    /**
     * Returns the count of menu categories.
     * @return The count.
     */
    int getNumMenuCategories() const;

    /**
     * Shows the first tab.
     */
    void setFirstCategory();

    /**
      * Show a tab
      */
    void setCurrentIndex(int index);

    /**
      * Number of tabs
      */
    int tabCount() const;

    /**
      * The index of the current visible tab
      */
    int currentIndex() const;

public slots:

    /**
      * Handle Enter/Return and arrow keys
      * @param event Keyboard event
      */
    void keyPressed(QKeyEvent* event);

signals:

    /**
     * Signal that is emitted whenever the user click on one launcher.
     */
    void clicked() const;

    /**
     * Signal that is emitted when a launcher is added to favorites.
     */
    void addedToFavorites() const;

    /**
     * Signal that is emitted when a launcher is removed from favorites.
     */
    void removedFromFavorites() const;

    /**
      * Signal that is emitted when Enter/Return or an arrow key is pressed
      */
    void signalArrowPressed(QKeyEvent* event);

private:

    /// Tab bar to show the different menu launchers and categories.
    Plasma::TabBar *menuBar;

};

}      // End namespace
#endif // TAKEOFFWIDGET_PARTS_PANEL_H
