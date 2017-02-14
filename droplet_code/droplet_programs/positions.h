#pragma once

#include "droplet_init.h"

//#define OCC_DEBUG_MODE
//#define POS_CALC_DEBUG_MODE
//#define POS_MSG_DEBUG_MODE
#define GEN_DEBUG_MODE
//#define RNB_DEBUG_MODE
#define MY_POS_DEBUG_MODE

#ifdef OCC_DEBUG_MODE
#define OCC_DEBUG_PRINT(format, ...) printf_P(PSTR(format), ##__VA_ARGS__)
#else
#define OCC_DEBUG_PRINT(format, ...)
#endif

#ifdef POS_CALC_DEBUG_MODE
#define POS_CALC_DEBUG_PRINT(format, ...) printf_P(PSTR(format), ##__VA_ARGS__)
#else
#define POS_CALC_DEBUG_PRINT(format, ...)
#endif

#ifdef POS_MSG_DEBUG_MODE
#define POS_MSG_DEBUG_PRINT(format, ...) printf_P(PSTR(format), ##__VA_ARGS__)
#else
#define POS_MSG_DEBUG_PRINT(format, ...)
#endif

#ifdef GEN_DEBUG_MODE
#define GEN_DEBUG_PRINT(format, ...) printf_P(PSTR(format), ##__VA_ARGS__)
#else
#define GEN_DEBUG_PRINT(format, ...)
#endif

#ifdef RNB_DEBUG_MODE
#define RNB_DEBUG_PRINT(format, ...) printf_P(PSTR(format), ##__VA_ARGS__)
#else
#define RNB_DEBUG_PRINT(format, ...)
#endif

#ifdef MY_POS_DEBUG_MODE
#define MY_POS_DEBUG_PRINT(format, ...) printf_P(PSTR(format), ##__VA_ARGS__)
#else
#define MY_POS_DEBUG_PRINT(format, ...)
#endif

/*
 * rnb takes 188 ms
 * messages take 2.5ms per byte. 
 * paddleMsg is 3 bytes. header is 8 bytes, so PaddleMsg should take 27.5
 * ballMsg is 7 bytes. header is 8 bytes, so ballMsg should take 37.5ms 
 * BotMeasMsg is 20 bytes. Header is 8 bytes, so msg should take 70ms (tripled)
 * BotPosMsg is 7 bytes. Header is 8 bytes, so msg should take 37.5ms
 */
#define RNB_DUR				220
#define PADDING_DUR			20 //padding.
#define POS_MSG_DUR			40
#define MEAS_MSG_DUR		71

#define SLOT_LENGTH_MS			487
#define SLOTS_PER_FRAME			28
#define FRAME_LENGTH_MS			(((uint32_t)SLOT_LENGTH_MS)*((uint32_t)SLOTS_PER_FRAME))
#define LOOP_DELAY_MS			17

///*
 //* See the top of page 16 in my notebook for basis for measCovar stuff below.
 //*/
Matrix measCovarClose  = {{213, 11, 31}, {11, 124, 139}, {31, 139, 290}};
Matrix measCovarMed = {{384, -9, -119}, {-9, 626, 166}, {-119, 166, 724}};
Matrix measCovarFar = {{1643, 408, -204}, {408, 9374, 141}, {-204, 141, 3392}};
//Matrix deltaPoseCovarClose = {{76, 15, 29}, {15, 83, 44}, {29, 44, 220}};
//Matrix deltaPoseCovarMed   = {{1569, 106, -163}, {106, 633, 35}, {-163, 35, 871}};

#define NUM_SEEDS 4
#define NUM_SHARED_BOTS 6
#define NUM_TRACKED_BOTS 12
const id_t	   SEED_IDS[NUM_SEEDS]	   = {0xDC62, 0x9363, 0x6597, 0x6C6F};
const int16_t  SEED_X[NUM_SEEDS]   = {100, 600, 100, 600};
const int16_t  SEED_Y[NUM_SEEDS]   = {600, 600, 100, 100};
#define MIN_X 0
#define MIN_Y 0
#define MAX_X 700
#define MAX_Y 700

#define UNDF	((int16_t)0x8000)

#define DROPLET_DIAMETER_MM		44.4
#define BOT_MEAS_MSG_FLAG		'X'
#define BOT_POS_MSG_FLAG		'P'

#define POS_DEFINED(pos) ((((pos)->x)!=UNDF)&&(((pos)->y)!=UNDF)&&(((pos)->o)!=UNDF))

typedef struct ball_msg_struct{
	char flag;
	uint8_t xPos;
	uint8_t yPos;
	uint8_t extraBits; //bits 7-5 are three high bits for xPos; bits 4-2 are three high bits for yPos; bits 0-1 are two low bits for id.
	int8_t xVel;
	int8_t yVel;
	uint8_t radius; //bits 0-1 are two high bits for id. rest is radius (which must be divisible by 4)
}BallMsg;

typedef union flex_byte_union{
	uint16_t u;
	int16_t d;
}FlexByte;

typedef FlexByte DensePosCovar[6];

typedef struct bot_pos_struct
{
	int16_t x;
	int16_t y;
	int16_t o;
} BotPos;

