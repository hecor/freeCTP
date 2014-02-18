
if [ -d "./pzyctp" ] ; then
	rm -rf pzyctp
fi
if [ -d "/home/hecor/programs/lib/python2.7/site-packages/pzyctp" ] ; then
	rm -rf /home/hecor/programs/lib/python2.7/site-packages/pzyctp
fi

mkdir pzyctp
cp pzyctp.py pzyctp/
cp _pzyctp.so pzyctp/
cp __init__.py pzyctp/
cp ./api/*.so pzyctp/
mv pzyctp /home/hecor/programs/lib/python2.7/site-packages/
