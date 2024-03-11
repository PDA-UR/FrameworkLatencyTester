all:
	mkdir bin
	g++ main.cpp -lGL -lX11 -lXext -lJetsonGPIO -lpthread -o bin/latency_tester
	g++ click_tester.cpp -lJetsonGPIO -lpthread -o bin/click_tester
	g++ click_tester_2.cpp -lJetsonGPIO -lpthread -o bin/click_tester_2
	g++ click_tester_3.cpp -lJetsonGPIO -lpthread -o bin/click_tester_3

debug:
	mkdir debug
	g++ -g main.cpp -lGL -lX11 -lXext -lJetsonGPIO -lpthread -o debug/latency_tester

clean:
	rm -rf bin
	rm -rf debug
