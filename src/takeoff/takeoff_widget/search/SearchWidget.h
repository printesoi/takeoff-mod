/**
 * @file /src/takeoff/takeoff_widget/search/SearchWidget.h
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
 * @class  TakeoffPrivate::SearchWidget
 */
#ifndef TAKEOFFWIDGET_SEARCH_POPUPWINDOW_H
#define TAKEOFFWIDGET_SEARCH_POPUPWINDOW_H

#include <QtGui/QGraphicsWidget>
namespace TakeoffPrivate  { class Panel; }
namespace Plasma          { class IconWidget; class LineEdit; }
namespace TakeoffPrivate  {

/**
 * Widget with the search dialog.
 */
class SearchWidget : public QGraphicsWidget
{
    Q_OBJECT

public:

    /**
     * Default constructor.
     * @param parent Parent of the widget.
     */
    SearchWidget(QGraphicsWidget *parent = 0);

    //--------------------------------------------------------------------------

    /**
     * Concatenates the specified text into the search box.
     * @param text The text.
     */
    void addSearchText(QKeyEvent *event);

    /**
     * Clears the search box.
     */
    void clearSearchText();

public slots:

    void keyPressed(QKeyEvent* event);

private slots:

    /**
     * Called whenever the search text change.
     * @param text The text to search
     */
    void search(const QString &text);

signals:

    /**
     * Signal that is emitted whenever the user click in the go back button.
     */
    void exitSearch();

    /**
     * Signal that is emited whenever the user click in a launcher.
     */
    void clicked();

    /**
     * Signal that is emitted when a launcher is added to favorites.
     */
    void addedToFavorites() const;

    /**
     * Signal that is emitted when a launcher is removed from favorites.
     */
    void removedFromFavorites() const;

private:

    /// Button to go to the menu area.
    Plasma::IconWidget *goBack;

    /// Line edit to search
    Plasma::LineEdit *searchBox;

    /// Widget to show the search results
    Panel *resultsPanel;
};

}      // End namespace
#endif // TAKEOFFWIDGET_SEARCH_POPUPWINDOW_H

