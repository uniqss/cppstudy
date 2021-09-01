

#netstat -anlp|grep 12345|grep ESTABLISHED|grep tcpc|awk '{print $7}'|cut -d/ -f1|xargs kill -INT
#netstat -anlp|grep 12345|grep LISTEN|awk '{print $7}'| cut -d/ -f1|xargs kill -INT


ps -ef|grep -v grep|grep 12345|grep tcpc1|awk '{print $2}'|xargs kill -INT
ps -ef|grep -v grep|grep 12345|grep tcps1|awk '{print $2}'|xargs kill -INT




