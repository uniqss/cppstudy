cd tcps
hv_root=/home/will/study/libhv/libhv-1.2.1
g++ -g smain1.c -I$hv_root/include/hv/ -I/$hv_root/base/ -L$hv_root/lib/ -lhv -o tcps1 -lpthread
