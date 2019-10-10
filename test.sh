if test -z "$HOSTTYPE" ; then
	unset HOSTTYPE
	echo "LA VARIABLE HOSTTYPE EXIST DEJA, SUPPRESSION"
else
	HOSTTYPE=$(uname -m)_$(uname -s)
	echo "HOSTTYPE CREE"
fi
