/**
 * @file /src/takeoff/takeoff_widget/util/Panel.cpp
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
#include "Panel.h"
#include <QtCore/QList>
#include <QtGui/QGraphicsGridLayout>
#include <QKeyEvent>
#include <KDE/Plasma/Label>
#include "../Launcher.h"
#include "../../model/config/Config.h"
using namespace TakeoffPrivate;

// ************************************************************************** //
// **********              CONSTRUCTORS AND DESTRUCTOR             ********** //
// ************************************************************************** //

Panel::Panel(QGraphicsWidget *parent)
        : QGraphicsWidget(parent),
          panelLayout(new QGraphicsGridLayout(this)),
          focused(false),
          colFocused(-1),
          rowFocused(-1)
{
    // To maintain the plasmoid at fullscreen mode, is necessary to make a hack.
    // This is the structure of the layout:
    //             0 1 . . . . . . . NUM_COLUMNS+1
    //             -------------------
    //          0 |___________________|
    //          1 | |o|o|o|o|o|o|o|o| |
    //          . | |o|o|o|o|o|o|o|o| |
    //          . | |o|o|o|o|o|o|o|o| |
    //          . | |o|o|o|o|o|o|o|o| |
    // NUM_ROWS+1 | |               | |
    //             ------------------
    // The first and the last columns and the last row are separators
    // (Plasma::Label). The first row is another margin.

    Config *cfg = Config::getInstance();
    this->numRows    = cfg->getSettings(Config::NUM_ROWS).toInt();
    this->numColumns = cfg->getSettings(Config::NUM_COLUMNS).toInt();

    // Set the separators and the top margin
    Plasma::Label *s1 = new Plasma::Label(this);
    Plasma::Label *s2 = new Plasma::Label(this);
    Plasma::Label *s3 = new Plasma::Label(this);

    this->panelLayout->addItem(s1, 1, 0, numRows+1, 1);            // Left
    this->panelLayout->addItem(s2, 1, numColumns+1, numRows+1, 1); // Right
    this->panelLayout->addItem(s3, numRows+1, 1, 1, numColumns);   // Bottom
    this->panelLayout->setRowMinimumHeight(0, 0);                  // Top margin
    this->panelLayout->setRowMaximumHeight(0, 0);

    // Set the maximum grid size
    int itemSize = cfg->getSettings(Config::LAUNCHER_SIZE).toInt();

    for (int n=1; n<=this->numColumns; n++) {
        this->panelLayout->setColumnMaximumWidth(n, itemSize);
        this->panelLayout->setColumnMinimumWidth(n, itemSize);
    }

    for (int n=1; n<=this->numRows; n++) {
        this->panelLayout->setRowMaximumHeight(n, itemSize);
        this->panelLayout->setRowMinimumHeight(n, itemSize);
    }


    // Set the separation between launchers
    this->panelLayout->setSpacing(
        cfg->getSettings(Config::SEPARATION_BETWEEN_LAUNCHERS).toInt());

    this->setLayout(this->panelLayout);

    setFocusPolicy(Qt::StrongFocus);

    m_hoverIndicator = new Plasma::ItemBackground(this);
    m_hoverIndicator->setZValue(-100);
    m_hoverIndicator->hide();
}

Panel::~Panel()
{
    this->removeAllLaunchers();
}


// ************************************************************************** //
// **********                    PUBLIC METHODS                    ********** //
// ************************************************************************** //

void Panel::addLauncher(Takeoff::Launcher *launcher)
{
    if (this->isFull())
        return;

    connect(launcher, SIGNAL(clicked()), this, SIGNAL(clicked()));
    connect(launcher, SIGNAL(addedToFavorites()),
            this, SIGNAL(addedToFavorites()));
    connect(launcher, SIGNAL(removedFromFavorites()),
            this, SIGNAL(removedFromFavorites()));

    int row    = (this->launchers.length() / numColumns) + 1;
    int column = (this->launchers.length() % numColumns) + 1;

    this->panelLayout->addItem(launcher, row, column, Qt::AlignCenter);
    this->launchers.append(launcher);
}

void Panel::removeAllLaunchers()
{
    for (int n=0; n<this->launchers.length(); n++) {
        Takeoff::Launcher *aux = this->launchers.at(n);
        delete aux;
    }
    this->launchers.clear();
}

// ************************************************************************** //
// **********                      GET/SET/IS                      ********** //
// ************************************************************************** //

Takeoff::Launcher *Panel::getLauncher(int index) const
{
    if (this->launchers.length() == 0)
        return NULL;
    else
        return this->launchers.at(index);
}

bool Panel::isFull() const
{
    return this->launchers.length() >= this->numColumns * this->numRows;
}

void Panel::keyPressed(QKeyEvent *event)
{
    if (focused && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
    {
        launchers.at(rowFocused * numColumns + colFocused)->runApplication();
    }
    else
    {
        int panelCols = launchers.size() > numColumns ? numColumns : launchers.size();
        int panelRows = launchers.size() / numColumns + (launchers.size() % numColumns ? 1 : 0);
        if (focused)
        {
            switch (event->key())
            {
            case Qt::Key_Down:
                rowFocused = (rowFocused + 1) % panelRows;
                break;
            case Qt::Key_Up:
                rowFocused = (panelRows + rowFocused - 1) % panelRows;
                break;
            case Qt::Key_Left:
                colFocused = (panelCols + colFocused - 1) % panelCols;
                break;
            case Qt::Key_Right:
                colFocused = (colFocused + 1) % panelCols;
            }
        }
        else
        {
            focused = true;
            colFocused = 0;
            rowFocused = 0;
        }
        int idx = rowFocused * panelCols + colFocused;
        idx = idx >= launchers.size() ? launchers.size() - 1 : idx;
        m_hoverIndicator->setTargetItem(launchers.at(idx));
    }
}
