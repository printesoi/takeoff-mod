#!/bin/bash
# Usage $ _run.sh SuperUserPass

if [ ! $# -eq 1 ] ; then
    echo The script must receive the super user password
    exit
fi

cd build
echo $1 | sudo -S make install
kbuildsycoca4
plasmoidviewer takeoff
 
