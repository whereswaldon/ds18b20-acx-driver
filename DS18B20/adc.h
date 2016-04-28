/*
 * adc.h
 *
 * Created: 4/27/2016 10:43:16 PM
 *  Author: waldonck
 */ 


#ifndef ADC_H_
#define ADC_H_

#define ADC0	0x0
#define ADC1	0x1
#define ADC2	0x2
#define ADC3	0x3
#define ADC4	0x4
#define ADC5	0x5
#define ADC6	0x6
#define ADC7	0x7
#define ADC8	0x20
#define ADC9	0x21
#define ADC10	0x22
#define ADC11	0x23
#define ADC12	0x24
#define ADC13	0x25
#define ADC14	0x26
#define ADC15	0x27

int ADC_read(byte channel, byte resolution, bool singleEnded);



#endif /* ADC_H_ */