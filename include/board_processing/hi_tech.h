#ifndef _HI_TECH_H
#define _HI_TECH_H

// YOU MUST DEFINE A LASER BEFORE INCLUDING hi_tech.h
// Example: #define STD_824S1

//#include "tchar.h"
#include "opt_str.h"
#include "edger_set.h"

//#include "edgerset.h"

#if ((!STD_824S1) && (!STD_JS50))
#define	HT_NO_DATA					-32768
#else
#define	HT_NO_DATA					0
#endif

#define NO_LZ_TEST_DATA					32768

#define	HT_CLIENT_TO_SERVER_SIZE		3000000		// 3Mb covers size of all data going to engine 

#if (STD_3X00 || STD_824S1 || STD_JS50)
#define HT_SERVER_TO_CLIENT_SIZE	70000000	// 70Mb reserved for 3X00 systems
#elif STD_JOESCAN
#define HT_SERVER_TO_CLIENT_SIZE	25000000	// 20Mb reserved for joescan
#else
#define HT_SERVER_TO_CLIENT_SIZE	8000000		// 8Mb reserved for others
#endif

#define HT_NO_SOLUTION			-1
#define HT_UNDEFINED			-1

// define the structure used to hold the shift start time information for use
// by the tally database operations
#define MAX_NUM_SHIFTS 3
#define MAX_NUM_BREAKS 10
#define SHIFT1_START 360
#define SHIFT1_STOP 1080

#if (STD_JS50)
//#define LINE_LZ_MAX_POINTS			1457	// FULL	// maximum number of JS50 points available per head.
#define LINE_LZ_MAX_POINTS			728	// Half	// maximum number of JS50 points available per head.
#else
#define LINE_LZ_MAX_POINTS			108
#endif

#define LINE_LZ_MAX_HEADS			28		// Maximum value expected for any system.

#define LINE_LZ_CNTS_PER_LUG		MAX_NUM_CNTS_PER_LUG

#define PP_BUFFER					2

#if (STD_JS50)
#define MAX_CAMERAS_PER_HEAD		2
#else
#define MAX_CAMERAS_PER_HEAD		1
#endif

typedef struct
{
	BOOL	bBreakEnabled[ 7 ][ MAX_NUM_BREAKS ];
	int		aiStartMinute[ 7 ][ MAX_NUM_BREAKS ];
	int		aiStopMinute[ 7 ][ MAX_NUM_BREAKS ];
} SBreakInfo;

typedef struct
{
	int				iFuncCode;
	BOOL			bIsManualMode;
	int				iCurrManualShiftNumber;
	int				iPrevManualShiftNumber;
	DATE			dtManualShiftStartTime;
	DATE			dtPrevManualShiftStartTime;
	DATE			dtPrevManualShiftEndTime;
	int				iShiftNumber[ 7 ][ MAX_NUM_SHIFTS ];
	int				aiStartMinute[ 7 ][ MAX_NUM_SHIFTS ];
	int				aiStopMinute[ 7 ][ MAX_NUM_SHIFTS ];
	SBreakInfo		sBreakInfo;
} SShiftInfo_UpDown; //used for upload / download ONLY.  Must convert DATE vars to COleDateTime::m_Date

