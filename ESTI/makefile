CC	= g++
CPPFLAGS= -Wno-deprecated -std=c++11 -O3 -m64 -c -w #-Wall
LDFLAGS	= -O3 -m64 
SOURCES	= ESTI.cpp generate_DAG.cpp accelerateFELINE.cpp query.cpp
OBJECTS	= $(SOURCES:.cpp=.o)
EXECUTABLE=ESTI

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $@.o -o $@

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

clear:
	rm -f *.o
