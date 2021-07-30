CXX      = $(shell fltk-config --cxx)
DEBUG = -g -Wall -O0 -time
CXXFLAGS = $(shell fltk-config --use-gl --use-images --cxxflags ) -I.
LDFLAGS  = $(shell fltk-config --use-gl --use-images --ldflags )
LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags )
LINK     = $(CXX)

main.exe: main.o
	$(CXX) $^ -o $@ $(LDSTATIC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $<

run: main
	./main

clean:
	rm -f *.o *~ deps.mk main

deps.mk: $(wildcard *.cpp) Makefile
	$(CXX) -MM $(wildcard *.cpp) > $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif
