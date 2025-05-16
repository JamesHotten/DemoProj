#include "tcq_math.h"
#include "main.h"
#include "arm_math.h"
#include "arm_const_structs.h"

#include "stdio.h"
#include "stdlib.h"
static void gauss_solve(int n, double A[], double x[], double b[]);

/*

*********************************************************************************************************

\*  函 数 名: tcq_inter_f32_sinc

\*  功能说明: 利用sinc拟合，

\*  形  参: input：			输入的待插值数据
            input_num：		待插值的数据的个数
            output：		输出数据存放的数组
            output_num：	输出数据的个数
            au：			放大的倍数
			output_num < input_num * au * 0.6
\*  返 回 值: 无

*********************************************************************************************************

*/
void tcq_inter_f32_sinc(float *input, uint16_t input_num, float *output, uint16_t output_num, uint16_t au)
{
	float sinc;
	uint32_t m, n, o;
	uint32_t sinc_num = input_num * au * 2 + 10;
	uint32_t trans = (uint32_t)input_num * au * 0.2; //刚开始的数据是过度点，误差过大

	if (input_num * au * 0.6 < output_num)
	{
		while (1)
			; //错误输入
	}
	/***************/
	//卷积，注意这里要跳过au个点一算，留这些空来填点
	for (m = 0; m < input_num; m++)
	{
		o = m * au;
		for (n = 0; n < sinc_num; n++)
		{
			/*******生成sinc信号*********/
			{
				if (n == sinc_num / 2)
				{
					sinc = 1;
				}
				if (n > sinc_num / 2)
				{
					sinc = arm_sin_f32((n - sinc_num / 2) * PI / au) / ((n - sinc_num / 2) * PI / au);
				}
				if (n < sinc_num / 2)
				{
					sinc = arm_sin_f32(((sinc_num - n) - sinc_num / 2) * PI / au) / (((sinc_num - n) - sinc_num / 2) * PI / au);
				}
			}
			/***************/
			if (((o + n) > (sinc_num / 2 + trans - 1)) && (o + n) < (sinc_num / 2 + trans + output_num)) //当他满足，条件的地方才会进行运算
			{
				output[o + n - sinc_num / 2 - trans] += input[m] * sinc;
				//-trans是为了在有效数据区中移去过度点
				//- sinc_num / 2是为了进入有效数据区
			}
		}
	}
}

/*==================polyfit(n,x,y,poly_n,a)===================*/
/*=======拟合y=a0+a1*x+a2*x^2+……+apoly_n*x^poly_n========*/
/*=====n是数据个数 xy是数据值 poly_n是多项式的项数======*/
/*===返回a0,a1,a2,……a[poly_n]，系数比项数多一（常数项）=====*/
void polyfit(int n, double x[], double y[], int poly_n, double p[])
{
	int i, j;
	double *tempx, *tempy, *sumxx, *sumxy, *ata;

	tempx = (double *)calloc(n, sizeof(double));
	sumxx = (double *)calloc((poly_n * 2 + 1), sizeof(double));
	tempy = (double *)calloc(n, sizeof(double));
	sumxy = (double *)calloc((poly_n + 1), sizeof(double));
	ata = (double *)calloc((poly_n + 1) * (poly_n + 1), sizeof(double));
	for (i = 0; i < n; i++)
	{
		tempx[i] = 1;
		tempy[i] = y[i];
	}
	for (i = 0; i < 2 * poly_n + 1; i++)
	{
		for (sumxx[i] = 0, j = 0; j < n; j++)
		{
			sumxx[i] += tempx[j];
			tempx[j] *= x[j];
		}
	}
	for (i = 0; i < poly_n + 1; i++)
	{
		for (sumxy[i] = 0, j = 0; j < n; j++)
		{
			sumxy[i] += tempy[j];
			tempy[j] *= x[j];
		}
	}
	for (i = 0; i < poly_n + 1; i++)
	{
		for (j = 0; j < poly_n + 1; j++)
		{
			ata[i * (poly_n + 1) + j] = sumxx[i + j];
		}
	}
	gauss_solve(poly_n + 1, ata, p, sumxy);

	free(tempx);
	free(sumxx);
	free(tempy);
	free(sumxy);
	free(ata);
}
/*============================================================*/
// 高斯消元法计算得到	n 次多项式的系数
// n: 系数的个数
// ata: 线性矩阵
// sumxy: 线性方程组的Y值
// p: 返回拟合的结果
/*============================================================*/
static void gauss_solve(int n, double A[], double x[], double b[])
{
	int i, j, k, r;
	double max;
	for (k = 0; k < n - 1; k++)
	{
		max = fabs(A[k * n + k]); // find maxmum
		r = k;
		for (i = k + 1; i < n - 1; i++)
		{
			if (max < fabs(A[i * n + i]))
			{
				max = fabs(A[i * n + i]);
				r = i;
			}
		}
		if (r != k)
		{
			for (i = 0; i < n; i++) //change array:A[k]&A[r]
			{
				max = A[k * n + i];
				A[k * n + i] = A[r * n + i];
				A[r * n + i] = max;
			}
			max = b[k]; //change array:b[k]&b[r]
			b[k] = b[r];
			b[r] = max;
		}

		for (i = k + 1; i < n; i++)
		{
			for (j = k + 1; j < n; j++)
				A[i * n + j] -= A[i * n + k] * A[k * n + j] / A[k * n + k];
			b[i] -= A[i * n + k] * b[k] / A[k * n + k];
		}
	}

	for (i = n - 1; i >= 0; x[i] /= A[i * n + i], i--)
	{
		for (j = i + 1, x[i] = b[i]; j < n; j++)
			x[i] -= A[i * n + j] * x[j];
	}
}

