#!/bin/dash -x


waitforfiles="
/tmp/occs_extrainfo
/tmp/occs_extrainfo_semaphore
"

alldone=0

while expr "$alldone = 0" ; do
	for i in $waitforfiles ; do
		printf "%s" "test for >>$i<<"
		if exists $i; then
			printf "- exists\n"
			alldone=1
		else
			printf " - missing\n"
			alldone=0
		fi
	done
	echo "waiting ..."
	sleep 1;
done


