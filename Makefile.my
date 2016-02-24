CC=g++
OPT=-c -ggdb
LIB=-lboost_system -lboost_filesystem -lboost_thread -lpthread

all: main.o request_parser.o request_handler.o reply.o
	$(CC) main.o request_parser.o request_handler.o reply.o -o http-server.exe $(LIB)

main.o: main.cpp http_server.h connection.h program_options.h app_manager.h
	$(CC) $(OPT) main.cpp
	
request_parser.o: request_parser.cpp request_parser.h
	$(CC) $(OPT) request_parser.cpp
	
request_handler.o: request_handler.cpp request_handler.h mime_types.h
	$(CC) $(OPT) request_handler.cpp
	
reply.o: reply.cpp reply.h
	$(CC) $(OPT) reply.cpp

clean:
	rm -rf *~ *.exe *.o
