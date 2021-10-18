#ifndef _ENUMERATIONS_H
#define _ENUMERATIONS_H

//------------------------------------ WHY -------------------------------------------
// This file was created when I started to clean-up code, so it is better constructed
// and debug is simplified, because (int) values are now automatically expanded to a
// more intuitive Symbol name
// As clean up continues, these file will be expanded as needed
//------------------------------------ FYI -------------------------------------------
// Basically only those (int) items in Opt_str.h and Classes that are used as
// enumerations are replaced with an enumeration Symbol of (int)Type and moved in here
// This way the execution code will still work with an harcoded numerical value and
// a Symbol (new way)
// Also, notice, that is many cases, header files have intended enumerations defined
// as #define(s) - This is legacy of the Standerd C Language
// VC2005 C++ debugger does not convert symbol to value consistently - It does if enum
// At some point more of these definitions will be moved in here, also, after defining
// an enumeration of that name
//------------------------------------------------------------------------------------


// NOTE: the delaration style, new to VC2005 - Type is required if not anonymous
// This way you can distinguish it with another of similar name inside another enum
//------------------------------------------------------------------------------------
// FYI: Type can be any of this...
// short, int, long, long long, signed char, char,
// unsigned short, unsigned int, unsigned long, unsigned long long,
// unsigned char, bool
//------------------------------------------------------------------------------------

// Used to return eREPORT from several new functions, all related to Print Parameters
enum  eREPORT : int {
	REPORT_OK,				// 0:
	REPORT_NO_MEMORY,		// 1:
	REPORT_NO_PRINTER,		// 2:
	REPORT_NO_DC			// 3:
};

// Used to control the Parameters Set Appearance in Reports/Grids
enum eLUMBER_TYPE : int {
	SW_GREEN,				// 0:
	HW_GREEN,				// 1:
	SW_DRY,					// 2:
	HW_DRY					// 3:
};

// Used to qualify Trim saw types and Operation (Reports / Decision Code)
enum eTRIM_TYPE : int {
	TWO_SAW,				// 0:  1 or 2 Saw 
	MULTI_SAW,				// 1:  Multi Saw 
	MINUS_SAW,				// 2:  -1ft Saw 
	PRECUT_SAW				// 3:  -1ft Saw w/ Precut
};

enum eSUM_FACES : int {
	TOGETHER,				// 0:  Sum top and bottom together
	SEPARATE				// 1:  Sum top and bottom separately
};
#endif
