#!/bin/sh
# ---------------------------------------------------------------------------
# Multi-Phasic Applications: Trouble In Furry Land
#     Copyright (C) Stephanie Gawroriski <xer@multiphasicapps.net>
#     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
# ---------------------------------------------------------------------------
# Trouble In Furry Land is under the GNU General Public License v3+, or later.
# See license.mkd for licensing and copyright information.
# ---------------------------------------------------------------------------
# DESCRIPTION: Checks whether the specified path is in the root of the
# project tree.

# Force C locale
export LC_ALL=C

# Directory of this script
__exedir="$(dirname -- "$0")"

# Which directory?
if [ "$#" -ge "1" ]
then
	__in="$1"
else
	__in="$(pwd)"
fi

# Stop at the root
__qq="$("$__exedir/absolute.sh" "$__in")"
while [ "$__qq" != "/" ]
do
	# Version marker?
	if [ -f "$__qq/tifl-version" ]
	then
		exit 0
	fi
	
	# Go down
	__qq="$(dirname -- "$__qq")"
done

# Not found
exit 1

