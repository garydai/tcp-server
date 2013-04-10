#ifndef __SYS_H
#define __SYS_H

#include "defs.h"

// system globals
class CORE_API Sys
{
public:
    static bool Initialize();
    static bool Deintitialize();
    static void Run();
};

#endif