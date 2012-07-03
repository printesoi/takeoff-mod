/**
 * @file /src/takeoff/MainWindow.h
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
#ifndef TAKEOFF_MAIN_WINDOW_H
#define TAKEOFF_MAIN_WINDOW_H

#include <KDE/Plasma/PopupApplet>
class QDomNode;
namespace Takeoff { class TakeoffWidget; }


/**
 * Main window of the plasmoid and start point of the same. All components are
 * placed into this window.
 */
class MainWindow : public Plasma::PopupApplet
{
    Q_OBJECT

public:

    /**
     * Constructor.
     * @param parent The parent of the applet.
     * @param args   A list of strings containing two entries: the service id
     *               and the applet id.
     */
    MainWindow(QObject *parent, const QVariantList &args);

    //--------------------------------------------------------------------------

    /**
     * Inherit from Plasma::PopupApplet. Returns the widget that must be show in
     * the popup window.
     * @return The widget.
     */
    virtual QGraphicsWidget *graphicsWidget();

    /**
     * Inherit from Plasma::Applet. Creates the configuration interface, adding
     * the Takeoff configuration GUI.
     * @param parent The parent KConfigDialog where we will add the Takeoff GUI.
     */
    virtual void createConfigurationInterface(KConfigDialog *parent);

    /**
     * Inherit from Plasma::Applet. Adds actions to the right click menu.
     */
    virtual QList<QAction*> contextualActions();

private slots:

    /// Loads the configuration using the private methods load*
    void loadConfig();

    /// Launch the KDE menu editor.
    void launchMenuEditor() const;

    void slotHide();

private:

    /// Loads the favorites applications.
    void loadFavorites();

    /// Load all applications in the same tab.
    void loadAllApplications();

    /// Loads the xdg-menu.
    void loadXdgMenu();

    //--------------------------------------------------------------------------

    /// The widget with the menu and the search dialog.
    Takeoff::TakeoffWidget *takeoff;
};

#endif // TAKEOFF_MAIN_WINDOW_H
