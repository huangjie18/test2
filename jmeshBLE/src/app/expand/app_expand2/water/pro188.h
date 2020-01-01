#ifndef PRO188_H
#define PRO188_H

enum{
    PRO188_TYPE_WATER_COLD=0x10,
    PRO188_TYPE_WATER_HOT=0x11,
    PRO188_TYPE_WATER_DRINKING=0x12,
    PRO188_TYPE_WATER_RECYCLED=0x13,

    PRO188_TYPE_HEAT_COLD=0x20,
    PRO188_TYPE_HEAT_HOT=0x20,
    PRO188_TYPE_GAS=0x30,
    PRO188_TYPE_ELSE=0x40,
};
enum{
    PRO188_CODE_READ_DATA=0x01,
    PRO188_CODE_READ_VERSION=0x09,
    PRO188_CODE_READ_ADDR=0x03,
};
enum{
    PRO188_DI_REAL_VALUE=0x901f,
    PRO188_DI_VERSION=0x8106,
    PRO188_DI_ADDRESS=0x810A,
};
enum{
    PRO188_UNIT_Wh=0x02,
    PRO188_UNIT_kWh=0x05,
    PRO188_UNIT_MWh=0x08,
    PRO188_UNIT_MWhx100=0x0A,
    PRO188_UNIT_J=0x01,
    PRO188_UNIT_kJ=0x0B,
    PRO188_UNIT_MJ=0x0E,
    PRO188_UNIT_GJ=0x11,
    PRO188_UNIT_GJx100=0x13,
    PRO188_UNIT_W=0x14,
    PRO188_UNIT_kW=0x17,
    PRO188_UNIT_MW=0x1A,
    PRO188_UNIT_L=0x29,
    PRO188_UNIT_m3=0x2C,
    PRO188_UNIT_L_h=0x32,
    PRO188_UNIT_m3_h=0x35,
};

enum{
    PRO188_STATUS_VALVE_ON=0x00,
    PRO188_STATUS_VALVE_OFF=0x01,
    PRO188_STATUS_VALVE_ERROR=0x03,
};
enum{
    PRO188_STATUS_BATTERY_OK=0x00,
    PRO188_STATUS_BATTERY_LOW=0x04,
};
typedef struct st_pro188{
    unsigned char head;
    unsigned char type;
    unsigned char addr[7];
    unsigned char c;
    unsigned char len;
    unsigned char di0;
    unsigned char di1;
    unsigned char ser;
    union{
        unsigned char data[1];
        struct{
            unsigned char real_value[4];
            unsigned char real_value_unit;
            unsigned char date_value[4];
            unsigned char date_value_unit;
            unsigned char timestamp[7];
            unsigned char status[2];
        }cold_water;
    }data;
}pro188_t;

int pro188_pick(unsigned char len,const unsigned char* data,unsigned char* offset);
int pro188_pack(const unsigned char* addr,unsigned short di,unsigned char t,unsigned char c,unsigned char ser,
                unsigned char len,unsigned char buffer_size,const unsigned char* data,unsigned char* buffer);

#endif // PRO188_H
