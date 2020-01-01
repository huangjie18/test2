#include"../jmesh_driver_config.h"
#if (JMESH_DRIVER_BX2400)
#include"app_pwm.h"
#include"bx_dbg.h"
#include"../jmesh_pwm.h"
#include"stdio.h"
#include"../../jmesh/jmesh_save.h"
#include"io_ctrl.h"
static app_pwm_inst_t pwm_inst[5] = {PWM_INSTANCE(0),PWM_INSTANCE(1),PWM_INSTANCE(2),
																			PWM_INSTANCE(3),PWM_INSTANCE(4)};
typedef struct {
    app_pwm_comm_params_t param; 
    jmesh_pwm_channel_t channel;
    unsigned char is_valid;	
}app_pwm_list_t;

#include"jmesh_print.h"
void jmesh_pwm_start(jmesh_pwm_pin_num_t pin_num,jmesh_pwm_channel_t channel, unsigned short high_ms, unsigned short low_ms)
{
    if(channel >4)	BX_ASSERT(0);
	app_pwm_list_t pwm_list;
	unsigned char i;
	signed char j=-1,idx=-1;
    for(i=0;i<5;i++){
		if(jmesh_save_buffer_read(JMESH_SAVE_PWM,i,sizeof(app_pwm_list_t),(unsigned char*)&pwm_list)){			
			if(pwm_list.is_valid){
				if(channel==pwm_list.channel){
					if(idx<0){
						idx=i;
					}
					if(pin_num!=pwm_list.param.pin_num){
				        io_pin_dir_set(pwm_list.param.pin_num,0);
					}				
				}
				if(pwm_list.param.pin_num==pin_num){
					pwm_list.is_valid=0;
					jmesh_save_buffer_write(JMESH_SAVE_PWM,i,sizeof(app_pwm_list_t),(unsigned char*)&pwm_list);
					if(j<0){
						j=i;
					}
				}
			}
			else if(j<0){
				j=i;
			}
		}
		else if(j<0){
			j=i;
		}	
	}
	if(idx>=0){
		//if(pin_num!=pwm_list.param.pin_num||pwm_list.param.high_time!=high_ms||pwm_list.param.low_time!=low_ms){
            pwm_list.param.pin_num=pin_num;
		    pwm_list.param.high_time=high_ms;
		    pwm_list.param.low_time=low_ms;
		    pwm_list.channel=channel;
		    pwm_list.is_valid=1;			
		    jmesh_save_buffer_write(JMESH_SAVE_PWM,idx,sizeof(app_pwm_list_t),(unsigned char*)&pwm_list);
		//}
	}
    else if(j>=0){
		pwm_list.param.pin_num=pin_num;
		pwm_list.param.high_time=high_ms;
		pwm_list.param.low_time=low_ms;
		pwm_list.channel=channel;
		pwm_list.is_valid=1;
		jmesh_save_buffer_write(JMESH_SAVE_PWM,j,sizeof(app_pwm_list_t),(unsigned char*)&pwm_list);
		
	}
//	else if(pin_num==pwm_list.param.pin_num||pwm_list.param.high_time!=high_ms||pwm_list.param.low_time!=low_ms){
//		pwm_list.param.pin_num=pin_num;
//		pwm_list.param.high_time=high_ms;
//		pwm_list.param.low_time=low_ms;
//		pwm_list.channel=channel;
//		jmesh_save_buffer_write(JMESH_SAVE_PWM,i,sizeof(app_pwm_list_t),(unsigned char*)&pwm_list);
//	}
    	
    pwm_inst[channel].param.pin_num=pin_num;
    pwm_inst[channel].param.high_time=high_ms;
    pwm_inst[channel].param.low_time=low_ms;
    app_pwm_init(&pwm_inst[channel].inst);
    app_pwm_start(&pwm_inst[channel].inst);
}
int jmesh_pwm_list(unsigned char *pin_num,jmesh_pwm_channel_t index, unsigned short *high_ms, unsigned short *low_ms,unsigned short *channel){
    app_pwm_list_t pwm_list;
	if(0==jmesh_save_buffer_read(JMESH_SAVE_PWM,index,sizeof(app_pwm_list_t),(unsigned char*)&pwm_list)){
	    return 0;
	}
    else if(pwm_list.is_valid==1) 
	{
        *pin_num=pwm_list.param.pin_num;
        *high_ms=pwm_list.param.high_time;
        *low_ms=pwm_list.param.low_time;
		*channel=pwm_list.channel;
        return 1;
	}
	else{
	    return 0;
	}
}

void jmesh_pwm_stop(jmesh_pwm_channel_t channel){
	int i=0;
	app_pwm_list_t pwm_list;
	if(channel >4)	BX_ASSERT(0);	
    for(i=0;i<5;i++){
	    if(jmesh_save_buffer_read(JMESH_SAVE_PWM,i,sizeof(app_pwm_list_t),(unsigned char*)&pwm_list)){
            if(channel==pwm_list.channel){
		        pwm_list.is_valid=0;
				jmesh_save_buffer_write(JMESH_SAVE_PWM,i,sizeof(app_pwm_list_t),(unsigned char*)&pwm_list);
				io_pin_dir_set(pwm_list.param.pin_num,0);
		    }
	    }		
	}        
    app_pwm_stop(&pwm_inst[channel].inst);	
}

#endif
