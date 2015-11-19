a=2
while [ $a -lt 100 ]
do
	d=0
	while [ $d -lt 100 ]
	do
		b=$(./virtmem 100 $a $1 $2)
		OLDIFS="$IFS"
		IFS=';'
		c=0
		for num in $b; 
		do 
			e[$c]=$((${e[$c]}+$num))
			c=`expr $c + 1`	
		done
		IFS="$OLDIFS"
		d=`expr $d + 1`
	done
	printf '%s;' "${e[@]}"
	printf '\n'
	e[0]=0
	e[1]=0
	a=`expr $a + 1`
done
