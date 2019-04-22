#!/bin/bash
for ((j = 1 ; j <= 9 ; j++ ))
do
	for ((i = 1 ; i <= 9 ; i++ ))
	do 
		tot=`expr $i + $j`
		tmp=`expr $tot % 2`
		if [ $tmp -eq 0 ]; then
			echo -e -n "\033[47m "
		else
			echo -e -n "\033[40m "
		fi
	done
echo -e -n "\033[40m"
echo ""
done

