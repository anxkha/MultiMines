// ----------------------------------------------------------------------------
//  Filename: debug.h
//  Author: Lucas Suggs
//
//  Date Created: 10 March 2008
//  Date Modified: 10 March 2008
//
//  This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------
#ifndef _DEBUG_H
#define _DEBUG_H

#include <string>
#include <fstream>
using namespace std;

void InitializeDebug();
void ShutdownDebug();
void PrintDebug( string sLog );

#endif // _DEBUG_H
