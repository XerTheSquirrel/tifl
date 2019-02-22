#!/bin/sh
# ---------------------------------------------------------------------------
# Multi-Phasic Applications: Trouble In Furry Land
#     Copyright (C) Stephanie Gawroriski <xer@multiphasicapps.net>
#     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
# ---------------------------------------------------------------------------
# Trouble In Furry Land is under the GNU General Public License v3+, or later.
# See license.mkd for licensing and copyright information.
# ---------------------------------------------------------------------------
# DESCRIPTION: Creates a file and potentially adds it to the repository with
# a given template.

# Force C locale
export LC_ALL=C

# Directory of this script
__exedir="$(dirname -- "$0")"

# Help
print_help()
{
	echo "Usage: $0 [-b] [-x] [-n] [-c command] [files...]"
	echo ""
	echo " -b Do not use a template."
	echo " -x Make the files executable."
	echo " -n Do not add files to the fossil repository."
	echo " -c Run the specified command for each file."
}

# Handle arguments
__cmd=""
__makex="0"
__noadd="0"
__notemplate="0"
while getopts "bnxc:" FOO
do
	case $FOO in
		c)
			__cmd="$OPTARG"
			;;
		
		x)
			__makex="1"
			;;
		
		b)
			__notemplate="1"
			;;
		
		n)
			__noadd="1"
			;;
		
		*)
			print_help
			exit 1
			;;
	esac 
done

# Down they go
shift $(($OPTIND - 1))

# No files specified?
if [ "$#" -eq "0" ]
then
	print_help()
	exit 1
fi

# The current date
__nowyear="$(date +%Y)"
__nowmont="$(date +%m)"
__nowdayy="$(date +%d)"

__repZZZSDATEZZZ="$__nowyear$__nowmont$__nowdayy"
__repZZZSINCEZZZ="$__nowyear\/$__nowmont\/$__nowdayy"

# Go through all arguments
__files=""
while [ "$#" -gt "0" ]
do
	# Shift off
	__orig="$1"
	shift 1
	
	# Get some details
	__abs="$("$__exedir/absolute.sh" "$__orig")"
	__idir="$(dirname -- "$__abs")"
	__file="$(basename -- "$__abs")"
	
	# Determine extension, if any
	if echo "$__file" | grep '\.' > /dev/null
	then
		__fext="$(echo "$__file" | sed 's/.*\.\([^.]*\)$/\1/')"
	else
		__fext=""
	fi
	
	# Base name
	__fbase="$(basename -- "$__file" ".$__fext")"
	
	# Determine the template file to be used, if any
	__template="NONE"
	for __trytemplate in "$__exedir/templates/"*
	do
		# Check if not using any template
		__willdo="NONE"
		if [ "$__notemplate" -ne "0" ]
		then
			break
		fi
		
		# The formula determines how a template is matched
		__formula="$(basename "$__trytemplate")"
		__formlen="$(echo "$__formula" | wc -c)"
		
		# Get the first and last character, which uses basic globbing
		__charfst="$(echo "$__formula" | cut -c 1)"
		__charlst="$(echo "$__formula" | cut -c $(expr $__formlen - 1))"
		
		# File ends with the given string
		if [ "$__charfst" = "_" ]
		then
			__trunc="$(echo "$__formula" | cut -c 2-)"
			
			if echo "$__file" | \
				grep "$($__exedir/escapegrep.sh "$__trunc")"'$' > /dev/null
			then
				__willdo="$__formula"
			fi
			:
		
		# File starts with the given string
		elif [ "$__charlst" = "_" ]
		then
			__trunc="$(echo "$__formula" | cut -c -$(expr $__formlen - 2))"
			
			if echo "$__file" | \
				grep '^'"$($__exedir/escapegrep.sh "$__trunc")" > /dev/null
			then
				__willdo="$__formula"
			fi
			:
		
		# File name exactly matches
		else
			if [ "$__file" = "$__formula" ]
			then
				__willdo="$__formula"
			fi
		fi
		
		# Found template use it
		if [ "$__willdo" != "NONE" ]
		then
			# If the template is none, use it
			if [ "$__template" = "NONE" ]
			then
				__template="$__willdo"
				continue
			fi
			
			# Otherwise use an exact over a wildcard
			__tempfst="$(echo "$__template" | cut -c 1)"
			__templst="$(echo "$__template" | cut -c $(expr $__formlen - 1))"
			if [ "$__charfst" != "_" ] && [ "$__charlst" != "_" ] && \
				([ "$__tempfst" = "_" ] || [ "$__templst" = "_" ])
			then
				__template="$__willdo"
				continue
			fi
		fi
	done
	
	# Not in the root?
	if ! "$__exedir/isinroot.sh" "$__abs"
	then
		echo "Not creating file outside of root."
		continue
	fi
	
	# Add to all files
	__files="$__files $__orig"
	
	# Make target directories
	mkdir -p "$__idir"
	
	# Calculate package and class name
	__javaname="$("$__exedir/javaname.sh" "$__abs")"
	__repZZZCLASSZZZ="$(basename -- "$__javaname")"
	__repZZZPACKAGEZZZ="$(dirname -- "$__javaname" | sed 's/\//./g')"
	
	# Use template and modify parts of it
	cat "$__exedir/templates/$__template" \
			| sed 's/ZZZCLASSZZZ/'"$__repZZZCLASSZZZ"'/g' \
			| sed 's/ZZZPACKAGEZZZ/'"$__repZZZPACKAGEZZZ"'/g' \
			| sed 's/ZZZSINCEZZZ/'"$__repZZZSINCEZZZ"'/g' \
			| sed 's/ZZZSDATEZZZ/'"$__repZZZSDATEZZZ"'/g' \
		> "$__abs"
	
	# Make it executable
	if [ "$__makex" -ne "0" ]
	then
		chmod +x "$__abs"
	fi

	# Add to repo
	if [ "$__noadd" -eq "0" ]
	then
		fossil add "$__abs"
	fi
done

# Run command on it
if [ -n "$__cmd" ] && [ -n "$__files" ]
then
	"$__cmd" $__files
fi

