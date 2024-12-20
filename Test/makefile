.PHONY: all clean
all:mq_queuetest mq_exchangetest mq_test

mq_queuetest:mq_queuetest.cpp
	g++ -std=c++11 mq_queuetest.cpp -o mq_queuetest -lgtest -lsqlite3
mq_exchangetest:mq_exchangetest.cpp
	g++  -g -std=c++11 $^ -o $@ -lgtest -lprotobuf -lsqlite3
mq_test:mq_filetest.cpp
	g++ -std=c++11 mq_filetest.cpp -o mq_test -lsqlite3
clean:
	rm -fr mq_test mq_exchangetest mq_queuetest