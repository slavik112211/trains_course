#include <main.h>
#include <timer.h>

int main(int argc, char* argv[]) {
    V timerStruct timer = {
        .ticksCounter = 0, .previousTicksCounter = 0xffffffff,
        .msFromEpoch = 0, .timePrinted = 0
    };
    startTimer();

    FOREVER {
        processTime(&timer);
    }
    return 0;
}
