#include <map>

#include "aoc_days.h"
#include "aoc_day.h"
// includes for each day will go here
#include "aoc_day_1.h"
#include "aoc_day_2.h"
#include "aoc_day_3.h"
#include "aoc_day_4.h"
#include "aoc_day_5.h"
#include "aoc_day_6.h"
#include "aoc_day_7.h"
#include "aoc_day_8.h"
#include "aoc_day_9.h"
#include "aoc_day_10.h"
#include "aoc_day_11.h"
#include "aoc_day_12.h"
#include "aoc_day_13.h"
#include "aoc_day_14.h"
#include "aoc_day_15.h"
#include "aoc_day_16.h"
#include "aoc_day_17.h"
#include "aoc_day_18.h"
#include "aoc_day_19.h"
#include "aoc_day_20.h"
#include "aoc_day_21.h"
#include "aoc_day_22.h"
#include "aoc_day_23.h"
#include "aoc_day_24.h"
#include "aoc_day_25.h"

using namespace std;

AocDays::AocDays()
{
    // adding each member to the map goes here
    m_days[1]=new AocDay1();
    m_days[2]=new AocDay2();
    m_days[3]=new AocDay3();
    m_days[4]=new AocDay4();
    m_days[5]=new AocDay5();
    m_days[6]=new AocDay6();
    m_days[7]=new AocDay7();
    m_days[8]=new AocDay8();
    m_days[9]=new AocDay9();
    m_days[10]=new AocDay10();
    m_days[11]=new AocDay11();
    m_days[12]=new AocDay12();
    m_days[13]=new AocDay13();
    m_days[14]=new AocDay14();
    m_days[15]=new AocDay15();
    m_days[16]=new AocDay16();
    m_days[17]=new AocDay17();
    m_days[18]=new AocDay18();
    m_days[19]=new AocDay19();
    m_days[20]=new AocDay20();
    m_days[21]=new AocDay21();
    m_days[22]=new AocDay22();
    m_days[23]=new AocDay23();
    m_days[24]=new AocDay24();
    m_days[25]=new AocDay25();
}

AocDays::~AocDays()
{
    // delete all of the days solutions
    for (map<int, AocDay *>::iterator days_iter = m_days.begin(); days_iter != m_days.end(); ++days_iter)
    {
        delete days_iter->second;
    }
    m_days.clear();
}

AocDay * AocDays::get_day(int day)
{
    return m_days[day];
}

