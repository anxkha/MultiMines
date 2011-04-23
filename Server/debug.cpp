// ----------------------------------------------------------------------------
//  Filename: debug.cpp
//  Author: Lucas Suggs
//
//  Date Created: 10 March 2008
//  Date Modified: 10 March 2008
//
//  This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "debug.h"




// Global declarations.
fstream	g_LogFile;




// ----------------------------------------------------------------------------
//  Name: InitializeDebug
//
//  Desc: Initializes the debug engine and opens the log file.
// ----------------------------------------------------------------------------
void InitializeDebug()
{
	// Open the log file.
	g_LogFile.open( ".\\Data\\server_log.txt", fstream::out );
}




// ----------------------------------------------------------------------------
//  Name: ShutdownDebug
//
//  Desc: Shuts down the debug engine and closes the log file.
// ----------------------------------------------------------------------------
void ShutdownDebug()
{
	// Close the log file.
	if( g_LogFile ) g_LogFile.close();
}




// ----------------------------------------------------------------------------
//  Name: PrintDebug
//
//  Desc: Prints to the log file.
// ----------------------------------------------------------------------------
void PrintDebug( string sLog )
{
	// Print to the log file.
	if( g_LogFile ) g_LogFile << sLog << endl;
}
