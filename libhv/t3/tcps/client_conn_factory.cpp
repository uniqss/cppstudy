#include "client_conn_factory.h"

#include "client_connection.h"

connection_t* client_conn_factory::create_connection(){
    connection_t* conn = new client_connection();
    if (conn == NULL){
        return NULL;
    }

    set_connections.insert(conn);
    return conn;
}

void client_conn_factory::release_connection(connection_t* conn){
    delete(conn);
    set_connections.erase(conn);
}
