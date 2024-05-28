all:
	mkdir bin
	g++ main.cpp -lGL -lX11 -lXext -lXdamage -lwiringPi -lpthread -o bin/latency_tester
	#g++ click_tester.cpp -lwiringPi -lpthread -o bin/click_tester
	#g++ click_tester_2.cpp -lwiringPi -lpthread -o bin/click_tester_2
	#g++ click_tester_3.cpp -lwiringPi -lpthread -o bin/click_tester_3

debug:
	mkdir debug
	g++ -g main.cpp -lGL -lX11 -lXext -lXdamage -lwiringPi -lpthread -o debug/latency_tester

clean:
	rm -rf bin
	rm -rf debug
