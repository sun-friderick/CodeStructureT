#include <stdio.h>

#include "monitorTest.h"
#include "Monitor.h"


extern int log_monitor_init();

int main ()
{
    log_monitor_init();
    printf("===========================log_monitor_init\n");
    return 0;
}




