#!/bin/bash

echo \#ifndef Z80_BOARD_BUILDCONFIG
echo \#define Z80_BOARD_BUILDCONFIG
echo \#define VERSION_BASE \"1.0\"
echo \#define GITVER \"`git log --pretty=format:'%h' -n 1`\"
echo \#define VERSION VERSION_BASE \" - \" GITVER

echo \#define BUILDDATETIME \"`date`\"
echo \#endif
echo
