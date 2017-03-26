// Copyright (C) 2014-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

/* how many days this month have */
int getDaysOfThisMonth()
{
    time_t now = time(NULL);
    tm date = *localtime(&now);
    
    /* first day of this month */
    tm this_month = {};
    this_month.tm_year = date.tm_year;
    this_month.tm_mon = date.tm_mon;
    this_month.tm_mday = 1;
    
    /* first day of last month */
    tm next_month = {};
    this_month.tm_year = date.tm_year + date.tm_mon / 11;
    this_month.tm_mon = (date.tm_mon % 11) + 1;
    this_month.tm_mday = 1;
    
    int elapsed_sec = mktime(&next_month) - mktime(&this_month);
    return elapsed_sec / (60* 60 * 24);
}

/* how many days this year have*/
int getDaysOfYear(int year)
{
    /* first day of January */
    tm january = {};
    january.tm_year = year;
    january.tm_mon = 0;
    january.tm_mday = 1;

    /* last day of December */
    tm december = {};
    december.tm_year = year;
    december.tm_mon = 11;
    december.tm_mday = 31;

    int elapsed_sec = mktime(&december) - mktime(&january);
    return (elapsed_sec / (60 * 60 * 24)) + 1;
}

/* elapsed days between two timestamps */
int getElapsedDay(time_t t1, time_t t2)
{
    assert(t1 > 0 && t2 > 0);
    time_t end = (t1 > t2 ? t1 : t2);
    time_t start = (t1 > t2 ? t2 : t1);
    tm end_date = *localtime(&end);
    tm start_date = *localtime(&start);
    assert(start_date.tm_year <= end_date.tm_year);
    if (end_date.tm_year == start_date.tm_year)
    {
        return end_date.tm_yday - start_date.tm_yday;
    }
    else
    {
        int year_days = getDaysOfYear(start_date.tm_year);
        return (year_days - start_date.tm_yday) + end_date.tm_yday;
    }
}