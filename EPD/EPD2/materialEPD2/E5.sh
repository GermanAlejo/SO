
#
#!/bin/bash
# Script to test if..elif...else
#
osch=0
echo "Te gustan mas las clases de:"
echo "1. Practica"
echo "2. Teoria"
echo -n "elige [1 or 2]? "
read osch

if [ $osch -eq 1 ]; then
	echo "te gusta la practica"
elif [ $osch -eq 2 ]
then
	echo "te gusta la teoria"
else
	echo "no te gusta nada"
fi
