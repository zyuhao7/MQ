all: dict_client dict_server
dict_client:dict_client.cc
	g++ -std=c++11 $^ -o $@ -I../include -L../lib -L /usr/local/lib -lmuduo_net -lmuduo_base -pthread
dict_server:dict_server.cc
	g++ -std=c++11 $^ -o $@ -I../include -L../lib  -L /usr/local/lib -lmuduo_net -lmuduo_base -pthread