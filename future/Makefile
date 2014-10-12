wrap: *.cpp *.h *.swig
	swig -c++ -python datafeeder.swig
	gcc -I/home/hecor/programs/include/python2.7 -fPIC -c datafeeder_wrap.cxx
	gcc -I/home/hecor/programs/include/python2.7 -fPIC -c datafeeder.cpp
	g++ -shared datafeeder_wrap.o datafeeder.o -o _datafeeder.so -lthostmduserapi -L./api

	swig -c++ -python trader.swig
	gcc -I/home/hecor/programs/include/python2.7 -fPIC -c trader_wrap.cxx
	gcc -I/home/hecor/programs/include/python2.7 -fPIC -c trader.cpp
	g++ -shared trader_wrap.o trader.o -o _trader.so -lthosttraderapi -L./api

clean:
	rm *.o *.so *.cxx datafeeder.py trader.py
