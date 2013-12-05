CC = g++
FLAGS = -Wall -Wextra -I ../File
LIBS = -lpthread

# Seulement ajoutez ici le nom du nouveaux module qui tu a crée ;)
OBJS = Client.o ../File/pubsub.o

BIN = res

%.o: %.cpp %.h
		$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 

all: $(OBJS) $(OBJLIB)
#		$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS) 
		#$(CC) $(FLAGS) $^ $(FLAGS) $(LIBS) 


clean:
		rm -f $(BIN)
		rm -f *.o
