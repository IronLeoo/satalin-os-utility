#!/bin/bash

sed -i 's/#xrandr --dpi 200\/eDP1\n#export BARHEIGHT=39\n#export ICONSIZE=32\n#export GDK_SCALE=2\n#export QT_AUTO_SCREEN_SCALE_FACTOR=1\n#export QT_FONT_DPI=192\n\nexport BARHEIGHT=21\nexport ICONSIZE=18/xrandr --dpi 200\/eDP1\nexport BARHEIGHT=39\nexport ICONSIZE=32\nexport GDK_SCALE=2\nexport QT_AUTO_SCREEN_SCALE_FACTOR=1\nexport QT_FONT_DPI=192\n\n#export BARHEIGHT=21\n#export ICONSIZE=18/' $1