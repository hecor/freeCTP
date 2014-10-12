
if [ -d "./pzyctp" ] ; then
	rm -rf pzyctp
fi
if [ -d "/home/hecor/programs/lib/python2.7/site-packages/pzyctp" ] ; then
	rm -rf /home/hecor/programs/lib/python2.7/site-packages/pzyctp
fi

mkdir pzyctp
cp __init__.py pzyctp/

LIST="stock future LTS"
for d in $LIST
do
	mkdir pzyctp/$d
	cp $d/datafeeder.py 	pzyctp/$d/
	cp $d/_datafeeder.so 	pzyctp/$d/
	cp $d/trader.py 		pzyctp/$d/
	cp $d/_trader.so 		pzyctp/$d/
	cp $d/__init__.py 		pzyctp/$d/
	cp $d/api/*.so 			pzyctp/$d/
done

mv pzyctp /home/hecor/programs/lib/python2.7/site-packages/
