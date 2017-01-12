#!/bin/sh
# ---------------------------------------------------------------------------
# Multi-Phasic Applications: Trouble In Furry Land
#     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
#     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
# ---------------------------------------------------------------------------
# Trouble In Furry Land is under the GNU General Public License v3+, or later.
# See license.mkd for licensing and copyright information.
# ---------------------------------------------------------------------------
# DESCRIPTION: Builds and runs the game

# Force C locale
export LC_ALL=C

# Directory of this script
__exedir="$(dirname -- "$0")"

. ${__jlp:=/usr/share/java}
. ${__nlp:=/usr/lib/jni}
if ant -v -Dtifl.lwjglpath="$__jlp" -f "$__exedir/build.xml"
then
	LD_LIBRARY_PATH="$__nlp:$__nlp/.." java \
		-Djava.library.path="$__jlp:$__nlp" \
		-Dorg.lwjgl.librarypath="$__nlp" \
		-classpath "$__jlp/lwjgl.jar:tifl.jar" \
		net.multiphasicapps.tifl.lwjgl.Main
fi

