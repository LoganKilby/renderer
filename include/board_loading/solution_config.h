// This document allows mulitiple solution configurations while still maintaining
// functionality of existing code that references these #defines.
// The type of system is defined in the PreProcessor defines.
// Based on the system type, the following settings are used.
// This allows for different stack usage and other requirements needed when
// "one size" doesn't fit all.

// the program uses some of these #defines in regular "if" statements
// So they must be defined to a value. even if not this type of system.

// these items were orginally defined in opt_str.h but moved here to keep Opt_str.h
// from being complicated by these conditions.

/********************  B800  ***********************/
#ifdef STD_B8

#define STD_M6                         0  // Standard M6 (up to 26ft)						
#define STD_B8						   1  // Standard B8 Ethernet System
#define STD_JOESCAN					   0  // (JoeScan Linear Edger)

#define STD_3X00					   0  // CS3300 or CS3100

#define STD_JS50					   0   // JoeScan Line Scan Lasers
#define STD_824S1					   0   // Profile only version of Hermary Lasers
#define SCANNER_ANGLED				   0	// 1 if scanning at angle;  0 if scanning at 90 degrees.


/********************  M6  ***********************/
#elif STD_M6

#define STD_M6                         1  // Standard M6 (up to 26ft)						
#define STD_B8						   0  // Standard B8 Ethernet System
#define STD_JOESCAN					   0  // (JoeScan Linear Edger)

#define STD_3X00					   0  // CS3300 or CS3100

#define STD_JS50					   0   // JoeScan Line Scan Lasers
#define STD_824S1					   0   // Profile only version of Hermary Lasers
#define SCANNER_ANGLED				   0	// 1 if scanning at angle;  0 if scanning at 90 degrees.


/********************  JoeScan  ***********************/
#elif STD_JOESCAN

#define STD_M6                         0  // Standard M6 (up to 26ft)						
#define STD_B8						   0  // Standard B8 Ethernet System
#define STD_JOESCAN					   1  // (JoeScan Linear Edger)

#define STD_3X00					   0  // CS3300 or CS3100

#define STD_JS50					   0   // JoeScan Line Scan Lasers
#define STD_824S1					   0   // Profile only version of Hermary Lasers
#define SCANNER_ANGLED				   0	// 1 if scanning at angle;  0 if scanning at 90 degrees.


/********************  CS31xx  ***********************/
#elif STD_31xx

#define STD_M6                         0  // Standard M6 (up to 26ft)						
#define STD_B8						   0  // Standard B8 Ethernet System
#define STD_JOESCAN					   0  // (JoeScan Linear Edger)

#define STD_3X00					   1  // CS3300 or CS3100

#define STD_JS50					   0   // JoeScan Line Scan Lasers
#define STD_824S1					   0   // Profile only version of Hermary Lasers
#define SCANNER_ANGLED				   0	// 1 if scanning at angle;  0 if scanning at 90 degrees.


/********************  CS33xx  ***********************/
#elif STD_33xx

#define STD_M6                         0  // Standard M6 (up to 26ft)						
#define STD_B8						   0  // Standard B8 Ethernet System
#define STD_JOESCAN					   0  // (JoeScan Linear Edger)

#define STD_3X00					   1  // CS3300 or CS3100

#define STD_JS50					   0   // JoeScan Line Scan Lasers
#define STD_824S1					   0   // Profile only version of Hermary Lasers
#define SCANNER_ANGLED				   0	// 1 if scanning at angle;  0 if scanning at 90 degrees.


/********************  STD_824S1  ***********************/
#elif STD_824S1

#define STD_M6                         0  // Standard M6 (up to 26ft)						
#define STD_B8						   0  // Standard B8 Ethernet System
#define STD_JOESCAN					   0  // (JoeScan Linear Edger)

#define STD_3X00					   0  // CS3300 or CS3100

#define STD_JS50					   0   // JoeScan Line Scan Lasers
#define STD_824S1					   1   // Profile only version of Hermary Lasers
#define SCANNER_ANGLED				   0	// 1 if scanning at angle;  0 if scanning at 90 degrees.


/********************  STD_JS50  ***********************/
#elif STD_JS50

#define STD_M6                         0  // Standard M6 (up to 26ft)						
#define STD_B8						   0  // Standard B8 Ethernet System
#define STD_JOESCAN					   0  // (JoeScan Linear Edger)

#define STD_3X00					   0  // CS3300 or CS3100

#define STD_824S1					   0  // Profile only version of Hermary Lasers
#define SCANNER_ANGLED				   0  // 1 if scanning at angle;  0 if scanning at 90 degrees.

#define STD_JS50					   1  // JoeScan JS50 Model heads.  Allow Profile and vision data

#endif

// add in when doing JS-50
#if defined(VISION_ENABLED) || defined(STD_33xx)
#define VISION_ENABLED				   1  // Set to one when using CS3300 heads or other Vision Hardware
#else
#define VISION_ENABLED				   0  // else zero for lasers only
#endif
