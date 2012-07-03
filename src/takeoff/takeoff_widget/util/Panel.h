/**
 * @file /src/takeoff/takeoff_widget/util/Panel.h
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
 * @class  TakeoffPrivate::Panel
 */
#ifndef TAKEOFFWIDGET_UTIL_PANEL_H
#define TAKEOFFWIDGET_UTIL_PANEL_H

#include <QtGui/QGraphicsWidget>
#include <Plasma/ItemBackground>
class QGraphicsGridLayout;
namespace Takeoff { class Launcher; }
namespace TakeoffPrivate  {

/**
 * Widget that contains a number of launchers, this number will depends of the
 * size of the applet or screen.
 */
class Panel : public QGraphicsWidget
{
    Q_OBJECT

public:

    /**
     * Default constructor.
     * @param parent Parent of the widget.
     */
    Panel(QGraphicsWidget *parent = 0);

    /**
     * Deletes itself and all the launchers.
     */
    virtual ~Panel();

    //--------------------------------------------------------------------------

    /**
     * Adds a launcher in the first free position. If the panel is full haven't
     * got effect.
     * @param launcher The launcher to add.
     */
    void addLauncher(Takeoff::Launcher *launcher);

    /**
     * Removes all the launchers.
     */
    void removeAllLaunchers();

    //--------------------------------------------------------------------------

    /**
     * Returns the specified launcher or NULL if any launcher is not available.
     * @param  index The index of the launcher.
     * @return The launcher.
     */
    Takeoff::Launcher *getLauncher(int index) const;

    /**
     * Indicates if the panel is full (have 32 launchers) or no.
     * @return If is full or not.
     */
    bool isFull() const;

    /**
      * Handle Enter/Return and Arrow keys
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

private:

    /// Layout to add the different Launchers.
    QGraphicsGridLayout *panelLayout;

    /// Number of rows of the panel.
    int numRows;

    /// Number of columns of the panel.
    int numColumns;

    /// List with all the launchers that contains the panel.
    QList<Takeoff::Launcher*> launchers;

    /// Hover indicator to highlight thethe  focused item
    Plasma::ItemBackground* m_hoverIndicator;

    /// True if the panel has an item focused
    bool focused;

    /// The column of current focused item, -1 if none
    int colFocused;

    /// The row of current focused item, -1 if none
    int rowFocused;

};

}      // End namespace
#endif // TAKEOFFWIDGET_UTIL_PANEL_H
