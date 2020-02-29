/*
(c) 2013 Nicolaus Anderson
License: zlib
*/

#include "InputPort.h"

#pragma once

namespace util
{

//! Count brackets
/* Attempts to find the closing bracket. It keeps count of the number of brackets that have
been openned and closed. */
void countBrackets( InputPort* port, irr::c8 openBrak, irr::c8 closeBrak, bool firstAlreadyFound=false );

}