/*

*********************************************************************************************************

\*  函 数 名: tcq_get_fc

\*  功能说明: 根据测到的频率，预期的采样点数，返回预期的采样率，采样周期

\*  形  参: fq:测量到的信号频率
            num	:需要测量的点数
            fs_max:最大支持的采样率
            fs:返回的采样率
            circle:返回的采样到的周期
\*  返 回 值: 无

*********************************************************************************************************

*/
void tcq_get_fs(float fq,uint16_t num,float fs_max, float* fs, uint16_t* circle)
{
    uint32_t fs_reg;
    uint16_t i;
    for(i=1;;i++)
    {
        fs_reg = num*fq/i;
        if(fs_reg<fs_max)
        {
            break;//当采样频率大于最大值，退出
        }
    }
    *fs = fs_reg;
    *circle = i;
}
/*

*********************************************************************************************************

\*  函 数 名: tcq_extract_value

\*  功能说明: 从数据中，等距离抽取指定个数的数据

\*  形  参: before[]:抽取前的数据
            after[]	:抽取后的数据
            num_before:抽取前的点数
            num_after:抽取后的点数
\*  返 回 值: 无

*********************************************************************************************************

*/
void tcq_extract_value(uint16_t before[], uint16_t after[],uint16_t num_before, uint16_t num_after)
{
    float i=0,j=0;
    float a = (float)num_before / num_after;
    for(i=0;i<num_before;i=i+a)
    {
        after[(uint16_t)j++]=before[(uint16_t)i];
    }
}
/*

*********************************************************************************************************

\*  函 数 名: tcq_extract_value_f32

\*  功能说明: 从数据中，等距离抽取指定个数的数据

\*  形  参: before[]:抽取前的数据
            after[]	:抽取后的数据
            num_before:抽取前的点数
            num_after:抽取后的点数
\*  返 回 值: 无

*********************************************************************************************************

*/
void tcq_extract_value_f32(float before[], float after[],uint16_t num_before, uint16_t num_after)
{
    float i=0,j=0;
    float a = (float)num_before / (num_after-1);
    for(i=0;i<num_before;i=i+a)
    {
        after[(uint16_t)j++]=before[(uint16_t)i];
    }
}

/**
 * @bieaf 微秒延时函数
 * @detail 注意:由于指令执行需要时间, 实际的延时精度约为1微秒
 *
 * @param unsigned int Delay 延时的微秒
 */
