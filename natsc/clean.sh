for D in `find . -type d -maxdepth 1`
do
	echo $D
	rm -f $D/s
	rm -f $D/p
done

