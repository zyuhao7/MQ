.PHONY: server
CFLAG = -I/usr/local/include -I../third/include
LFLAG = -L/usr/local/lib  -L../third/lib  -lgtest -pthread -lmuduo_net -lmuduo_base -lz -lsqlite3 -lprotobuf
server:server.cc  ../common/msg.pb.cc ../common/proto.pb.cc  ../third/include/muduo/proto/codec.cc
	g++ -g -std=c++11 $(CFLAG) $^ -o $@  $(LFLAG)