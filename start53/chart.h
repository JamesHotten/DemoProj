#pragma once
#include <math.h>
#include <string.h>
#include "uart.h"
#define hexEND "\xff\xff\xff"

typedef struct
{
    /* data */
    const char *objname;
    int Xmin, Xmax, Ymin, Ymax;   // 实际的像素坐标系
    float xmin, xmax, ymin, ymax; // 虚拟的坐标系
    float lastx, lasty;           // 上次绘制的点

} Chart;

int mapChart(float x, float xmin, float xmax, int ymin, int ymax)
{ // 区域映射
    if(x >= xmax) x = xmax;
    if(x <= xmin) x = xmin;
    return (int) ((x - xmin) * (ymax - ymin) / (xmax - xmin) + ymin);
}

void initChart(Chart *obj, const char *objname, int x, int y, int w, int h)
{
    obj->objname = objname;
    obj->Xmin = x;
    obj->Xmax = x + w;
    obj->Ymin = y;
    obj->Ymax = y + h;
    obj->lastx = NAN;
}

void axis(Chart *obj, float x_min, float x_max, float y_min, float y_max)
{
    obj->xmin = x_min;
    obj->xmax = x_max;
    obj->ymin = y_min;
    obj->ymax = y_max;
}

void clf(Chart *obj, const char *color = "WHITE")
{
    const static int space = 4;
    obj->lastx = NAN;
    HMI_printf("fill %d,%d,%d,%d,%s" hexEND,
               obj->Xmin - space, obj->Ymin - space, obj->Xmax - obj->Xmin + space * 2, obj->Ymax - obj->Ymin + space * 2, color);
    HMI_printf("draw %d,%d,%d,%d,%s" hexEND, obj->Xmin, obj->Ymin, obj->Xmax, obj->Ymax, "BLACK");
    // HMI_printf("ref %s" hexEND, objname);
}

void text(Chart *obj, int X, int Y, int W, int H, int xcenter, int ycenter, const char *label, bool transparent)
{
    const static int fontId = 0;
    HMI_printf("xstr %d,%d,%d,%d,%d,BLACK,WHITE,%d,%d,%d,\"%s\"" hexEND,
               xcenter == 0 ? X : xcenter == 1 ? X - W / 2 : X - W,
               ycenter == 0 ? Y : ycenter == 1 ? Y - H / 2 : Y - H,
               W, H, fontId, xcenter, ycenter, transparent ? 3 : 1, label);
}

void drawLabel(Chart *obj, int nx, int precx, int ny, int precy,
               const char *xsuf = "", const char *ysuf = "", bool draw_ymin = true)
{
    const static int fontsize = 8, fontspace = 4;
    const int times[5] = {1, 10, 100, 1000, 10000};
    char fmt[20], label[20];
    if (precy >= 0)
        sprintf(fmt, "%%d.%%0%dd%%s", precy);
    for (int i = draw_ymin ? 0 : 1; i <= ny; i++)
    {
        float y = mapChart(i, 0, ny, obj->ymin, obj->ymax);
        int Y = mapChart(i, 0, ny, obj->Ymax, obj->Ymin);
        if (precy >= 0)
        {
            sprintf(label, fmt, (int)(y), (int)(y * times[precy]) % times[precy], ysuf);
            text(obj, obj->Xmin - fontspace, Y - (i == 0 ? 3 : 0), fontsize * strlen(label), fontsize * 2, 2, 1, label, false);
        }
        HMI_printf("line %d,%d,%d,%d,%s" hexEND, obj->Xmin, Y, obj->Xmax - 1, Y, "GRAY");
    }
    if (precx >= 0)
        sprintf(fmt, "%%d.%%0%dd%%s", precx);
    for (int i = 0; i <= nx; i++)
    {
        float x = mapChart(i, 0, nx, obj->xmin, obj->xmax);
        int X = mapChart(i, 0, nx, obj->Xmin, obj->Xmax);
        if (precx >= 0)
        {
            sprintf(label, fmt, precx, xsuf);
            text(obj, X, obj->Ymax + fontspace, fontsize * strlen(label), fontsize * 2, 0, 1, label, false);
        }
        HMI_printf("line %d,%d,%d,%d,%s" hexEND, X, obj->Ymin, X, obj->Ymax - 1, "GRAY");
    }
}

/////////////// 基本图形
// 绘制一条曲线
template<class T>
void plot(Chart *obj, T y[], int n, const char *color)
{
    int w = obj->Xmax - obj->Xmin;
    int step = n > w? n/w : 1;
    int lastX = obj->Xmin, lastY = mapChart(y[0], obj->ymin, obj->ymax, obj->Ymax, obj->Ymin);
    //HMI_printf("ref_stop" hexENND);
    for (int i = 1; i < n; i += step)
    {
        T yy = 0;
        for(int j = 0; j < step; j++)  yy += y[i+j];
        yy /= step;
        int X = mapChart(i, 0, n, obj->Xmin, obj->Xmax), Y = mapChart(yy, obj->ymin, obj->ymax, obj->Ymax, obj->Ymin);
        HMI_printf("line %d,%d,%d,%d,%s" hexEND, lastX, lastY, X, Y, color);
        lastX = X;
        lastY = Y;
    }
    //HMI_printf("ref_star" hexENND);
}


// 画实心圆点
void point(Chart *obj, float x, float y, int r, const char *color)
{
    int X = mapChart(x, obj->xmin, obj->xmax, obj->Xmin, obj->Xmax);
    int Y = mapChart(y, obj->ymin, obj->ymax, obj->Ymax, obj->Ymin);
    HMI_printf("cirs %d,%d,%d,%s" hexEND, X, Y, r, color);
}

// 画一条线
void line(Chart *obj, float x1, float y1, float x2, float y2, const char *color)
{
    int X1 = mapChart(x1, obj->xmin, obj->xmax, obj->Xmin, obj->Xmax);
    int X2 = mapChart(x2, obj->xmin, obj->xmax, obj->Xmin, obj->Xmax);
    int Y1 = mapChart(y1, obj->ymin, obj->ymax, obj->Ymax, obj->Ymin);
    int Y2 = mapChart(y2, obj->ymin, obj->ymax, obj->Ymax, obj->Ymin);
    HMI_printf("line %d,%d,%d,%d,%s" hexEND, X1, Y1, X2, Y2, color);
}

// 添加一个点(可自动连线)
void plot(Chart *obj, float x, float y, const char *color,
          int r = 0, bool connect = true)
{
    if (connect && !isnan(obj->lastx))
        line(obj, obj->lastx, obj->lasty, x, y, color); // 自动连线
    if (r > 0)
        point(obj, x, y, r, color); // 画实心圆点
    obj->lastx = x;
    obj->lasty = y;
}
