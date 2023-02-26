#include <REGX52.H>
#include<INTRINS.H>
// LED 是P2_0到P2_7 亮是0 不亮是1
#define LED P2
// 独立按键 K1 K2 K3 K4 按下是0
#define K1 P3_1
#define K2 P3_0
#define K3 P3_2
#define K4 P3_3

typedef unsigned char uchar;

// 临时变量
uchar tmp = 0;

void delay_500ms()		//@11.0592MHz
{
    uchar i, j, k;

    _nop_();
    i = 4;
    j = 129;
    k = 119;
    do
    {
        do
        {
            while (--k);
        } while (--j);
    } while (--i);
}


void delay_20ms()		//@11.0592MHz
{
    uchar i, j;

    i = 36;
    j = 217;
    do
    {
        while (--j);
    } while (--i);
}


// 延迟1ms
void delay_1ms()		//@11.0592MHz
{
    unsigned char i, j;

    _nop_();
    i = 2;
    j = 199;
    do
    {
        while (--j);
    } while (--i);
}


// 颠倒unsigned char类型的位
// 具体思路是：先交换高4与低4 再分别交换高4的高2与低2...
uchar reverse_8_bits(uchar n)
{
    // 11交换
    n = n >> 1 & 0x55 | (n & 0x55) << 1;
    // 22交换
    n = n >> 2 & 0x33 | (n & 0x33) << 2;
    // 44交换
    return n >> 4 | n << 4;
}



// 向右流水灯
void flow_led_right()
{
    LED = 0xFE; // 设置灯初始状态
    while (1)
    {
        delay_500ms();  // 延时0.5s
        P2 = _crol_(LED, 1);
    }
}

// 向左流水灯
// 向右流水灯
void flow_led_left()
{
    LED = 0x7f; // 设置灯初始状态
    while (1)
    {
        delay_500ms();  // 延时0.5s
        P2 = _cror_(LED, 1);
    }
}

// k1 k2 实现LED二进制
// 左加右减
void two_caculate()
{
    // 初始状态
    tmp = 0xff;
    while (1)
    {
        if (K1 == 0)			//如果K1按键按下
        {
            delay_20ms();	    //延时消抖
            while (K1 == 0);	//松手检测 一直按下的话就什么反应都没有
            delay_20ms();        //延时消抖
            tmp--;
            LED = reverse_8_bits(tmp);
        }
        else if (K2 == 0)
        {
            delay_20ms();		//延时消抖
            while (K2 == 0);	//松手检测 一直按下的话就什么反应都没有
            delay_20ms();		//延时消抖
            tmp++;
            LED = reverse_8_bits(tmp);
        }
    }

}


// 静态显示代码管
// 数码管段码表 它根据数码管led特有的排布，从而显示特殊的数字
// 分别是0,1,2,3...
uchar code nixie_table[] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F };

// 静态数码管显示
void nixie_static(uchar location, number)
{
    switch (location)		//位码输出
    {
    case 1:P2_4 = 1;P2_3 = 1;P2_2 = 1;break;
    case 2:P2_4 = 1;P2_3 = 1;P2_2 = 0;break;
    case 3:P2_4 = 1;P2_3 = 0;P2_2 = 1;break;
    case 4:P2_4 = 1;P2_3 = 0;P2_2 = 0;break;
    case 5:P2_4 = 0;P2_3 = 1;P2_2 = 1;break;
    case 6:P2_4 = 0;P2_3 = 1;P2_2 = 0;break;
    case 7:P2_4 = 0;P2_3 = 0;P2_2 = 1;break;
    case 8:P2_4 = 0;P2_3 = 0;P2_2 = 0;break;
    }
    P0 = nixie_table[number];	//段码输出
}


// 动态数码管显示
// 显示位置、显示数字、数组大小
void nixie_dynamic(uchar *locations, uchar *numbers, uchar size)
{
    while (1)
    {
        for (tmp = 0; tmp < size; tmp++)
        {
            nixie_static(locations[tmp], numbers[tmp]);
            // 为了解决消影
            delay_1ms();
            P0 = 0x00;
        }
    }
}


// k1 k2控制左右流水灯
void flow_led_k1_k2()
{

}
