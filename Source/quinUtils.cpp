/*
 ==============================================================================
 
 quinUtils.cpp
 Created: 8 Sep 2019 11:25:11pm
 Author:  Quin Scacheri
 
 ==============================================================================
 */

#include "quinUtils.h"
namespace qtils
{
    float map(float val, float low1, float high1, float low2, float high2)
    {
        return ((val - low1) / (high1 - low1)) * (high2 - low2) + low2;
    }
}
