.PHONY: all clean
all: mq_msgtest #mq_queuetest mq_exchangetest mq_test
mq_msgtest:mq_msgtest.cpp ../common/msg.pb.cc
	g++ -g -std=c++11 $^ -o $@ -I /usr/local/include -L /usr/local/lib  -lgtest -lprotobuf -lsqlite3
mq_queuetest:mq_queuetest.cpp
	g++ -std=c++11 mq_queuetest.cpp -o mq_queuetest -lgtest -lsqlite3 -lprotobuf
mq_exchangetest:mq_exchangetest.cpp
	g++  -g -std=c++11 $^ -o $@ -lgtest -lprotobuf -lsqlite3 -lpthread
mq_test:mq_filetest.cpp
	g++ -std=c++11 mq_filetest.cpp -o mq_test -lsqlite3 -lgtest -lprotobuf
clean:
	rm -fr mq_test mq_exchangetest mq_queuetest mq_msgtest