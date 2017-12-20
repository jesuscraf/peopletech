#!/bin/ksh
#==============================================================================
# @(#) FileVersion: [M4_FILE_VERSION]
# @(#) FileDescription: This script configures application server.
# @(#) CompanyName: Meta4 Spain, S.A.
# @(#) LegalCopyright: (c) [M4_YEAR]
# @(#) ProductName:  PeopleNet
#=============================================================================

#-----------------------------------------------------------------------------
# Initialize variables.
#-----------------------------------------------------------------------------
export M4_SERVERSITE_ROOT=../
export ISETUP_PATH=${M4_SERVERSITE_ROOT}/setup:${M4_SERVERSITE_ROOT}/setup/debug
export ISETUP_LIB=${M4_SERVERSITE_ROOT}/m4server/lib:${M4_SERVERSITE_ROOT}/m4server/lib/debug
export ISETUP=isetup.pl
export ISETUP_INI=./isetup.ini

export PATH=${ISETUP_PATH}:${PATH}
export LD_LIBRARY_PATH=${ISETUP_LIB}:${LD_LIBRARY_PATH}
export LIBPATH=${LD_LIBRARY_PATH}

#---------------------------------------------------------------------------
# print usage
#---------------------------------------------------------------------------
Usage()
{
	programName=$1
 
	echo "Usage:"
	echo "  ${programName} -h      (show options)"
	echo "  ${programName} full-path/isetup.ini"

	exit
}

#---------------------------------------------------------------------------
# print FatalError
#---------------------------------------------------------------------------
FatalError()
{
	message=$1
 
	echo "ERROR:"
	echo "  ${message}"
	exit
}

#---------------------------------------------------------------------------
# function: EscapeVariable
# description: escaped a variable.
#---------------------------------------------------------------------------
EscapeVariable()
{
    ev_InVariable=$1
    ev_OutNameVariable=$2
    
    ev_EscapedVariable=`echo ${ev_InVariable} | sed -e 's/\//\\\\\//g'`
    export ${ev_OutNameVariable}=${ev_EscapedVariable}
}

#---------------------------------------------------------------------------
# Isetup
#---------------------------------------------------------------------------
Isetup()
{
	ISETUP_INI=$1
 
	# create the temporal isetup.ini
	TMPFILE=/tmp/isetup$$.ini
	
	# edit $HOME, 3001
	USER=`whoami`
	EscapeVariable $HOME esHOME
	esPORT=`ypcat services | grep -w ${USER} | grep -v _2 | awk '{print $2}'| sed -e "s/\/tcp//g"`
	if [ "${esPORT}" = "" ]; then
		FatalError "User ${USER} has no assign port. Contact with your system administrator"
	fi
	
	cat ${ISETUP_INI} | sed	-e "s/\$HOME/${esHOME}/g" \
							-e "s/^base_port.*/base_port = ${esPORT}/g" > ${TMPFILE}

	ISETUP_INI=${TMPFILE}
	
	# run the script.
	${ISETUP} --ex=${ISETUP_INI}
	
	# print the ports.
	echo
	echo "Your m4serversite has been configured. The assigned ports are:"

	integer iBasePort=${esPORT}
	echo "  $iBasePort: SNMP Port"
	iBasePort=$iBasePort+1
	echo "  $iBasePort: AppServer Client Connection Port"
	iBasePort=$iBasePort+1
	echo "  $iBasePort: AppServer Client SSL Connection Port"
	iBasePort=$iBasePort+1
	echo "  $iBasePort: AppServer Administrator Connection  Port"
	
	echo	
	echo "      If you are running the AppServer in the same machine that Dispatcher, "
	echo "      you should change the above ports in your startup.obl file. You only "
	echo "      can use the next available ports: xxx0/xxx7/xxx8/xxx9, where xxx are "
	echo "      the same digits assigned in the AppServer Internal Port"

	echo	
	iBasePort=$iBasePort+1
	echo "  $iBasePort: Controller Internal Port"
	iBasePort=$iBasePort+1
	echo "  $iBasePort: Controller Dispatcher Port"

	echo	
	iBasePort=${esPORT}
	echo "  $iBasePort: Dispatcher Internal Port"
	iBasePort=$iBasePort+1
	echo "  $iBasePort: Dispatcher Client Connection Port"
	iBasePort=$iBasePort+1
	echo "  $iBasePort: Dispatcher Client SSL Connection Port"
	iBasePort=$iBasePort+1
	echo "  $iBasePort: Dispatcher Administrator Connection Port"
		
	# delete the temporal file.
	rm -fr ${TMPFILE}
}

#-----------------------------------------------------------------------------
# check input arguments
#-----------------------------------------------------------------------------
export PROGRAMNAME=$0
if [ "$1" != "" ]; then
	export ISETUP_INI=$1
fi


case "$1" in
'-h' | '/h' | '-help' | '/help')
	Usage ${PROGRAMNAME}
	;;

*)
	Isetup ${ISETUP_INI}
	;;
esac
