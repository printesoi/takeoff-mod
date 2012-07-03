/**
 * @file /src/takeoff/takeoff_widget/search/SearchWidget.cpp
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
#include "SearchWidget.h"
#include <QtGui/QGraphicsGridLayout>
#include <KDE/KIcon>
#include <KDE/Plasma/IconWidget>
#include <KDE/Plasma/LineEdit>
#include "../util/Panel.h"
#include "../Launcher.h"
#include "../../model/menu/Menu.h"
using namespace TakeoffPrivate;

// ************************************************************************** //
// **********              CONSTRUCTORS AND DESTRUCTOR             ********** //
// ************************************************************************** //

SearchWidget::SearchWidget(QGraphicsWidget *parent)
        : QGraphicsWidget(parent),
          goBack(new Plasma::IconWidget(this)),
          searchBox(new Plasma::LineEdit(this)),
          resultsPanel(new Panel(this))
{
    // Set widgets properties
    this->goBack->setIcon("arrow-left");
    this->goBack->setMaximumIconSize(QSizeF(24, 24));
    connect(this->goBack, SIGNAL(clicked()), this, SIGNAL(exitSearch()));

    this->searchBox->setMaximumWidth(300);
    this->searchBox->setClearButtonShown(true);
    this->searchBox->setFocusPolicy(Qt::NoFocus);
    connect(this->searchBox, SIGNAL(textChanged(QString)),
            this, SLOT(search(QString)));

    connect(this->resultsPanel, SIGNAL(clicked()), this, SIGNAL(clicked()));
    connect(this->resultsPanel, SIGNAL(addedToFavorites()),
            this, SIGNAL(addedToFavorites()));
    connect(this->resultsPanel, SIGNAL(removedFromFavorites()),
            this, SIGNAL(removedFromFavorites()));

    // Add the widgets
    QGraphicsGridLayout *l = new QGraphicsGridLayout(this);
    l->addItem(this->goBack, 0, 0);
    l->addItem(this->searchBox, 0, 1);
    l->addItem(resultsPanel, 1, 0, 1, 2);
    this->setLayout(l);
}


// ************************************************************************** //
// **********                     PRIVATE SLOTS                    ********** //
// ************************************************************************** //

void SearchWidget::search(const QString &text)
{
    // Remove all the current launchers
    this->resultsPanel->removeAllLaunchers();

    if (text.length() < 2 || text.trimmed().isEmpty())
        return;

    // Get all the applications
    Menu *menu = Menu::getInstance();
    QList<Takeoff::Launcher*> *apps = menu->getAllApplications();

    // Prepare the regular expression to find matches
    QRegExp reg("*" + text.toLower() + "*");
    reg.setPatternSyntax(QRegExp::Wildcard);

    int n = 0;
    while (n<apps->length() && !this->resultsPanel->isFull()) {
        Takeoff::Launcher launcher = *(apps->at(n));
        QString name = launcher.getName().toLower();

        // If the application match with the search
        if (reg.exactMatch(name)) {
            Takeoff::Launcher *aux = new Takeoff::Launcher(launcher);
            this->resultsPanel->addLauncher(aux);
        }

        n++;
    }
}


// ************************************************************************** //
// **********                    PUBLIC METHODS                    ********** //
// ************************************************************************** //

void SearchWidget::addSearchText(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        emit this->clicked();
        Takeoff::Launcher *launcher = this->resultsPanel->getLauncher(0);
        if (launcher != NULL)
            launcher->runApplication();

    } else if (event->key() == Qt::Key_Backspace)
        this->searchBox->setText(this->searchBox->text().mid(
                0, this->searchBox->text().length()-1));
    else
        this->searchBox->setText(this->searchBox->text() + event->text());
}

void SearchWidget::clearSearchText()
{
    this->searchBox->setText("");
    this->resultsPanel->removeAllLaunchers();
}

void SearchWidget::keyPressed(QKeyEvent *event)
{
    this->resultsPanel->keyPressed(event);
}
