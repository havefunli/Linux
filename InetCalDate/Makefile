.PTHONY:all
all:server client

client:Client.cc
	g++ $^ -o $@ -std=c++14

server:Server.cc
	g++ $^ -o $@ -std=c++14

.PTHONY:clean
clean:
	rm server client
