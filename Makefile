CLIENT = warmup_cli
SERVER = warmup_svr

all: $(CLIENT) $(SERVER)

$(CLIENT): $(CLIENT).cpp
	g++ -g -std=c++0x -Wall -Werror -o $@ $^

$(SERVER): $(SERVER).cpp
	g++ -g -std=c++0x -Wall -Werror -o $@ $^

clean:
	rm -f *.o $(CLIENT) $(SERVER)
