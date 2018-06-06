#ifndef _KEYCODES_H_
#define _KEYCODES_H_

#include <cstring.h>

/**
 * is this really what it's come to
 *
 * if anybody knows a better way with the Quartz API
 * please submit a pull request
 */
int keyCodeForKeyString(char * keyString)
{
	if (strcmp(keyString, "a") == 0) return 0;
	if (strcmp(keyString, "s") == 0) return 1;
	if (strcmp(keyString, "d") == 0) return 2;
	if (strcmp(keyString, "f") == 0) return 3;
	if (strcmp(keyString, "h") == 0) return 4;
	if (strcmp(keyString, "g") == 0) return 5;
	if (strcmp(keyString, "z") == 0) return 6;
	if (strcmp(keyString, "x") == 0) return 7;
	if (strcmp(keyString, "c") == 0) return 8;
	if (strcmp(keyString, "v") == 0) return 9;
	// what is 10?
	if (strcmp(keyString, "b") == 0) return 11;
	if (strcmp(keyString, "q") == 0) return 12;
	if (strcmp(keyString, "w") == 0) return 13;
	if (strcmp(keyString, "e") == 0) return 14;
	if (strcmp(keyString, "r") == 0) return 15;
	if (strcmp(keyString, "y") == 0) return 16;
	if (strcmp(keyString, "t") == 0) return 17;
	if (strcmp(keyString, "1") == 0) return 18;
	if (strcmp(keyString, "2") == 0) return 19;
	if (strcmp(keyString, "3") == 0) return 20;
	if (strcmp(keyString, "4") == 0) return 21;
	if (strcmp(keyString, "6") == 0) return 22;
	if (strcmp(keyString, "5") == 0) return 23;
	if (strcmp(keyString, "=") == 0) return 24;
	if (strcmp(keyString, "9") == 0) return 25;
	if (strcmp(keyString, "7") == 0) return 26;
	if (strcmp(keyString, "-") == 0) return 27;
	if (strcmp(keyString, "8") == 0) return 28;
	if (strcmp(keyString, "0") == 0) return 29;
	if (strcmp(keyString, "]") == 0) return 30;
	if (strcmp(keyString, "o") == 0) return 31;
	if (strcmp(keyString, "u") == 0) return 32;
	if (strcmp(keyString, "[") == 0) return 33;
	if (strcmp(keyString, "i") == 0) return 34;
	if (strcmp(keyString, "p") == 0) return 35;
	if (strcmp(keyString, "return") == 0) return 36;
	if (strcmp(keyString, "l") == 0) return 37;
	if (strcmp(keyString, "j") == 0) return 38;
	if (strcmp(keyString, "'") == 0) return 39;
	if (strcmp(keyString, "k") == 0) return 40;
	if (strcmp(keyString, ";") == 0) return 41;
	if (strcmp(keyString, "\\") == 0) return 42;
	if (strcmp(keyString, ",") == 0) return 43;
	if (strcmp(keyString, "/") == 0) return 44;
	if (strcmp(keyString, "n") == 0) return 45;
	if (strcmp(keyString, "m") == 0) return 46;
	if (strcmp(keyString, ".") == 0) return 47;
	if (strcmp(keyString, "tab") == 0) return 48;
	if (strcmp(keyString, "space") == 0) return 49;
	if (strcmp(keyString, "`") == 0) return 50;
	if (strcmp(keyString, "delete") == 0) return 51;
	if (strcmp(keyString, "enter") == 0) return 52;
	if (strcmp(keyString, "escape") == 0) return 53;

	// some more missing codes abound, reserved I presume, but it would
	// have been helpful for Apple to have a document with them all listed

	if (strcmp(keyString, ".") == 0) return 65;

	if (strcmp(keyString, "*") == 0) return 67;

	if (strcmp(keyString, "+") == 0) return 69;

	if (strcmp(keyString, "clear") == 0) return 71;

	if (strcmp(keyString, "/") == 0) return 75;
	if (strcmp(keyString, "enter") == 0) return 76;  // numberpad on full kbd

	if (strcmp(keyString, "=") == 0) return 78;
	
	if (strcmp(keyString, "=") == 0) return 81;
	if (strcmp(keyString, "0") == 0) return 82;
	if (strcmp(keyString, "1") == 0) return 83;
	if (strcmp(keyString, "2") == 0) return 84;
	if (strcmp(keyString, "3") == 0) return 85;
	if (strcmp(keyString, "4") == 0) return 86;
	if (strcmp(keyString, "5") == 0) return 87;
	if (strcmp(keyString, "6") == 0) return 88;
	if (strcmp(keyString, "7") == 0) return 89;
	
	if (strcmp(keyString, "8") == 0) return 91;
	if (strcmp(keyString, "9") == 0) return 92;

	if (strcmp(keyString, "f5") == 0) return 96;
	if (strcmp(keyString, "f6") == 0) return 97;
	if (strcmp(keyString, "f7") == 0) return 98;
	if (strcmp(keyString, "f3") == 0) return 99;
	if (strcmp(keyString, "f8") == 0) return 100;
	if (strcmp(keyString, "f9") == 0) return 101;
	
	if (strcmp(keyString, "f11") == 0) return 103;
	
	if (strcmp(keyString, "f13") == 0) return 105;
	
	if (strcmp(keyString, "f14") == 0) return 107;
	
	if (strcmp(keyString, "f10") == 0) return 109;
	
	if (strcmp(keyString, "f12") == 0) return 111;

	if (strcmp(keyString, "f15") == 0) return 113;
	if (strcmp(keyString, "help") == 0) return 114;
	if (strcmp(keyString, "home") == 0) return 115;
	if (strcmp(keyString, "pgup") == 0) return 116;
	if (strcmp(keyString, "delete") == 0) return 117;
	if (strcmp(keyString, "f4") == 0) return 118;
	if (strcmp(keyString, "end") == 0) return 119;
	if (strcmp(keyString, "f2") == 0) return 120;
	if (strcmp(keyString, "pgdn") == 0) return 121;
	if (strcmp(keyString, "f1") == 0) return 122;
	if (strcmp(keyString, "left") == 0) return 123;
	if (strcmp(keyString, "right") == 0) return 124;
	if (strcmp(keyString, "down") == 0) return 125;
	if (strcmp(keyString, "up") == 0) return 126;

  return -1;
}

#endif
