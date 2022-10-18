cd tcpc
hv_root=/home/will/study/libhv/libhv-1.2.1
g++ -g -std=c++11 cmain1.cpp connection_c.cpp ../../net/net.cpp -I../../net/ -I$hv_root/include/hv/ -I/$hv_root/base/ -L$hv_root/lib/ -lhv -o ../tcpc1 -lpthread
