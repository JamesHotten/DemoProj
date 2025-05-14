#include <chart.h>
#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "uart.h"
#include "adc.h"

#define FloatInt(x)  (int)(x)
#define FloatFrac(x) (int)((long long)(abs(x) * 1000000LL) % 1000000)


float re[1024], im[1024];
float mag[1024];
float hamming[1024];
void  DFT(int N = 500) {
    const float PIf = 3.1415926535f;
    for (int k = 0; k < N; k++) {
        float r = 0, i = 0;
        for (int n = 0; n < N; n++) {
            r += cosf(2 * PIf / N * k * n) * adcData[n];
            i -= sinf(2 * PIf / N * k * n) * adcData[n];
        }
        re[k]  = r;
        im[k]  = i;
        mag[k] = sqrtf(r * r + i * i);
    }
}


double peak_mag[5];
int    har_id[5];
double har_freq[5];
void   peak_find_basic(float fft_result_mag[], int n, double fs) {
    // calculate the fundamental harmonic, which has the largest power

    for (int i = 0; i < 5; i++) {
        peak_mag[i] = 0.0;
        har_id[i]   = 0;
        har_freq[i] = 0.0;
    }

    for (int i = 2; i < n / 2; i++) {
        if (fft_result_mag[i] > peak_mag[0]) {
            peak_mag[0] = fft_result_mag[i];
            har_id[0]   = i;
        }
    }

    int did = har_id[0] / 2;
    did /= 4; if(did <= 1) did = 1;
    if (did % 2 == 1) { did = did - 1; }

    // calculate the 2~5 harmonic
    for (int k = 1; k < 5; k++) // 1~4 har wave
    {
        har_id[k] = (k + 1) * har_id[0];
        peak_mag[k] =  fft_result_mag[har_id[k]];
        for (int i = (k + 1) * har_id[0] - did / 2; i < (k + 1) * har_id[0] + did / 2 + 1; i++) {
            if (fft_result_mag[i] > peak_mag[k]) {
                peak_mag[k] = fft_result_mag[i];
                har_id[k]   = i;
            }
        }
    }

    for (int i = 0; i < 5; i++) har_freq[i] = har_id[i] / n * fs;

    for (int k = 0; k < 5; k++) {
        double sre = 0, sim = 0;
        int num = 1;
        for(int i = har_id[k] - num; i < har_id[k] + num; i++) {
            if(i > 0 && i < n) {
                sre += re[i];
                sim += im[i];
            }
        }
        // peak_mag[k] = fft_result_mag[i];
        peak_mag[k] = sqrt(sre*sre+sim*sim);
    }

    // for debug display
    //  for(int i=0; i<5; i++)
    //    Serial << "har_freq of harmonic and power of " << i<< " is " << har_freq[i]<<" &
    //    "<<peak_mag[i]<< "\n";
}

double cal_thd(double mag_1, double mag_2, double mag_3, double mag_4, double mag_5) {
    return sqrt(mag_2 * mag_2 + mag_3 * mag_3 + mag_4 * mag_4 + mag_5 * mag_5) / mag_1;
}

double get_diff(double thd1, double thd0) {
    if (thd1 >= thd0)
        return (thd1 - thd0);
    else
        return (thd0 - thd1);
}


// 数据处理
template <class T> void getMinMax(const T arr[], int n, T &min, T &max) {
    min = max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < min) { min = arr[i]; }
        if (arr[i] > max) { max = arr[i]; }
    }
}
template <class T> void getMinMax(const T arr[], int n, T &min, T &max, int &min_id, int &max_id) {
    min = max = arr[0];
    min_id    = 0;
    max_id    = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i] < min) {
            min    = arr[i];
            min_id = i;
        }
        if (arr[i] > max) {
            max    = arr[i];
            max_id = i;
        }
    }
}
int trigger(const uint16_t arr[], int n, uint16_t triggerVal) {
    int startId = 0;
    for (int i = 4; i < n; i++) {
        if (adcData[i - 4] <= triggerVal && adcData[i] >= triggerVal) {
            for (int j = i - 4; j < i; j++) {
                if (adcData[j + 1] >= triggerVal) return j;
            }
            return i;
        }
    }
    return startId;
}


Chart chart1, chart2, chart3;



