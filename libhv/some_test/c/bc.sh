
hv_root=/home/uniqs/projects/libhv/libhv/build
g++ -Wall -g -std=c++11 cmain1.cpp connection_c.cpp net.cpp -I$hv_root/include/hv/ -I/$hv_root/base/ -L$hv_root/lib/ -lhv -o ./tcpc1 -lpthread -Wl,-rpath=$hv_root/lib/
