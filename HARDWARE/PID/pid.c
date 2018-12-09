#include "pid.h"


/************************************************************************************ 
* @author： 镜璍氺月 
* @date : 2018/2/15 
* @fuction name：FUZZY_PID_CONTROL 
* @fuction description： 模糊自适应控制算法，为了方便测试默认e、ec在[-3,3]区间， 
* 如需改变e、ec范围，需引入量化因子(Ke、Kec=N/emax)、缩放因子(Ku=umax/N)。以下代码采 
*用三角隶属函数求隶属度以及加权平均法解模糊，PID采用位置式PID算法，算法仅供参考，欢迎报错。

以上转自https://blog.csdn.net/a841771798/article/details/79323118

  感谢原作者，现根据本项目情况有部分修改，

  我是用的增量式的pid，而且pid定义和原作不同  2018/6/10
*************************************************************************************/    


PID fuzzy(float e,float ec)  
{  
  
     float etemp, ectemp;  
     float eLefttemp, ecLefttemp;  
     float eRighttemp, ecRighttemp;  
  
     int eLeftIndex,ecLeftIndex;  
     int eRightIndex,ecRightIndex;  
     PID   fuzzy_PID;  
     etemp = e > 3.0 ? 0.0 : (e < - 3.0 ? 0.0 : (e >= 0.0 ? (e >= 2.0 ? 2.5: (e >= 1.0 ? 1.5 : 0.5)) : (e >= -1.0 ? -0.5 : (e >= -2.0 ? -1.5 : (e >= -3.0 ? -2.5 : 0.0) ))));  
  
     eLeftIndex = (int)e;  
     eRightIndex = eLeftIndex;  
     eLeftIndex = (int)((etemp-0.5) + 3);        //[-3,3] -> [0,6]  
     eRightIndex = (int)((etemp+0.5) + 3);  
  
     eLefttemp =etemp == 0.0 ? 0.0:((etemp+0.5)-e);  
     eRighttemp=etemp == 0.0 ? 0.0:( e-(etemp-0.5));  
  
     ectemp = ec > 3.0 ? 0.0 : (ec < - 3.0 ? 0.0 : (ec >= 0.0 ? (ec >= 2.0 ? 2.5: (ec >= 1.0 ? 1.5 : 0.5)) : (ec >= -1.0 ? -0.5 : (ec >= -2.0 ? -1.5 : (ec >= -3.0 ? -2.5 : 0.0) ))));  
  
     ecLeftIndex = (int)((ectemp-0.5) + 3);        //[-3,3] -> [0,6]  
     ecRightIndex = (int)((ectemp+0.5) + 3);  
  
     ecLefttemp =ectemp == 0.0 ? 0.0:((ectemp+0.5)-ec);  
     ecRighttemp=ectemp == 0.0 ? 0.0:( ec-(ectemp-0.5));  
  
/*************************************反模糊*************************************/  

  
    fuzzy_PID.Kp = (eLefttemp * ecLefttemp *  fuzzyRuleKp[ecLeftIndex][eLeftIndex]                      
                    + eLefttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eLeftIndex]  
                    + eRighttemp * ecLefttemp * fuzzyRuleKp[ecLeftIndex][eRightIndex]  
                    + eRighttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eRightIndex]);  
  
    fuzzy_PID.Ki =   (eLefttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eLeftIndex]  
                    + eLefttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eLeftIndex]  
                    + eRighttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eRightIndex]  
                    + eRighttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eRightIndex]);  
  
    fuzzy_PID.Kd = (eLefttemp * ecLefttemp * fuzzyRuleKd[ecLeftIndex][eLeftIndex]  
                    + eLefttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eLeftIndex]  
                    + eRighttemp * ecLefttemp * fuzzyRuleKd[ecLeftIndex][eRightIndex]  
                    + eRighttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eRightIndex]);  
return fuzzy_PID;  
  
}  
  
/************ PID计算 ************/ 
void PID_Calc(PID_Regulator_t *pid)
{
	static float SUM=0;
    float dError, Error;
	PID OUT = {0,0,0};
	
	Error = pid->ref - pid->fdb;  //
	SUM += Error;               //积分项
	
	if(SUM > 160) {SUM = 160;}  //积分限幅
	if(SUM < -160){SUM = -160;}
	
	dError = pid->err[0] - pid->err[1];//微分项
	pid->err[1] = pid->err[0];
	pid->err[0] = Error;
	
	OUT = fuzzy(Error/100, dError/100); //+OUT.Kp*10 +OUT.Ki +OUT.Kd//模糊化处理得到隶属度
	
	pid->output = (pid->kp)*Error + (pid->ki) + (pid->kd)*dError; //计算pid
	
	if(pid->output > 6000)    //输出pwm限幅
		pid->output = 6000;
	else if(pid->output < -6000)
		pid->output = -6000;
}

