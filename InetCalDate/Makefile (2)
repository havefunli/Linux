.PTHONY:all
all:server client

client:Client.cc Date.cpp
	g++ $^ -o $@ -std=c++14 -ljsoncpp -lpthread

server:Server.cc Date.cpp
	g++ $^ -o $@ -std=c++14 -ljsoncpp -lpthread

.PTHONY:clean
clean:
	rm server client
