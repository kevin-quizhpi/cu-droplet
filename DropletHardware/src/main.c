#include "main.h"


volatile float blah;

int main(void)
{
	uint16_t pg_no = 0;
	init_all_systems();
	printf("Initialized.\r\n");
			
			set_all_ir_powers(256);	
			
	char msg = NULL;
	uint8_t dir=0;
	uint8_t count;
	char dat;
	//set_rgb(0,0,230);
	while (1)
	{	
		//handle_shout(&msg);
		//delay_ms(500);		
		//delay_ms(20);
		//check_collisions();
		//
		//if(count==32)
		//{
			//dat = dir + '0';
			////if(dir==6)	ir_send(ALL_DIRS, &dat, 1);
			///*else*/		ir_send(1<<dir, &dat, 1);
			////ir_send(DIR0,&dat,1);
			//dir = (dir+1)%6;
			//count=0;
		//}else{
			//count++;
		//}
		//delay_ms(100);
		//check_messages();
		//delay_ms(28);
		//set_rgb(0,0,230);
		printf("%3hhd ", get_red_sensor());
		printf("%3hhd ", get_green_sensor());
		printf("%3hhd\r\n", get_blue_sensor());
		delay_ms(300);
		//if(get_red_sensor()>RED_THRESH){
			//set_rgb(0,0,230);
		//}else{
			//set_rgb(230,230,230);
		//}
		//for(uint8_t i=0;i<6;i++) printf("%hhu ", get_IR_sensor(i));
		//uint8_t randDir = rand_byte()%6;
		//int8_t meas = get_IR_sensor(randDir);
		//printf("| %hhu: %3hhd ",randDir, meas);
		//if(randDir==5) printf("\r\n");
		//printf("\r\n");
		//delay_ms(50);
		//set_rgb(200,0,0);
		//delay_ms(5000);
		//set_rgb(0,200,0);
		//delay_ms(5000);
		//set_rgb(0,0,200);
		//delay_ms(5000);
	}
}

//void turn_on_emitters()
//{
	//if(emitters_on=(!emitters_on))
	//{
		//for(uint8_t i=0;i<6;i++) ir_blast(1000); 
		//set_rgb(20,0,0); 
	//}
	//else
	//{
		//set_rgb(0,0,0);
	//}	
	//schedule_task(1000,turn_on_emitters, NULL);
//}

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

