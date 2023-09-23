#include "adc.h"
#include "msp430g2553.h"

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    __bic_SR_register_on_exit(CPUOFF);
}

adc2::adc2()
{
    lastRead = 0;
};

void adc2::a2Read()
{
    initAdc2();
    ADC10CTL0 |= ENC + ADC10SC;         // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);    // LPM0, ADC10_ISR will force exit
    lastRead = ADC10MEM;
    stopAdc2();
}

void adc2::initAdc2()
{
    ADC10CTL1 = INCH_2;     // input A2
    ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE;
    ADC10AE0 |= BIT2;
}

void adc2::stopAdc2()
{
    ADC10CTL0 = 0x00;
    ADC10CTL1 = 0x00;
    ADC10AE0 = 0x00;
}

unsigned adc2::getRead()
{
    a2Read();
    return lastRead;
}



batLvl::batLvl()
{
    pAdc = new adc2;
    dynAlloc = true;
}

batLvl::batLvl(adc2 * adc)
{
    pAdc =  adc;
    dynAlloc = false;
}

batLvl::~batLvl()
{
    if (dynAlloc) delete pAdc;
}

char batLvl::getBatLvl()
{
    updateLvl();
    return level;
}

void batLvl::updateLvl()
{
    unsigned analogRead;
    analogRead = pAdc->getRead();
    computeLvl(&analogRead);
}

void batLvl::computeLvl(unsigned * read)
{
    if (*read <= 623)
    {
        level = '0';
        return;
    }
    (*read) -= 623;
    level = ((*read) * 9 / 400) + '0';
}
