#include "process.h"

struct
{
    uint8_t mode;      //当先模式
    uint8_t mode_last; //记录上次模式
    uint8_t ready;     //开启测量
} state;

void process_init()
{
    state.mode = 0;
    state.mode_last = 0;
    state.ready = 0;
}
void process0()
{
    /*****过程初始化****/
    if (state.mode != state.mode_last)
    {
        if (state.mode != 0x00)//防止操作快引起的模式错位
        {
            return;
        }
        state.mode_last = state.mode;
    }
    /*********************/
}

void process1()
{
    /*****过程初始化****/
    if (state.mode != state.mode_last)
    {
        if (state.mode != 0x01)//防止操作快引起的模式错位
        {
            return;
        }
        state.mode_last = state.mode;
    }
    /*********************/
}

void process2()
{
    /*****过程初始化****/
    if (state.mode != state.mode_last)
    {
        if (state.mode != 0x02)//防止操作快引起的模式错位
        {
            return;
        }
        state.mode_last = state.mode;
    }
    /*********************/
}

void process3()
{
    /*****过程初始化****/
    if (state.mode != state.mode_last)
    {
        if (state.mode != 0x03)//防止操作快引起的模式错位
        {
            return;
        }
        state.mode_last = state.mode;
    }
    /*********************/
}

void printf_info()
{
    /*****过程初始化****/
    if (state.mode != state.mode_last)
    {

        state.mode_last = state.mode;
    }
    /*********************/
}

void mode_switch()
{
    switch (state.mode) //模式选择
    {
    case 0x00:
    {
        process0();
    }
    break;
    }
}

void IDLE_process()
{
}