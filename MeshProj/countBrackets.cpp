/*
(c) 2013 Nicolaus Anderson
License: zlib
*/

#include "CountBrackets.h"

#pragma once

void util::countBrackets(
			InputPort* port,
			irr::c8 openBrak,
			irr::c8 closeBrak,
			bool firstAlreadyFound )
{
	irr::core::stringc tk; // token
	irr::s32 count = 1; // remaining brackets to close

	if ( !firstAlreadyFound )
	{
		do {
			tk = port->getNextToken();
			if ( tk == "END" ) return;

		} while ( tk[0] != openBrak; )
	}

	do {
		tk = port->getNextToken();
		if ( tk == "END" ) return;

		if ( tk[0] == openBrak )
			count++;

		if ( tk[0] == closeBrak )
			count--;

	} while ( count > 0 );
}
