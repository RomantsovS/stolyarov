CXX = g++
CXXFLAGS = -g -Wall -O0 -time


chat: sockets.o chat.o main.o
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

run: chat
	./chat

clean:
	rm -f *.o *~ deps.mk chat

deps.mk: $(wildcard *.cpp) Makefile
	$(CXX) -MM $(wildcard *.cpp) > $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif
