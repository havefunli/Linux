main: Main.cc InetDateCal/Date.cpp
	g++ $^ -ljsoncpp -o $@

.PHONY:client
client:Client.cc InetDateCal/Date.cpp
	g++ $^ -ljsoncpp -o $@

.PHONY:clean
clean:
	rm -rf main
