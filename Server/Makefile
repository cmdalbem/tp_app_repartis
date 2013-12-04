CC = g++
FLAGS = -Wall -Wextra -I ../File
LIBS = 

# Seulement ajoutez ici le nom du nouveaux module qui tu a crée ;)
OBJS = BasicServer.o Server.o Connector.o ../File/pubsub.o ../File/FilesManager.o ../File/File.o

BIN = res

%.o: %.cpp %.h
		$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 

all: $(OBJS) $(OBJLIB)
		$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS) 
		#$(CC) $(FLAGS) $^ $(FLAGS) $(LIBS) 


clean:
		rm -f $(BIN)
		rm -f *.o
