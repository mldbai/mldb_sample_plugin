default: plugin.so

CXX := g++ -fPIC -std=c++0x -O2 -I /opt/include

plugin.o:	plugin.cc
	$(CXX) -c -o plugin.o $<

plugin.so:	plugin.o
	$(CXX) -shared -o $@ $<

