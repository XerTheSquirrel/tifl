#!/bin/sh
# ---------------------------------------------------------------------------
# Multi-Phasic Applications: Trouble In Furry Land
#     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
#     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
# ---------------------------------------------------------------------------
# Trouble In Furry Land is under the GNU General Public License v3+, or later.
# See license.mkd for licensing and copyright information.
# ---------------------------------------------------------------------------
# DESCRIPTION: Makes windows binary

# Force C locale
export LC_ALL=C

# Directory of this script
__exedir="$(dirname -- "$0")"

# Go to source location
cd "$__exedir/.."

# Clean up local
make clean
make clean EXESUFFIX=.exe

# Compile for Windows
__sdldir="/usr/local/cross-tools/i686-w64-mingw32/"
if make CC=i686-w64-mingw32-gcc SDL_CONFIG="$__sdldir/bin/sdl2-config" EXESUFFIX=.exe
then
	cp "$__sdldir/bin/"*.dll .
	echo "yay"
	
	rm tifl-win32.zip
	zip tifl-win32.zip *.dll tifl.exe readme.mkd license.mkd
fi