// in the following structure, the iShiftNumber is to be interpreted as follows:
//
// for each day, there are MAX_NUM_SHIFTS values for iShiftNumber
//
//		iShiftNumber[ 0 ] contains the number of the 1st shift of the day
//		iShiftNumber[ 1 ] contains the number of the 2nd shift of the day
//		iShiftNumber[ 2 ] contains the number of the 3rd shift of the day
//		etc.
//
// if any of the iShiftNumber[] values contain zero for the shift number, this 
// indicates that there are no more shifts defined for the day; e.g.:
//
//		iShiftNumber[ 0 ] == 0   =>   no shifts defined for the day
//		iShiftNumber[ 1 ] == 0   =>   only one shift defined for the day
//		iShiftNumber[ 2 ] == 0   =>   only two shifts defined for the day
//		etc.
//
// the aiStartMinute and aiStopMinute values at index 'i' are the start and stop
// times for the shift number found in iShiftNumber[ i ];  e.g.
//
//		iShiftNumber[ 0 ] == 1   =>   aiStartMinute[ 0 ] contains start time for shift 1
//									  aiStopMinute[ 0 ] contains start time for shift 1
//
//		iShiftNumber[ 0 ] == 3   =>   aiStartMinute[ 0 ] contains start time for shift 3
//									  aiStopMinute[ 0 ] contains start time for shift 3
//
// If bIsManualMode is TRUE, we are currently in manual override mode.  If this is the
// case, then iCurrManualShiftNumber is the shift number that should be used when creating
// database entries.  If iCurrManualShiftNumber is 0, then the manually set shift has 
// been stopped; in this case, iPrevManualShiftNumber contains the number of the shift
// that was last run in manual mode, and is used by OptWin when generating reports.
// The value in dtManualShiftStartTime is the time that the manually set shift specified 
// in iCurrManualShiftNumber started, while dtPrevManualShiftStartTime is the time that
// the manually set shift specified in iPrevManualShiftNumber started and dtPrevManualShiftEndTime
// is the time it ended.  Note that there is no variable for the end time of the current
// manual shift since that value is unknown until the user takes action to stop the shift.
/*
typedef struct
{
	int				iFuncCode;
	BOOL			bIsManualMode;
	int				iCurrManualShiftNumber;
	int				iPrevManualShiftNumber;
	COleDateTime	dtManualShiftStartTime;
	COleDateTime	dtPrevManualShiftStartTime;
	COleDateTime	dtPrevManualShiftEndTime;
	int				iShiftNumber[ 7 ][ MAX_NUM_SHIFTS ];
	int				aiStartMinute[ 7 ][ MAX_NUM_SHIFTS ];
	int				aiStopMinute[ 7 ][ MAX_NUM_SHIFTS ];
} SShiftInfo;
*/

// this version of the shift information structure assembles the values in the 
// SShiftInfo structure so they are easily transferred to a dialog that displays
// the shift information
//
// in this structure, the index into the second dimension of the 2-d array directly
// corresponds to the shift number; e.g.
//
//		bShiftEnabled[ 0 ] contains the enabled flag for shift 1
//		bShiftEnabled[ 1 ] contains the enabled flag for shift 2
//		bShiftEnabled[ 2 ] contains the enabled flag for shift 3
//
// in essence, SShiftInfo arranges the start and stop times in chronological order
// rather than "shift number" order, while SShiftInfoForDialog arranges the start
// and stop times in "shift number" order
//
// to translate the data between the two formats, use the following functions:
//
//		LoadShiftInfoFromDialogData() to go from SShiftInfoForDialog to SShiftInfo
//		LoadDialogDataFromShiftInfo() to go from SShiftInfo to SShiftInfoForDialog

typedef struct
{
	BOOL	bShiftEnabled[ 7 ][ MAX_NUM_SHIFTS ];
	int		aiStartMinute[ 7 ][ MAX_NUM_SHIFTS ];
	int		aiStopMinute[ 7 ][ MAX_NUM_SHIFTS ];
} SShiftInfoForDialog;

typedef struct
{
	BOOL	bBreakEnabled[ 7 ][ MAX_NUM_BREAKS ];
	int		aiStartMinute[ 7 ][ MAX_NUM_BREAKS ];
	int		aiStopMinute[ 7 ][ MAX_NUM_BREAKS ];
} SBreakInfoForDialog;

// structures detailing the data that is saved for each board
// when implementing a shift simulation
struct SRawLaserData
{
#if (!STD_JOESCAN)
    RAW_LASER_DATA		bot_raw_laser_data	;
    RAW_LASER_DATA		top_raw_laser_data	;
#else
    XYPOINT_DATA sorted_xlat_data;
    LAST_BOARD_DATA last_board_data;
#endif
};