typedef struct bot_meas_msg_struct{
	BotPos pos; //6 bytes
	DensePosCovar covar; //12 bytes
	uint8_t seedIdx;
	char flag;
}BotMeasMsg;

typedef struct bot_pos_msg_struct{
	BotPos pos; //6 bytes
	char flag;
}BotPosMsg;

typedef struct bot_meas_struct
{
	id_t id;
	uint16_t r;
	int16_t b;
	int16_t h;
} BotMeas;

typedef struct other_bot_rnb_struct{
	BotMeas myMeas;
	BotPos posFromMe;
	DensePosCovar posCovar;
	uint8_t occluded;
	uint8_t seedIdx;
	//uint8_t hasNewInfo;
} OtherBot;
OtherBot nearBots[NUM_TRACKED_BOTS+1];

BotPos myPos;
BotPos perSeedPos[NUM_SEEDS];
DensePosCovar perSeedCovars[NUM_SEEDS];
//BotMeasMsg preppedMsg;
//id_t preppedMsgTarget;
//uint8_t preppedMsgDirMask;
//uint16_t msgExtraDelay;

uint8_t		seedFlag;
uint8_t		myState;
uint32_t	frameCount;
uint32_t	frameStart;
uint16_t	mySlot;
uint16_t	loopID;
int16_t		xRange;
int16_t		yRange;
int16_t		maxRange;



void		init();
void		loop();
void		handleMySlot();
void		handleFrameEnd(); 


void		getWeightedRandOtherBots(uint8_t* botIdxs, uint8_t n);

uint8_t     nearBotUseabilityCheck(uint8_t i);
float		chooseOmega(Matrix* myPinv, Matrix* yourPinv);
void		ciUpdatePos(uint8_t idx, BotPos* pos, Matrix* yourP);
void		updatePositions();
void		getMeasCovar(Matrix* R, BotMeas* meas);
void		calcRelativePose(Vector* pose, BotMeas* meas);
void		populateGammaMatrix(Matrix* G, Vector* pos);
void		populateHMatrix(Matrix* H, Vector* x_me, Vector* x_you);
void		compressP(Matrix* P, DensePosCovar* covar);
void		decompressP(Matrix* P, DensePosCovar* covar);

void		useNewRnbMeas();

void		sendBotPosMsg();
void		sendBotMeasMsg(uint8_t i);
//void		prepBotMeasMsg(uint8_t i);
void		handleBotMeasMsg(BotMeasMsg* msg, id_t senderID);
void		handleBotPosMsg(BotPosMsg* msg, id_t senderID);

void		updateColor();
//Coverage functions below are helper functions for 'updateColor'.
void		updateNearBotOcclusions();
void		handle_msg			(ir_msg* msg_struct);

void		fusePerSeedMeas();

void		printPosCovar(DensePosCovar* P);
void		printTwoPosCovar(DensePosCovar* P1, DensePosCovar* P2);
void		frameEndPrintout();

//These four functions are for interacting with the OtherBot data structure.
OtherBot*	getOtherBot(id_t id);
void		findAndRemoveOtherBot(id_t id);
void		removeOtherBot(uint8_t idx);
OtherBot*	addOtherBot(id_t id);
void		cleanOtherBot(OtherBot* other);

void printNearBots();
void printOtherBot(OtherBot* bot);

inline static void copyBotPos(BotPos* src, BotPos* dest){
	dest->x = src->x;
	dest->y = src->y;
	dest->o = src->o;
}

inline static uint8_t dirFromAngle(int16_t angle){
	return abs((angle - (angle>0 ? 360 : 0))/60);
}

static int nearBotsCmpFunc(const void* a, const void* b){
	OtherBot* aN = (OtherBot*)a;
	OtherBot* bN = (OtherBot*)b;
	uint16_t aRange = (aN->myMeas).r;
	uint16_t bRange = (bN->myMeas).r;
	if(aRange < bRange){
		return -1;
	}else if(bRange < aRange){
		return 1;
	}else{
		return 0;
	}
}

inline static void initPositions(){
	myPos.x = UNDF;
	myPos.y = UNDF;
	myPos.o = UNDF;
	for(uint8_t i=0;i<NUM_SEEDS;i++){
		perSeedPos[i].x = UNDF;
		perSeedPos[i].y = UNDF;
		perSeedPos[i].o = UNDF;
		for(uint8_t j=0;j<6;j++){
			perSeedCovars[i][j].u = 0;
		}
	}

	seedFlag = 0;	
	for(uint8_t i=0;i<NUM_SEEDS;i++){
		if(get_droplet_id()==SEED_IDS[i]){
			seedFlag = 1;
			myPos.x = SEED_X[i];
			myPos.y = SEED_Y[i];
			myPos.o = 0;
			perSeedPos[i].x = myPos.x;
			perSeedPos[i].y = myPos.y;
			perSeedPos[i].o = myPos.o;
			perSeedCovars[i][0].u = 1;
			perSeedCovars[i][3].u = 1;
			perSeedCovars[i][5].u = 1;
			break;
		}
	}
}

inline static uint16_t getSlot(id_t id){
	return (id%(SLOTS_PER_FRAME-1));
}