#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>


void print_wall_time(const struct timespec* wall, int tick) 
{
    time_t ts = wall->tv_sec;
    struct tm ti = *localtime(&ts);
    int ms = wall->tv_nsec / (1000*1000);
    printf("walltime: %d-%02d-%02d %02d:%02d:%02d.%03d, tick %d\n", ti.tm_year+1900, ti.tm_mon+1, ti.tm_mday, 
        ti.tm_hour, ti.tm_min, ti.tm_sec, ms, tick);
}

void print_elapsed_time(const struct timespec* start, const struct timespec* end, int tick)
{
    int64_t ts1 = start->tv_sec * 1000 + start->tv_nsec / (1000*1000);
    int64_t ts2 = end->tv_sec * 1000 + end->tv_nsec / (1000*1000);
    printf("cost %d ms, tick %d\n", int(ts2 - ts1), tick);
}

void print_time_res()
{
    struct timespec res;
    clock_getres(CLOCK_REALTIME, &res);
    printf("walltime precision: %lds %ldus\n", res.tv_sec, res.tv_nsec);
    clock_getres(CLOCK_MONOTONIC, &res);
    printf("monotime precision: %lds %ldus\n", res.tv_sec, res.tv_nsec);
}

int main(int argc, char* argv[])
{
    int tick = 0;
    int interval = 1000; // ms
    if (argc > 1) 
    {
        int ms = atoi(argv[1]);
        if (ms != 0) 
        {
            if (ms < 0) 
            {
                ms = -ms;
            }
            interval = ms;
        }
    }
    
    print_time_res();
    
    while(true)
    {
        tick++;
        
        struct timespec start, end;
        struct timespec wall;
        clock_gettime(CLOCK_REALTIME, &wall);
        clock_gettime(CLOCK_MONOTONIC, &start);

        usleep(interval*1000); // 1s
        
        print_wall_time(&wall, tick);
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        print_elapsed_time(&start, &end, tick);
    }

    return 0;
}

