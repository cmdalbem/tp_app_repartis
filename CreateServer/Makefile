CC = g++
FLAGS = -Wall -Wextra -I ../File -I ../Server -I../Sender -fpermissive -g
LIBS = -lpthread -ljson_linux-gcc-4.7_libmt

OBJS = createServer.o ../File/File.o ../File/FilesManager.o ../Server/Server.o ../Server/Connector.o ../Server/Receiver.o ../Sender/Sender.o

BIN = res

%.o: %.cpp %.h
		$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 

all: $(OBJS) $(OBJLIB)
		$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS) 
		#$(CC) $(FLAGS) $^ $(FLAGS) $(LIBS) 


clean:
		rm -f $(BIN)
		rm -f $(OBJS)
		rm -f *.o
