/**
 * @file /src/takeoff/takeoff_widget/TakeoffWidget.h
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
#ifndef TAKEOFFWIDGET_TAKEOFFWIDGET_H
#define TAKEOFFWIDGET_TAKEOFFWIDGET_H

#include <KDE/Plasma/Applet>
namespace Plasma         { class TabBar; }
namespace Takeoff        { class Launcher; }
namespace TakeoffPrivate { class MenuWidget; class SearchWidget; }
namespace Takeoff {

/**
 * A widget with the public interface of Takeoff. Is composed for a
 * Plasma::TabBar that have two differents areas. The first area is the menu
 * interface, with the categories and launchers. The second area is the search
 * widget, that will be show when the user begin to type.
 */
class TakeoffWidget : public Plasma::Applet
{
    Q_OBJECT

public:

    /**
     * Default constructor.
     * @param parent Parent of the widget.
     */
    TakeoffWidget(QGraphicsWidget *parent = 0);

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
    void addMenuLauncher(int tabIndex, Launcher *launcher);

    /**
     * Reset the widget to the empty state.
     */
    void reset();

    //--------------------------------------------------------------------------

    /**
     * Inherit from QGraphicsItem. Called when a key is pressed. Launch the
     * search dialog.
     */
    virtual void keyPressEvent(QKeyEvent *event);

    /**
     * Overloaded to hide the popup menu.
     */
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *) {}

    /**
     * Paints the background image, if any.
     */
    virtual void paint(QPainter *painter,
            const QStyleOptionGraphicsItem *option, QWidget *widget);

    //--------------------------------------------------------------------------

    /**
     * Returns the count of menu categories.
     * @return The count.
     */
    int getNumMenuCategories() const;

    /**
     * Shows the first tab.
     * @param changeMenuTab For remember or not the last menu tab.
     */
    void setFirstTab(bool changeMenuTab);

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
      * Signal that is emitted when a arrow key or the return is hit in the
      * menu tab
      */
    void keyPressedInMenu(QKeyEvent* event);

    /**
      * Signal that is emitted when a arrow key or the return is hit in the
      * search tab
      */
    void keyPressedInSearch(QKeyEvent* event);

    /**
      * Signal that is emitted when the applet must be hidden
      */
    void hideWindow();

private slots:

    /// Shows the menu area.
    void exitSearch();

    /// reloads the favorites
    void reloadFavorites();

private:

    /// Tab bar with the two different areas, the menu and the search area.
    Plasma::TabBar *tabBar;

    /// Widget with the different categories, placed on the first tab of tabBar.
    TakeoffPrivate::MenuWidget *menuWidget;

    /// Widget with the search interface, placed on the second tab of tabBar.
    TakeoffPrivate::SearchWidget *searchWidget;

};

}      // End namespace
#endif // TAKEOFFWIDGET_TAKEOFFWIDGET_H
