#ifndef DIMMER_H
#define DIMMER_H

class Dimmer {

private:
    static int dimmerValue;
    static void setDimmerValue(int value);
    static int getDimmerValue();
    static void up(int target);
    static void down(int target);

public:
    static void init();
    static void full();
    static void off();
    static void setBrightness(int value);
    static void switchMode();

};

#endif
