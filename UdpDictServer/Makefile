.PTHONY:all
all:server client

client:Clientmain.cc
	g++ $^ -o $@ -std=c++14

server:Servermain.cc
	g++ $^ -o $@ -std=c++14

.PTHONY:clean
clean:
	rm server client
