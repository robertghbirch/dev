CC=gcc

my_program: main.cpp p_wire_protocol.pb.cc utils.cpp
	time gcc main.cpp p_wire_protocol.pb.cc utils.cpp -o my_program -pthread -I/usr/local/include -pthread -L/usr/local/lib -lprotoc -lprotobuf -lpthread -std=c++11 -lstdc++
