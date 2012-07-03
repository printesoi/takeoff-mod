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
#include "ConfigForm.h"
#include "Config.h"
#include "ui_ConfigForm.h"
#include <KDE/KFileDialog>
#include <KDE/KIconDialog>

// ************************************************************************** //
// **********              CONSTRUCTORS AND DESTRUCTOR             ********** //
// ************************************************************************** //

ConfigForm::ConfigForm(QWidget *parent)
        : QWidget(parent),
          ui(new Ui::ConfigForm),
          cfg(Config::getInstance())
{
    ui->setupUi(this);

    // Load configuration
    this->ui->showFavorites->setChecked(
            this->cfg->getSettings(Config::SHOW_FAVORITES).toBool());
    this->ui->showAllApplications->setChecked(
            this->cfg->getSettings(Config::SHOW_ALL_APPLICATIONS).toBool());
    this->ui->showXdgMenu->setChecked(
            this->cfg->getSettings(Config::SHOW_XDG_MENU).toBool());
    this->ui->launcherSize->setValue(
            this->cfg->getSettings(Config::LAUNCHER_SIZE).toInt());
    this->ui->separationSize->setValue(this->cfg->getSettings(
            Config::SEPARATION_BETWEEN_LAUNCHERS).toInt());
    this->ui->numRows->setValue(
            this->cfg->getSettings(Config::NUM_ROWS).toInt());
    this->ui->numColumns->setValue(
            this->cfg->getSettings(Config::NUM_COLUMNS).toInt());
    this->ui->iconBtn->setIcon(
            KIcon(this->cfg->getSettings(Config::ICON).toString()));
    this->ui->rememberLastTab->setChecked(
            this->cfg->getSettings(Config::REMEMBER_LAST_TAB).toBool());
    this->ui->showIconText->setChecked(
            this->cfg->getSettings(Config::SHOW_ICON_TEXT).toBool());
    this->ui->fullScreen->setChecked(
            this->cfg->getSettings(Config::FULL_SCREEN).toBool());
    this->ui->showBackgroundImage->setChecked(
            this->cfg->getSettings(Config::SHOW_BACKGROUND_IMAGE).toBool());
    this->ui->backgroundImageText->setText(
            this->cfg->getSettings(Config::BACKGROUND_IMAGE).toString());
    this->ui->backgroundImageText->setEnabled(
            this->cfg->getSettings(Config::SHOW_BACKGROUND_IMAGE).toBool());
    this->ui->selectImageBtn->setIcon(QIcon::fromTheme("document-open"));
    this->ui->selectImageBtn->setEnabled(
            this->cfg->getSettings(Config::SHOW_BACKGROUND_IMAGE).toBool());
}

ConfigForm::~ConfigForm()
{
    delete ui;
}



// ************************************************************************** //
// **********                    PRIVATE SLOTS                     ********** //
// ************************************************************************** //

void ConfigForm::on_showFavorites_clicked()
{
    this->cfg->setSettings(Config::SHOW_FAVORITES,
            this->ui->showFavorites->isChecked());
}

void ConfigForm::on_showAllApplications_clicked()
{
    this->cfg->setSettings(Config::SHOW_ALL_APPLICATIONS,
            this->ui->showAllApplications->isChecked());
}

void ConfigForm::on_showXdgMenu_clicked()
{
    this->cfg->setSettings(Config::SHOW_XDG_MENU,
            this->ui->showXdgMenu->isChecked());
}

void ConfigForm::on_launcherSize_valueChanged(int value)
{
    this->cfg->setSettings(Config::LAUNCHER_SIZE, value);
}

void ConfigForm::on_separationSize_valueChanged(int value)
{
    this->cfg->setSettings(Config::SEPARATION_BETWEEN_LAUNCHERS, value);
}

void ConfigForm::on_numRows_valueChanged(int value)
{
    this->cfg->setSettings(Config::NUM_ROWS, value);
}

void ConfigForm::on_numColumns_valueChanged(int value)
{
    this->cfg->setSettings(Config::NUM_COLUMNS, value);
}

void ConfigForm::on_iconBtn_clicked()
{
    KIconDialog dlg;
    KIcon icon = KIcon(dlg.getIcon());
    if (!icon.name().isEmpty()) {
        this->ui->iconBtn->setIcon(icon);
        this->cfg->setSettings(Config::ICON, icon.name());
    }
}

void ConfigForm::on_rememberLastTab_clicked()
{
    this->cfg->setSettings(Config::REMEMBER_LAST_TAB,
            this->ui->rememberLastTab->isChecked());
}

void ConfigForm::on_showIconText_clicked()
{
    this->cfg->setSettings(Config::SHOW_ICON_TEXT,
            this->ui->showIconText->isChecked());
}

void ConfigForm::on_fullScreen_clicked()
{
    this->cfg->setSettings(Config::FULL_SCREEN,
            this->ui->fullScreen->isChecked());
}

void ConfigForm::on_showBackgroundImage_clicked()
{
    this->cfg->setSettings(Config::SHOW_BACKGROUND_IMAGE,
            this->ui->showBackgroundImage->isChecked());
}

void ConfigForm::on_backgroundImageText_editingFinished()
{
    this->cfg->setSettings(Config::BACKGROUND_IMAGE,
            this->ui->backgroundImageText->text());
}

void ConfigForm::on_selectImageBtn_clicked()
{
    QString fileName = KFileDialog::getOpenFileName();
    if (!fileName.isEmpty()) {
        this->ui->backgroundImageText->setText(fileName);
        this->cfg->setSettings(Config::BACKGROUND_IMAGE,
                this->ui->backgroundImageText->text());
    }
}
