#include "client_conn_factory.h"

#include "connection_s.h"

#include "p.h"

connection_t* client_conn_factory::create_connection(){
    _myprtf("\n");
    connection_t* conn = new connection_s();
    if (conn == NULL){
        return NULL;
    }

    set_connections.insert(conn);
    return conn;
}

void client_conn_factory::release_connection(connection_t* conn){
    _myprtf("\n");
    delete(conn);
    set_connections.erase(conn);
}
