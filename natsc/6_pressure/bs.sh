g++ -g -std=c++11 -Wall -I ../include/ -lnats -lprotobuf-c -L ../lib/ -Wl,-rpath ../lib/ -o s subscriber.c
