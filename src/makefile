
pingtx: config main.o icmptx.o getip.o
	g++ -o ./out/pingtx ./out/main.o ./out/icmptx.o ./out/getip.o
	rm ./out/main.o ./out/icmptx.o ./out/getip.o
	
config:
	mkdir -p ./out
	echo -e "IP:192.168.1.1\n" > ./out/config

main.o: main.cpp
	g++ -o ./out/main.o -c main.cpp 

icmptx.o: icmptx.cpp
	g++ -o ./out/icmptx.o -c icmptx.cpp

getip.o: getip.c
	g++ -o ./out/getip.o -c getip.c
