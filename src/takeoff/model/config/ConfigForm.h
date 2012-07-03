/**
 * @file /src/takeoff/config/ConfigForm.h
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
 * @class  ConfigForm
 */
#ifndef CONFIG_CONFIGFORM_H
#define CONFIG_CONFIGFORM_H

#include <QWidget>
class Config;
namespace Ui { class ConfigForm; }

/**
 * Widget to configure Takeoff.
 */
class ConfigForm : public QWidget
{
    Q_OBJECT

public:

    ConfigForm(QWidget *parent = 0);

    ~ConfigForm();

private slots:

    void on_showFavorites_clicked();

    void on_showAllApplications_clicked();

    void on_showXdgMenu_clicked();

    void on_launcherSize_valueChanged(int value);

    void on_separationSize_valueChanged(int value);

    void on_numRows_valueChanged(int value);

    void on_numColumns_valueChanged(int value);

    void on_iconBtn_clicked();

    void on_rememberLastTab_clicked();

    void on_showIconText_clicked();

    void on_fullScreen_clicked();

    void on_showBackgroundImage_clicked();

    void on_backgroundImageText_editingFinished();

    void on_selectImageBtn_clicked();

private:

    Ui::ConfigForm *ui;
    Config *cfg;

};

#endif // CONFIG_CONFIGFORM_H
