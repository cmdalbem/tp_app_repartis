CC = g++

CFLAGS = -Wall 
LIBS = -ljson_linux-gcc-4.7_libmt 

# Seulement ajoutez ici le nome du nouveaux module qui tu a crée ;)
OBJS =  Server.o File.o

BIN = 

%.o: %.cpp %.h
		$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 


all: $(OBJS) $(OBJLIB)
#		$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS) 
		$(CC) $(FLAGS) $^ $(FLAGS) $(LIBS) 


clean:
		rm -f $(BIN)
		rm -f *.o