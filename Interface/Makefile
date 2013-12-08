CC = g++
FLAGS = -Wall -Wextra -I ../File -I ../Server -I../Sender -I . -fpermissive -g
LIBS = -lpthread -ljson_linux-gcc-4.7_libmt

OBJS = interface.o ../File/File.o ../File/FilesManager.o ../Sender/Sender.o

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