void delay_us(unsigned int Delay)
{
    uint32_t tickstart = SysTick->VAL;                            ///<获取当前tick
    uint32_t tickNum = 0;
    uint32_t tickMax = SysTick->LOAD + 1;
    uint32_t delay_usvalue = (tickMax / 1000) * Delay;            ///<计算一共需要延时的tick
    while(1)
    {
        uint32_t cur_tick = SysTick->VAL;
        if (cur_tick > tickstart)//默认他不会重装在2次              ///<进行了一次重载
        {
            tickNum = tickstart + (tickMax - cur_tick);
        }
        else                                                      ///<未进行过重载
        {
            tickNum = tickstart - cur_tick;
        }

        if (tickNum > delay_usvalue)                              ///<达到延时的tick数
        {
            return;
        }
    }
}
/**
 * @bieaf 纳秒延时函数
 * @detail 注意:由于指令执行需要时间, 误差约5ns，最小延迟110ns
 *
 * @param unsigned int Delay 延时的微秒
 */
void delay_ns(unsigned int Delay)
{
    uint32_t tickstart = SysTick->VAL;                            ///<获取当前tick
    uint32_t tickNum = 0;
    uint32_t tickMax = SysTick->LOAD + 1;
    uint32_t delay_usvalue = (tickMax / 1000) * (Delay-105)/1000;            ///<计算一共需要延时的tick
    while(1)
    {
        uint32_t cur_tick = SysTick->VAL;
        if (cur_tick > tickstart)//默认他不会重装在2次              ///<进行了一次重载
        {
            tickNum = tickstart + (tickMax - cur_tick);
        }
        else                                                      ///<未进行过重载
        {
            tickNum = tickstart - cur_tick;
        }

        if (tickNum > delay_usvalue)                              ///<达到延时的tick数
        {
            return;
        }
    }
}
//寻找第二大数
uint16_t tcq_find_set_max(uint16_t data[], int count)
{
    uint16_t maxnumber = data[0];
    int16_t sec_max = -32767;

    for (int i = 1; i < count; i++)
    {
        if (maxnumber < data[i])
        {
            sec_max = maxnumber; //将data[0]用来存放最大的数，通过一个sec_max来存放永远是上一个max 则为第二大的数
            maxnumber = data[i];
        }

        else
        {
            if (data[i] > sec_max) // 在max没有更新的时候 sec也必须更新 就是不断和data[i]比较 找这其中的最大值
                sec_max = data[i];
        }
    }

    return sec_max;
}

float tcq_find_set_max_f32(float data[], int count)
{

    float maxnumber = data[0];
    float sec_max = -32767;

    for (int i = 1; i < count; i++)
    {
        if (maxnumber < data[i])
        {
            sec_max = maxnumber; //将data[0]用来存放最大的数，通过一个sec_max来存放永远是上一个max 则为第二大的数
            maxnumber = data[i];
        }
        else
        {
            if (data[i] > sec_max) // 在max没有更新的时候 sec也必须更新 就是不断和data[i]比较 找这其中的最大值
                sec_max = data[i];
        }
    }
    return sec_max;
}

void search_near_q31(uint32_t origin[], uint32_t num,uint32_t pianyi, uint32_t data, uint32_t *out, uint32_t *out_address)
{
    uint32_t i;
    uint32_t address_temp = 0;
    uint32_t out_temp = origin[0];
    for (i = pianyi; i < pianyi+num; i++)
    {
        if (abs((int)out_temp - (int)data) > abs((int)origin[i] - (int)data))
        {
            out_temp = origin[i];
            address_temp = i;
        }
    }
    *out = out_temp;
    *out_address = address_temp;
}

void search_near_f32(float origin[], uint32_t num, uint32_t pianyi,float data, float *out, uint32_t *out_address)
{
    uint32_t i;
    uint32_t address_temp = 0;
    float out_temp = origin[pianyi];
    for (i = pianyi; i < pianyi+num; i++)
    {
        if (fabs(out_temp - data) > fabs(origin[i] - data))
        {
            out_temp = origin[i];
            address_temp = i;
        }
    }
    *out = out_temp;
    *out_address = address_temp;
}
