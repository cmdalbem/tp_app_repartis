CC = g++
FLAGS = -Wall -Wextra -std=c++11
LIBS = -ljson_linux-gcc-4.7_libmt 

# Seulement ajoutez ici le nom du nouveaux module qui tu a crée ;)
OBJS = File.o interface.o 

BIN = res

%.o: %.cpp %.h
		$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 

all: $(OBJS) $(OBJLIB)
		$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS) 
		#$(CC) $(FLAGS) $^ $(FLAGS) $(LIBS) 


clean:
		rm -f $(BIN)
		rm -f *.o
