CXX = g++
CXXFLAGS = -std=c++11 -I/usr/local/include -pthread
LDFLAGS = -L/usr/local/lib -lgrpc++ -lgrpc -lgpr -labsl_synchronization -labsl_time -labsl_strings -labsl_base -lprotobuf -lpthread -ldl

all: prime_client

prime_client: prime_client.o prime.pb.o prime.grpc.pb.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o prime_client

.PHONY: all clean
