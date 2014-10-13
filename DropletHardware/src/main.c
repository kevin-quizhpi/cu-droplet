#include "main.h"

int main(void)
{
	init_all_systems();
	printf("Initialized.\r\n");
	set_all_ir_powers(256);	
	while (1)
	{	
		//collect_rnb_data(0x32A7, 255);
		for(uint8_t i=0 ; i<200 ; i++)
		{
			check_messages();
			if(rnb_updated)
			{
				printf("R: %f, ", (last_good_rnb.range));
				printf("B: %f, ", rad_to_deg(last_good_rnb.bearing));
				printf("H: %f\r\n", rad_to_deg(last_good_rnb.heading));
				rnb_updated = 0;
			}
			delay_ms(50);
		}
	}
}

void check_messages()
{
	uint8_t num_msgs = 0;
	while(last_ir_msg!=NULL)
	{
		printf("\tGot: \"");
		for(uint8_t i=0; i<last_ir_msg->msg_length; i++)
		{
			printf("%c",last_ir_msg->msg[i]);
		}
		printf("\"\r\n\tfrom %hx on ch%hhu %ums ago.\r\n",last_ir_msg->sender_ID, last_ir_msg->arrival_dir, get_time()-last_ir_msg->arrival_time);
		msg_node* temp = last_ir_msg;
		last_ir_msg = last_ir_msg->prev;
		free(temp->msg);
		free(temp);
		num_msgs++;
	}
	if(num_msgs>0) printf("Got %hhu messages.\r\n",num_msgs);
}

void color_cycle()
{
	for(uint16_t i=0;i<=360;i++)
	{
		set_hsv(i, 255, 10);
		delay_ms(10);
	}
}

