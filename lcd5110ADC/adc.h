#ifndef LCD5110ADC_ADC_H
#define LCD5110ADC_ADC_H

class adc2
{
public:
    adc2();
    unsigned getRead();
private:
    unsigned lastRead;

    void a2Read();
    void initAdc2();
    void stopAdc2();
};

class batLvl
{
public:
    batLvl();
    batLvl(adc2 * adc);
    ~batLvl();
    char getBatLvl();

private:
    char level;
    adc2 * pAdc;
    bool dynAlloc;

    void updateLvl();
    void computeLvl(unsigned * read);
};

#endif /* LCD5110ADC_ADC_H */
