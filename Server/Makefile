CC = g++
FLAGS = -Wall -Wextra -I ../File
LIBS = -lpthread
INC = -I./ -I../Client/

# Seulement ajoutez ici le nom du nouveaux module qui tu a crée ;)
OBJS = BasicServer.o Server.o Connector.o ../File/pubsub.o ../File/FilesManager.o ../File/File.o ../Client/Client.o

BIN = res

%.o: %.cpp %.h
		$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) $(INC)

all: $(OBJS) $(OBJLIB)
		#$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS) 
		$(CC) $(FLAGS) $^ $(FLAGS) $(LIBS) 


clean:
		rm -f $(BIN)
		rm -f *.o
