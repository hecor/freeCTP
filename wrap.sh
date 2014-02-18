swig -c++ -python pzyctp.swig
gcc -I/home/hecor/programs/include/python2.7 -fPIC -c pzyctp_wrap.cxx
gcc -I/home/hecor/programs/include/python2.7 -fPIC -c trader.cpp
gcc -I/home/hecor/programs/include/python2.7 -fPIC -c datafeeder.cpp
g++ -shared pzyctp_wrap.o trader.o datafeeder.o -o _pzyctp.so -lthosttraderapiSSE -lthostmduserapiSSE -llevel2userapi -L./api

