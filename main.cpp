/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "robotInterface.h"

using namespace robohan;

float pwma_value = 0.f;
float pwmb_value = 0.f;

const uint8_t endMsg = '\n';
const uint8_t startMsg = 's';
uint32_t recv_data_size = 0;
uint8_t buffer[256] = {};
float twistData[3] = {};

const float two_wheel_distance_m = 0.167f;
const float wheel_diamiter_m = 0.035f;
const float max_omega = 100.f;

PwmOut pwma(A6);
DigitalOut ain1(A5);
DigitalOut ain2(A4);
PwmOut pwmb(A3);
DigitalOut bin1(A2);
DigitalOut bin2(A1);
UnbufferedSerial serial(USBTX, USBRX, 9600);

void receiveData();
void rotate(bool isA, float power);
namespace myfunc
{
    uint32_t printf(const char *str, ...);
}

int main()
{
    serial.attach(receiveData);
    pwma.period(0.0001);
    pwmb.period(0.0001);
    while (1)
    {
        const float V = sqrt(pow(twistData[0], 2) + pow(twistData[1], 2));
        const float omega = twistData[2];
        const float omega_L = (V - omega * (two_wheel_distance_m * 0.5f)) / wheel_diamiter_m;
        const float omega_R = (V + omega * (two_wheel_distance_m * 0.5f)) / wheel_diamiter_m;
        myfunc::printf("omega_L : %f omega_R : %f\r\n", omega_L, omega_R);
        rotate(true, omega_L / max_omega);
        rotate(false, omega_R / max_omega);
        ThisThread::sleep_for(10ms);
    }
}

void rotate(bool isA, float power)
{
    if (isA)
    {
        if (power < 0)
        {
            ain1 = 0;
            ain2 = 1;
            pwma = -power;
        }
        else
        {
            ain1 = 1;
            ain2 = 0;
            pwma = power;
        }
    }
    else
    {
        if (power < 0)
        {
            bin1 = 0;
            bin2 = 1;
            pwmb = -power;
        }
        else
        {
            bin1 = 1;
            bin2 = 0;
            pwmb = power;
        }
    }
}

uint32_t myfunc::printf(const char *str, ...)
{
    va_list args;
    int n;
    static char strbuf[1024];

    va_start(args, str);
    n = vsnprintf(strbuf, sizeof(strbuf), str, args);
    va_end(args);
    strbuf[n] = '\0';

    serial.write(strbuf, n + 1);
    return n + 1;
}

void receiveData()
{
    int recv_data = serial.read(&buffer[recv_data_size], sizeof(uint8_t));

    if (recv_data > 0)
    {
        recv_data_size += recv_data;
        if (recv_data_size >= 256)
        {
            recv_data_size = 0;
        }
        else if (buffer[recv_data_size - 1] == endMsg)
        {
            if (buffer[0] == startMsg)
            {
                const msgs msgType = static_cast<msgs>(buffer[1]);
                const uint8_t arraySize = buffer[2];

                switch (msgType)
                {
                case msgs::twist:
                {
                    if (recv_data_size == arraySize * sizeof(float) + 4)
                    {
                        for (int i = 0; i < arraySize; i++)
                        {
                            for (int i = 0; i < arraySize; i++)
                            {
                                twistData[i] = *(float *)(&buffer[i * sizeof(float) + 3]);
                            }
                        }
                    }
                    else
                    {
                    }
                    break;
                }
                case msgs::command:
                {
                    if (recv_data_size == arraySize * sizeof(float) + 4)
                    {

                        for (int i = 0; i < arraySize; i++)
                        {
                            // memcpy(&commandData[i], &buffer[i * sizeof(float) + 3], sizeof(float));
                        }
                    }
                    else
                    {
                    }
                    break;
                }
                default:
                    break;
                }
            }
            recv_data_size = 0;
        }
    }
}