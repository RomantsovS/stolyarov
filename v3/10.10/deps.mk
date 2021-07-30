sockets.o: sockets.cpp sockets.hpp
chat.o: chat.cpp chat.hpp sockets.hpp
main.o: main.cpp chat.hpp sockets.hpp
