
if [ -d "./pzyctp" ] ; then
	rm -rf pzyctp
fi
if [ -d "/home/hecor/programs/lib/python2.7/site-packages/pzyctp" ] ; then
	rm -rf /home/hecor/programs/lib/python2.7/site-packages/pzyctp
fi

mkdir pzyctp
cp __init__.py pzyctp/

mkdir pzyctp/stock
cp stock/datafeeder.py pzyctp/stock/
cp stock/_datafeeder.so pzyctp/stock/
cp stock/trader.py pzyctp/stock/
cp stock/_trader.so pzyctp/stock/
cp stock/__init__.py pzyctp/stock/
cp stock/api/*.so pzyctp/stock/

mkdir pzyctp/future
cp future/datafeeder.py pzyctp/future/
cp future/_datafeeder.so pzyctp/future/
cp future/trader.py pzyctp/future/
cp future/_trader.so pzyctp/future/
cp future/__init__.py pzyctp/future/
cp future/api/*.so pzyctp/future/

mv pzyctp /home/hecor/programs/lib/python2.7/site-packages/
