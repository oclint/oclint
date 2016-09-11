#include "oclint/Analytics.h"

#ifndef COUNTLY_ANALYTICS

void oclint::Analytics::send() {}

#else

#include <countly/Countly.h>

using namespace oclint;

void Analytics::send()
{
  countly::Countly cntly;
  cntly.start("countly.ryuichisaito.com",
    "873c792b2ead515f27f0ccba01a976ae9a4cc425");
  cntly.recordEvent("DevEvent");
  cntly.suspend();
}

#endif
