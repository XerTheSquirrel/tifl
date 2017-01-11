#!/bin/sh
# ---------------------------------------------------------------------------
# Multi-Phasic Applications: Trouble In Furry Land
#     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
#     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
# ---------------------------------------------------------------------------
# Trouble In Furry Land is under the GNU General Public License v3+, or later.
# See license.mkd for licensing and copyright information.
# ---------------------------------------------------------------------------
# DESCRIPTION: Determines the Java name for a path

# Force C locale
export LC_ALL=C

# Directory of this script
__exedir="$(dirname -- "$0")"

# Check
if [ "$#" -le "0" ]
then
	echo "Usage: $0 (Path)" 1>&2
	exit 1
fi

# Starting point
__sdir="$(dirname -- "$1")"
__sfil="$(basename -- "$1" .java)"

# Scan the directory tree
__at="$("$__exedir/absolute.sh" "$__sdir/$__sfil")"
__rv=""
while true
do
	# If not in the root, stop
	if ! "$__exedir/isinroot.sh" "$__at"
	then
		echo "unknown/package"
		exit 0
	fi
	
	# Get base and directory for this
	__dir="$(dirname -- "$__at")"
	__fil="$(basename -- "$__at")"
	
	# Build name
	if [ "$__rv" = "" ]
	then
		__rv="$__fil"
	else
		__rv="$__fil/$__rv"
	fi
	
	# If the GPG is here, stop
	if [ -f "$__dir/steven-gawroriski.gpg" ] || \
		[ -f "$__dir/../steven-gawroriski.gpg" ]
	then
		break
	fi
	
	# Go up
	__at="$__dir"
done

# Return result
echo "$__rv"

