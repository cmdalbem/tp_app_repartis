CC = g++

CFLAGS = -Wall
LIBS =

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