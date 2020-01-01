#ifndef JMESH_PRINT_H
#define JMESH_PRINT_H



#include"stdint.h"
#include <stdarg.h>
#include"jmesh_config.h"
extern char print_log_flag;
int _print(const char* fmt, ...);
int _print_buffer(int len,char* data,const char* fmt, ...);
int _print_cmd(const char* fmt, ...);
int _print_buffer_cmd(int len,char* data,const char* fmt, ...);
int _gatt_send_cmd(unsigned char id,const char* fmt, ...);
int _gatt_send_buffer_cmd(unsigned char id,int len,char* data,const char* fmt, ...);
#if(JMESH_ROUTE_PRINTABLE==1)
    #define print_route(fmt, ...) _print(fmt, ##__VA_ARGS__)
    #define print_buffer_route(len,buffer,fmt,...) _print_buffer(len,(char*)buffer,fmt, ##__VA_ARGS__)
#else
    #define print_route(fmt, ...)
    #define print_buffer_route(len,buffer,fmt,...)
#endif
#if(JMESH_CMD_PRINTABLE==1)
    #define print_cmd(fmt, ...) _print_cmd(fmt, ##__VA_ARGS__)
    #define print_buffer_cmd(len,buffer,fmt,...) _print_buffer_cmd(len,(char*)buffer,fmt, ##__VA_ARGS__)
#else
    #define print_cmd(fmt, ...)
    #define print_buffer_cmd(len,buffer,fmt,...)
#endif

#if(JMESH_INFO_PRINTABLE==1)
    #define print_info(fmt, ...) _print(fmt, ##__VA_ARGS__)
    #define print_buffer_info(len,buffer,fmt,...) _print_buffer(len,(char*)buffer,fmt, ##__VA_ARGS__)
#else
    #define print_info(fmt, ...)
    #define print_buffer_info(len,buffer,fmt,...)
#endif

#if(JMESH_NOTE_PRINTABLE==1)
    #define print_note(fmt, ...)  _print("(note)"fmt"", ##__VA_ARGS__)
    #define print_buffer_note(len,buffer,fmt,...) _print_buffer(len,(char*)buffer,"(note)"fmt"", ##__VA_ARGS__)
#else
    #define print_note(fmt, ...)
    #define print_buffer_note(len,buffer,fmt,...)
#endif

#if(JMESH_WARNING_PRINTABLE==1)
    #define print_warning(fmt, ...)  _print("<warning>"fmt"", ##__VA_ARGS__)
    #define print_buffer_warning(len,buffer,fmt,...) _print_buffer(len,(char*)buffer,"<warning>"fmt"", ##__VA_ARGS__)
#else
    #define print_warning(fmt, ...)
    #define print_buffer_warning(len,buffer,fmt,...)
#endif

#if(JMESH_ERROR_PRINTABLE==1)
    #define print_error(fmt, ...)  _print_cmd("[error]file:"__FILE__",line:%d,"fmt"", __LINE__,##__VA_ARGS__)
    #define print_buffer_error(len,buffer,fmt,...) _print_buffer_cmd(len,(char*)buffer,"[error]file:"__FILE__",line:%d,"fmt"", __LINE__, ##__VA_ARGS__)
#else
    #define print_error(fmt, ...)
    #define print_buffer_error(len,buffer,fmt,...)
#endif

#define JMESH_CMD_RES(interface,fmt,...)        \
    do{                                         \
        if(JMESH_CMD_INTERFACE_UART==interface) \
        {                                       \
            _print_cmd(fmt,##__VA_ARGS__);      \
        }                                       \
        else                                    \
        {                                       \
            _gatt_send_cmd(interface,fmt,##__VA_ARGS__); \
        }                                       \
    }while(0)


#define JMESH_CMD_RES_BUFFER(interface,len,buffer,fmt,...) \
    do{                                         \
        if(JMESH_CMD_INTERFACE_UART==interface) \
        {                                       \
            _print_buffer_cmd(len,(char*)buffer,fmt,##__VA_ARGS__);      \
        }                                       \
        else                                    \
        {                                       \
            _gatt_send_buffer_cmd(interface,len,(char*)buffer,fmt,##__VA_ARGS__); \
        }                                       \
    }while(0)



#if(JMESH_LOG_LVL >= JMESH_LOG_LVL_ERROR)
    #define JMESH_LOGE(code,tag,fmt,...) _print("<e%d.%s>"fmt,code,tag,##__VA_ARGS__)
    #define JMESH_LOGE_BUFFER(code,tag,length,buffer,fmt,...) _print_buffer(length,(char*)buffer,"<e%d.%s]>"fmt,code,tag,##__VA_ARGS__)
#else
    #define JMESH_LOGE(code,tag,fmt,...)
    #define JMESH_LOGE_BUFFER(code,tag,length,buffer,fmt,...)
#endif

#if(JMESH_LOG_LVL >=JMESH_LOG_LVL_WARN)
    #define JMESH_LOGW(code,tag,fmt,...) _print("<w%d.%s>"fmt,code,tag,##__VA_ARGS__)
    #define JMESH_LOGW_BUFFER(code,tag,length,buffer,fmt,...) _print_buffer(length,(char*)buffer,"<w%d.%s>"fmt,code,tag,##__VA_ARGS__)
#else
    #define JMESH_LOGW(code,tag,fmt,...)
    #define JMESH_LOGW_BUFFER(code,tag,length,buffer,fmt,...)
#endif


#if(JMESH_LOG_LVL >=JMESH_LOG_LVL_NOTE)
    #define JMESH_LOGN(tag,fmt,...) _print("<n.%s>"fmt,tag,##__VA_ARGS__)
    #define JMESH_LOGN_BUFFER(tag,length,buffer,fmt,...) _print_buffer(length,(char*)buffer,"<n.%s>"fmt,tag,##__VA_ARGS__)
#else
    #define JMESH_LOGN(tag,fmt,...)
    #define JMESH_LOGN_BUFFER(tag,length,buffer,fmt,...)
#endif

#if(JMESH_LOG_LVL >=JMESH_LOG_LVL_INFO)
    #define JMESH_LOGI(tag,fmt,...) _print("<i.%s>"fmt,tag,##__VA_ARGS__)
    #define JMESH_LOGI_BUFFER(tag,length,buffer,fmt,...) _print_buffer(length,(char*)buffer,"<i.%s>"fmt,tag,##__VA_ARGS__)
#else
    #define JMESH_LOGI(tag,fmt,...)
    #define JMESH_LOGI_BUFFER(tag,length,buffer,fmt,...)
#endif

#if(JMESH_LOG_LVL >=JMESH_LOG_LVL_DEBUG)
    #define JMESH_LOGD(tag,fmt,...) _print("<d.%s>"fmt,tag,##__VA_ARGS__)
    #define JMESH_LOGD_BUFFER(tag,length,buffer,fmt,...) _print_buffer(length,(char*)buffer,"<d.%s>"fmt,tag,##__VA_ARGS__)
#else
    #define JMESH_LOGD(tag,fmt,...)
    #define JMESH_LOGD_BUFFER(tag,length,buffer,fmt,...)
#endif


#endif // JMESH_PRINT_H
