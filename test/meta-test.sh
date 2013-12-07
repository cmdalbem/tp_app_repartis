if [ $# -ne 1 ] ; then
	echo "I need one argument"
	exit
fi

touch tmp.sh 
touch entryTmp 
chmod 777 tmp.sh
even="True"
for word in $(cat $1); do
	if [ $even = "True" ]
	then
		even="False"
		cat "machine/$word" > entryTmp
	else 
		even="True"
		echo "./../Interface/res < entryTmp" > tmp.sh
		gnome-terminal -x ./tmp.sh
		sleep 1
	fi
done

