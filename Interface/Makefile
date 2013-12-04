CC = g++
FLAGS = -Wall -Wextra -I ../File -I ../Server -I../Client
LIBS = 

OBJS = interface.o ../File/File.o ../File/FilesManager.o ../Server/Server.o ../Server/Connector.o ../Client/Client.o

BIN = res

%.o: %.cpp %.h
		$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 

all: $(OBJS) $(OBJLIB)
		$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS) 
		#$(CC) $(FLAGS) $^ $(FLAGS) $(LIBS) 


clean:
		rm -f $(BIN)
		rm -f *.o
