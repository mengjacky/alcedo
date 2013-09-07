#ifndef __ALCEDO_H__
#define __ALCEDO_H__
#include <vector>
#include "connection.h"

#define NET_NOT_CONNETED 101
#define NET_NOT_CONNETED 102
#define EVENT_PROCESS_ERROR 1001
#define EVENT_PROCESS_SUCCESS 1002
#define NOT_MATCH_SERVER 1100
typedef std::vector<Connection*> ConnList;

//1. short 2. long for pingpong 3. long for continuity
enum ConnType{ kSortConn, kLongConn, kLongConnContinuity}
#endif //__ALCEDO_ERROR_H__