void draw(int baseFreq, int sampleFreq, int nDFT) {

    // 幅度获取与计算
    peak_find_basic(mag, nDFT, sampleFreq);
    BT_printf("#");
    // float Uo1 = peak_mag[0] / nDFT * 3.333f;
    // HMI_printf("Uo1.txt=\"%d.%06d\"" hexEND, FloatInt(Uo1), FloatFrac(Uo1));
    for (int i = 2; i <= 5; i++) {
        float val = peak_mag[i - 1] / peak_mag[0];
        HMI_printf("Uo%d.txt=\"%d.%06d\"" hexEND, i, FloatInt(val), FloatFrac(val));
        BT_printf("Uo%d/Uo1=%d.%06d\n", i, FloatInt(val), FloatFrac(val));
    }
    float THD = cal_thd(peak_mag[0], peak_mag[1], peak_mag[2], peak_mag[3], peak_mag[4]) * 100;
    HMI_printf("THD.txt=\"%d.%06d\"" hexEND, FloatInt(THD), FloatFrac(THD));
    BT_printf("THD=%d.%06d\n", FloatInt(THD), FloatFrac(THD));
    BT_printf("!");

    double minMag, maxMag;
    getMinMax(peak_mag, 5, minMag, maxMag);


    BT_printf("B%d!", baseFreq);
    BT_printf("S%d!", sampleFreq);

    HMI_printf("ref_stop" hexEND);

    clf(&chart2);
    axis(&chart2, 1, nDFT / 2, 0, maxMag);
    drawLabel(&chart2, 10, -1, 4, -1);
    plot(&chart2, mag + 1, nDFT / 2 - 1, "BLUE");

    clf(&chart3);
    axis(&chart3, 1, nDFT / 2, -M_PI, M_PI);
    drawLabel(&chart3, 1, -1, 4, -1);
    for (int i = 1; i < nDFT / 2; i++) {
        if (har_id[0] != 0 && i % har_id[0] == 0) {
            float phase = atan2(im[i], re[i]);
            line(&chart3, i, 0, i, phase, "RED");
        }
    }

    int har_len = nDFT / har_id[0];
    if (sampleFreq * 10 <= 1000 * 1000) {
        sampleFreq *= 10;
        har_len *= 10;
        StartAdcAMP1(sampleFreq, nDFT);
        StartAdcAMP1(sampleFreq, nDFT);
    } else if (sampleFreq * 5 <= 1000 * 1000) {
        sampleFreq *= 5;
        har_len *= 5;
        StartAdcAMP1(sampleFreq, nDFT);
        StartAdcAMP1(sampleFreq, nDFT);
    } else if (sampleFreq * 2 <= 1000 * 1000) {
        sampleFreq *= 2;
        har_len *= 2;
        StartAdcAMP1(sampleFreq, nDFT);
        StartAdcAMP1(sampleFreq, nDFT);
    }
    // 波形显示
    uint16_t adcMin, adcMax;
    getMinMax(adcData, nDFT, adcMin, adcMax);
    int startId = trigger(adcData, nDFT / 2, (adcMin + adcMax) / 2);

    clf(&chart1);
    axis(&chart1, 0, har_len, adcMin, adcMax);
    drawLabel(&chart1, 10, -1, 4, -1);
    plot(&chart1, adcData + startId, har_len, "BLUE");

    HMI_printf("ref_star" hexEND);


    BT_printf("W");
    for (int i = startId; i < startId + har_len; i++) { BT_printf(i == startId ? "%d" : ",%d", adcData[i]); }
    BT_printf("!");
}






int getBaseFreq2() {
    int   calcN = 500;
    float magMin, magMax;
    int   magMinId, magMaxId;
    int   sampleFreq = 250 * 1000;
    StartAdcAMP1(sampleFreq, calcN);
    StartAdcAMP1(sampleFreq, calcN);
    DFT(calcN);
    getMinMax(mag + 1, calcN / 2 - 1, magMin, magMax, magMinId, magMaxId);
    magMinId += 1;
    magMaxId += 1;
    int calc1 = (long long)magMaxId * sampleFreq / calcN;
    UART_printf("%% calc1 %d\n", calc1);
    if (calc1 >= 20 * 1000) return calc1;

    sampleFreq = 50 * 1000;
    StartAdcAMP1(sampleFreq, calcN);
    StartAdcAMP1(sampleFreq, calcN);
    DFT(calcN);
    getMinMax(mag + 1, calcN / 2 - 1, magMin, magMax, magMinId, magMaxId);
    magMinId += 1;
    magMaxId += 1;
    int calc2 = (long long)magMaxId * sampleFreq / calcN;
    UART_printf("%% calc2 %d\n", calc2);
    //    if (calc2 >= 10 * 1000)
    return calc2;
}


void loop() {
    static bool first = true;
    if (first) {
        StartAdcAMP1(250*1000, 500);
        StartAdcAMP1(250*1000, 500);
        StartAdcAMP1(250*1000, 500);
        StartAdcAMP1(250*1000, 500);
    }

    int baseFreq = getBaseFreq2();
    UART_printf("%%baseFreq=%d\n", baseFreq);
    HMI_printf("f.val=%d" hexEND, (baseFreq + 500) / 1000);

    int nDFT       = 500;           // 499; // 523;
    int sampleFreq = baseFreq * 20; // if(baseFreq >= 33*1000) sampleFreq = baseFreq * 10;

//    if ((baseFreq > 50 * 1000 && (baseFreq / 1000) % 2 == 1) ||
//        (20 * 1000 <= baseFreq && baseFreq < 50 * 1000)) // 特殊频率处理
//    {
//        nDFT = 1000;
//    }
//    if(baseFreq == 20*1000 || baseFreq == 100*1000) {
//        nDFT = 503;
//    }

    UART_printf("nDFT = %d\n", nDFT);

    StartAdcAMP1(sampleFreq, nDFT);
    StartAdcAMP1(sampleFreq, nDFT);
    DFT(nDFT);
    draw(baseFreq, sampleFreq, nDFT);

    UART_printf("%% \n");
}


void main(void) {
    MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
    UART_Configure();
    HMI_Configure();
    BT_Configure();
    ConfigureAdc();
    FPUEnable();
    FPULazyStackingEnable();

    HMI_printf("rest" hexEND); // 刷掉无用数据
    HMI_printf("rest" hexEND);
    initChart(&chart1, "wave", 284, 5, 189, 99);
    initChart(&chart2, "freq", 284, 111, 189, 99);
    initChart(&chart3, "phase", 284, 217, 189, 99);

    for (;;) loop();
}
