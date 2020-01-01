#include"pro188.h"

/* 68 T a0 a1 a2 a3 a4 a5 a6 C L DATA CS 16
    0 1  2  3  4  5  6  7  8 9 10     11 12
*/




static unsigned char check_sum(unsigned short len,const unsigned char* data){
    unsigned char cs;

    for(cs=0;len>0;){
        len--;
        cs+=data[len];
    }
    return cs;
}

int pro188_pick(unsigned char len,const unsigned char* data,unsigned char* offset)
{
    unsigned char i;

    for(i=0;i+12<len;i++){
        if(data[i]==0x68
            &&len>=(i+data[i+10]+12)
            &&data[i+data[i+10]+12]==0x16
            &&data[i+data[i+10]+11]==check_sum(data[i+10]+11,data+i)){
            *offset=i;
            return data[i+10]+13;
        }
    }
    return 0;
}
int pro188_pack(const unsigned char* addr,unsigned short di,unsigned char t,unsigned char c,unsigned char ser,unsigned char len,unsigned char buffer_size,const unsigned char* data,unsigned char* buffer)
{
    int i;
    if(buffer_size<len+15){
        return -1;
    }
    buffer[0]=0x68;
    buffer[1]=t;
    for(i=0;i<7;i++){
        buffer[i+2]=addr[i];
    }
    buffer[9]=c;
    buffer[10]=len+3;
    buffer[11]=(unsigned char)(di>>8);
    buffer[12]=(unsigned char)di;
    buffer[13]=ser;

    for(i=0;i<len;i++){
        buffer[14+i]=data[i];
    }
    buffer[len+11+3]=check_sum(len+11+3,buffer);
    buffer[len+12+3]=0x16;
    return len+13+3;
}
