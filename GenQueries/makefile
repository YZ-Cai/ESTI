CC	= g++
CPPFLAGS= -Wno-deprecated -std=c++11 -O3 -m64 -c -w #-Wall
LDFLAGS	= -O3 -m64
SOURCES	= generate_queries.cpp
OBJECTS	= $(SOURCES:.cpp=.o)
EXECUTABLE=generate_queries

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $@.o -o $@

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

clear:
	-rm -f *.o
