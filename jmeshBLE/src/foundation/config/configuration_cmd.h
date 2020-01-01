#ifndef CONFIGURATION_CMD_H
#define CONFIGURATION_CMD_H
#include"string.h"
#include"stdlib.h"
#include "../../jmesh/jmesh_save_define.h"
#include "../../jmesh/jmesh_config.h"
#include "../../jmesh/jmesh_save.h"

#define ERROR_PARSE_FAILURE 0x10
#define ERROR_PARA_INVALID 0x11
#define ERROR_PARA1_INVALID 0x12
#define ERROR_PARA2_INVALID 0x13
#define ERROR_PARA3_INVALID 0x14
#define ERROR_PARA4_INVALID 0x15
#define ERROR_EXC_FAILURE 0x16

extern unsigned char cur_cmd_interface;

#if (JMESH_CMD_OUTPUT_CHOOSE>JMESH_CMD_OUTPUT_NONE)
void configuration_cmd_process(unsigned char interface,unsigned char length,unsigned char* data);
#else
#define configuration_cmd_process(interface,length,data)
#endif


#endif // CONFIGURATION_CMD_H
