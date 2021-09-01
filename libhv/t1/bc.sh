cd tcpc
hv_root=/home/will/study/libhv/libhv-1.2.1
g++ -g cmain1.c -I$hv_root/include/hv/ -I/$hv_root/base/ -L$hv_root/lib/ -lhv -lpthread -o tcpc1
