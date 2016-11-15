#include "user_template.h"

/*
 * Any code in this function will be run once, when the robot starts.
 */
void init(){
	loopID = 0xFFFF;
	frameCount = 0;
	mySlot = (get_droplet_id()%(SLOTS_PER_FRAME-1));
	frameStart = get_time();
}

/*
 * The code in this function will be called repeatedly, as fast as it can execute.
 */
void loop(){
	uint32_t frameTime = get_time()-frameStart;
	if(frameTime>FRAME_LENGTH_MS){
		frameTime = frameTime - FRAME_LENGTH_MS;
		frameStart += FRAME_LENGTH_MS;
		frameCount++;
	}
	if(loopID!=(frameTime/SLOT_LENGTH_MS)){
		loopID = frameTime/SLOT_LENGTH_MS;
		if(loopID==mySlot){
			//code you want to execute on your slot.
		}else if(loopID==SLOTS_PER_FRAME-1){
			//code you want to execute at the end of a frame.
		}
	}
	delay_ms(LOOP_DELAY_MS);
}

/*
 * After each pass through loop(), the robot checks for all messages it has 
 * received, and calls this function once for each message.
 */
void handle_msg(ir_msg* msg_struct){
	//if(((NearBotsMsg*)(msg_struct->msg))->flag==NEAR_BOTS_MSG_FLAG){
		//handleNearBotsMsg((NearBotsMsg*)(msg_struct->msg), msg_struct->sender_ID);
	//}else{
		//printf_P(PSTR("%hu byte msg from %04X:\r\n\t"), msg_struct->length, msg_struct->sender_ID);
		//for(uint8_t i=0;i<msg_struct->length;i++){
			//printf("%02hX ", msg_struct->msg[i]);
		//}
		//printf("\r\n");
	//}
}

///*
 //*	The function below is optional - commenting it in can be useful for debugging if you want to query
 //*	user variables over a serial connection.
 //*/
//uint8_t user_handle_command(char* command_word, char* command_args)
//{
	//return 0;
//}

///*
 //*	The function below is optional - if it is commented in, and the leg interrupts have been turned on
 //*	with enable_leg_status_interrupt(), this function will get called when that interrupt triggers.
 //*/	
//void	user_leg_status_interrupt()
//{
	//
//}