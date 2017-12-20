#! /bin/sh
case `uname -s` in
	'AIX') echo aix
		     ;;
    *)	echo solaris
			;;
esac

