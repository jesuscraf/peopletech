#!/bin/ksh
#==============================================================================
# @(#) FileVersion: [M4_FILE_VERSION]
# @(#) FileDescription: This script prints a file.
# @(#) CompanyName: Meta4 Spain, S.A.
# @(#) LegalCopyright: (c) [M4_YEAR]
# @(#) ProductName:  PeopleNet
#==============================================================================

# This file is used to print to server from the Meta4 report engine
# The requirement to use it is to have the lp command in the user path
# If you want to trace the result, check the /tmp/lp-USER directory, 
# where USER is the username.


PRINTER=$1
FILE=$2

unalias lp

M4LP_OUTPUT=/tmp/lp-${USER}
M4LP_EXE_NAME="lp -c -d ${PRINTER} -o nobanner ${FILE}"

#==============================================================================
IsExeInPath()
{
    exename=$1
    checked_path=`which $exename`
    if [ -x "${checked_path}" ]; then
        #-- echo "$exename found at $checked_path"
        return 1
    else
        #-- echo "$exename not in path..."
        return 0
    fi
}
#==============================================================================

Run()
{
echo "----------------------------------: " 	1>> ${M4LP_OUTPUT} 2>&1
echo "Running command: " 			1>> ${M4LP_OUTPUT} 2>&1
echo ${M4LP_EXE_NAME}    			1>> ${M4LP_OUTPUT} 2>&1
echo "from directory: `pwd`" 			1>> ${M4LP_OUTPUT} 2>&1
echo "----------------------------------: " 1>> ${M4LP_OUTPUT} 2>&1

IsExeInPath "lp"; IS_LP=$?
if  [ "${IS_LP}" != "0" ]; then
	${M4LP_EXE_NAME}   1>> ${M4LP_OUTPUT} 2>&1
else
         echo "You do not seem to have the lp command in your path"  1>> ${M4LP_OUTPUT} 2>&1
	 echo "Result of which lp is: `which lp`"  1>> ${M4LP_OUTPUT} 2>&1
fi

echo "----------------------------------: " 1>> ${M4LP_OUTPUT} 2>&1
}


#==============================================================================
Run
