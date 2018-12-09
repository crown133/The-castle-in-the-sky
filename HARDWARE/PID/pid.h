#ifndef _PID_H_
#define _PID_H_


#define NL   -3
#define NM	 -2
#define NS	 -1
#define ZE	 0
#define PS	 1
#define PM	 2
#define PL	 3


typedef struct PID_Regulator_t
{
	float ref;
	float fdb;
	float err[2];//error_now  error_last error_inter
	float kp;
	float ki;
	float kd;
	float output;
	//void (*Calc)(struct PID_Regulator_t *pid);//定义了一个函数指针，没有返回值，函数名是Calc，形参的类型是
                                      //PID_Regulator_t结构体变量的指针 ，所以需要对这个指针进行赋值，表示这个指针指向那个函数 
}PID_Regulator_t;

extern PID_Regulator_t Cir1_PID, Cir2_PID;
void PID_Calc(PID_Regulator_t *pid);

static const float fuzzyRuleKp[7][7]={  //Kp模糊规则表
	PL,	PL,	PM,	PM,	PS,	PS,	ZE,
	PL,	PL,	PM,	PM,	PS,	ZE,	ZE,
	PM,	PM,	PM,	PS,	ZE,	NS,	NM,
	PM,	PS,	PS,	ZE,	NS,	NM,	NM,
	PS,	PS,	ZE,	NS,	NS,	NM,	NM,
	ZE,	ZE,	NS,	NM,	NM,	NM,	NL,
	ZE,	NS,	NS,	NM,	NM,	NL,	NL
};

static const float fuzzyRuleKi[7][7]={  //Ki模糊规则表
	NL,	NL,	NL,	NM,	NM,	ZE,	ZE,
	NL,	NL,	NM,	NM,	NS,	ZE,	ZE,
	NM,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	ZE,	PS,	PS,	PM,
	NS,	NS,	ZE,	PS,	PS,	PM,	PM,
	ZE,	ZE,	PS,	PM,	PM,	PL,	PL,
	ZE,	ZE,	PS,	PM,	PL,	PL,	PL
};

static const float fuzzyRuleKd[7][7]={  //Kd模糊规则表
	PS,	PS,	ZE,	ZE,	ZE,	PL,	PL,
	NS,	NS,	NS,	NS,	ZE,	NS,	PM,
	NL,	NL,	NM,	NS,	ZE,	PS,	PM,
	NL,	NM,	NM,	NS,	ZE,	PS,	PM,
	NL,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	NS,	ZE,	PS,	PS,
	PS,	ZE,	ZE,	ZE,	ZE,	PL,	PL
};

typedef struct{
	float Kp;
	float Ki;
	float Kd;
}PID;



#endif