#if 1
// a simple x,y point with Brightness data (if available)
struct LineLzPoint
{
    unsigned short x;	// X inch units  (0 to 65,535)  - Range value
    short y;			// Y inch units  (-32,768 to 32,767)  - along length; 0=center of head.
#if (STD_JS50)
    char brightness;	// 0 to 255 gray scale image
#endif
};
#else
// a simple x,y point with Brightness data (if available)
// test as int; may allow faster processing on x64 machine than shorts and char?
//	Error	LNK1248	image size(B05B0000) exceeds maximum allowable size(80000000)
struct LineLzPoint
{
    int x;			// X inch units  (0 to 65,535)  - Range value
    int y;			// Y inch units  (-32,768 to 32,767)  - along length; 0=center of head.
    int brightness;	// 0 to 255 gray scale image
};
#endif

struct LineLzStorageStruct
{
#if (STD_JS50)
    int camera;
#endif
    unsigned int encoder;
    int numPoints;
    LineLzPoint p[LINE_LZ_MAX_POINTS];	// x, y
};


struct H_BufferStruct
{
    LineLzStorageStruct H_Buffer[LINE_LZ_MAX_HEADS / 2][LINE_LZ_CNTS_PER_LUG];
};


typedef struct DefectRectangleTag
{
	int left;
	int top;
	int right;
	int bottom;
} DefectRectangle;

typedef struct VisionDefectTag
{
	int id;
	int type;
	DefectRectangle position;	/* pixel position */
	double area;				/* square inches */
} VISION_DEFECT;


#define MAX_NUM_VISION_DEFECTS		500
#define VISION_NUM_DEFECT_TYPES		(7)
typedef struct {
	int iNumDefects;
	VISION_DEFECT sDefects[MAX_NUM_VISION_DEFECTS];
} VISION_DEFECT_STRUCT;

typedef struct {
	VISION_DEFECT_STRUCT	sTopDefectData;
	VISION_DEFECT_STRUCT	sBottomDefectData;
} VISION_DEFECT_DATA;


//-----------------------------------------------------------------------------
// TODO: Make sure folowing expressions are immune by alignment issues,
// because the presence of History Items, throws alignemnt off in lastboard.
// Revisit this code, once you check operation w/other laser families
//-----------------------------------------------------------------------------

// slm: 06-25-14 Changed declaration to works for all the configuration + and clean-up
// This was required because some memory pads insert by compiler, at times, in between terms of the structure
// This resulted in miss-calculating the expression for SIM_BOARD_SIZE
// Now we mimic OptWin operation where there we use only sizeof(SBoardData)
// sjf: 09/2017 moved to Hi-Tech.h so optwin and optimizer use the same struct format.  
typedef struct SBoardData_Tag
{
#if (!STD_JOESCAN)
	RAW_LASER_DATA		bot_raw_laser_data;
	RAW_LASER_DATA		top_raw_laser_data;
#endif
    
	LAST_BOARD_DATA		last_board_data;
	BOOLEAN_PF_MAP		pf_map;
    
#if (!EDG_56x || VISION_ENABLED)	// always present in version 57x and later.
	VISION_DEFECT_DATA	 vs_defect_data;  // top and bottom image defect data
#endif
    
#if( VISION_ENABLED )
	//CS3300_RANGE_DATA	 cs_range_data		;  // top and bottom range data
	//CS3300_IMAGE_DATA	 cs_image_data;  // top and bottom image data
	VISION_IMAGE_DATA	 vs_image_data;  // top and bottom image data
    
#endif
    
    // slm: 03-11-16 Addition
#if (STD_824S1 || STD_JS50)
	H_BufferStruct H_Top_Raw;
	H_BufferStruct H_Bot_Raw;
#endif
    
} SBoardData;


#define SIM_BOARD_SIZE	sizeof(SBoardData)


enum SimulationModes 
{ 
	SHIFT_SIMULATION = 1,
	BOARDXX_SIMULATION,
	BOARD_ZIP_SIMULATION,
	HERMARY_RAW_SIM,				// for debug/baxley use only
	NUM_SIMULATION_MODES			// ALWAYS HAVE THIS LAST IN THE ENUMERATION!!
};

typedef struct
{
	int	iPrimaryIndex;
	int	iSecondaryIndex;
} MASTER_LIST;

#endif