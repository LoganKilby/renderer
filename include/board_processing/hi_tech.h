// hi-tech.h

#ifndef _HI_TECH_H
#define _HI_TECH_H

//#include "tchar.h"
#include "opt_str.h"
//#include "LineLaserStructs.h"

//#include <afxdisp.h>		// for COleDateTime


#if ((!STD_824S1) && (!STD_JS50))
#define	HT_NO_DATA					-32768
#else
#define	HT_NO_DATA					0
#endif

#define NO_LZ_TEST_DATA				 32768

#define	HT_CLIENT_TO_SERVER_SIZE		3000000		// 3Mb covers size of all data going to engine 

#if (VISION_ENABLED)
#define HT_SERVER_TO_CLIENT_SIZE	600000000	// 400Mb reserved for TAV systems
#elif (STD_3X00 || STD_824S1 || STD_JS50)
#define HT_SERVER_TO_CLIENT_SIZE	70000000	// 70Mb reserved for 3X00 systems
#elif STD_JOESCAN
#define HT_SERVER_TO_CLIENT_SIZE	25000000	// 20Mb reserved for joescan
#else
#define HT_SERVER_TO_CLIENT_SIZE	8000000		// 8Mb reserved for others
#endif

#define HT_NO_SOLUTION			-1
#define HT_UNDEFINED			-1

// sjf: 02-12-19	Moved from StdAfx.h to here so both Optwin and Optimizer share the same setting.
//					This value allows the defect struct to remain without vision data.
//					Future intent is to be like edger and evaluate wane defects with cutting units.
//					Defect Struct adds about 32KB, if feature never develops - can consider removing
//					from non-vision builds by setting value equal to status of VISION_ENABLED.
// slm: 07-05-17
// Set to 1 for anything newer than V79x Hermary (which includes the New Scanner Vision development)
// Set to 0 to be compatible to the v79x Hermary (no vision), where no defect data is not in SBoardData
// This means all Configurations prior the Developed Vision new Version
//---------------------------------------------------------------------------------------
// Used to Bridge into Edger Environment from which we import VISION Code
// Note: EDG_V56X does not exist in this environment
// Must match value in Engine
#define NEW_V79X					1

// define the structure used to hold the shift start time information for use
// by the tally database operations
#define MAX_NUM_SHIFTS 3
#define MAX_NUM_BREAKS 10
#define SHIFT1_START 360
#define SHIFT1_STOP 1080

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
	//COleDateTime	dtManualShiftStartTime;
	//COleDateTime	dtPrevManualShiftStartTime;
	//COleDateTime	dtPrevManualShiftEndTime;
	int				iShiftNumber[ 7 ][ MAX_NUM_SHIFTS ];
	int				aiStartMinute[ 7 ][ MAX_NUM_SHIFTS ];
	int				aiStopMinute[ 7 ][ MAX_NUM_SHIFTS ];
	SBreakInfo		sBreakInfo;
} SShiftInfo;

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

typedef struct
{
	int					iFuncCode;
    
#ifdef TRIMMER
	QC					qc;
	QC_ALARMS			qc_alarms;
#endif
    
	IO_MAP				io_map;
	BRD_DATA			brd_data;
	LC_CALIB			lc_beam_width;
	CATEGORY			category;
	CUST_DATA			cust_data;
	IO_BIT_MAP			io_bit_map;
	ENET_COMM			enet_comm;
    
	LASER_MAP			laser_map;
	LASER_CALIBRATION	laser_calib;
	LIGHT_CURTAIN_MAP	lc_map;
    
    
#ifdef EDGER 
	EDG_DATA            edg_data;
#elif CANTER
	EDG_DATA            edg_data;
#endif
    
#if (DYNA_VISION_MODEL == 120)
	VISION_PARAMETERS	vision_parameters;
#endif
    
#if (STD_824S1 || STD_JS50)
	//CLineLzStruct::HEAD_MAP		HeadMap;
	//CLineLzStruct::HeadCalib	HeadCal;
#endif
    
	TCHAR				szConfigName[ 50 ];
} SUploadDownloadBuf;

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

// prototypes for functions related to SShiftInfo
BOOL	LoadShiftInfoFromDialogData(	const SShiftInfoForDialog& sShiftInfoForDialog, SShiftInfo& sShiftInfo, 
                                 int& iBadDayNum, int& iBadShiftNum );
void	LoadDialogDataFromShiftInfo( const SShiftInfo& sShiftInfo, SShiftInfoForDialog& sShiftInfoForDialog );
void	SetDefaultShiftInfo( SShiftInfo& sShiftInfo);
void	GetCurrentShiftInfo(	const SShiftInfo& sShiftInfo, int iCurrentDayOfWeek, int iCurrentTimeInMinutes,
                         int& iShiftNum, int& iShiftStartTime, int& iShiftStartDay,
                         int& iShiftStopTime, int& iShiftStopDay );
void	GetPreviousShiftInfo(	const SShiftInfo& sShiftInfo, int iCurrentDayOfWeek, int iCurrentTimeInMinutes,
                          int& iShiftNum, int& iShiftStartTime, int& iShiftStartDay,
                          int& iShiftStopTime, int& iShiftStopDay );
int		GetBreakTimeFromCurrentShiftBegin( const SShiftInfo& sShiftInfo, time_t ClearTime );
int		GetBreakTime( const SShiftInfo& sShiftInfo );
int		GetTimeUntilCurrentShiftEnds( const SShiftInfo& sShiftInfo, int iCurrentDayOfWeek, int iCurrentTimeInMinutes );
int		GetTimeFromCurrentShiftBegin( const SShiftInfo& sShiftInfo, int iCurrentDayOfWeek, int iCurrentTimeInMinutes );

int		GetCurrentDayIndex( int iOleDateTimeDayOfWeek );
int		GetCurrentDayIndexLocal( int iLocalDateTimeDayOfWeek );
int		GetNextDayIndex( int iCurrentDayIndex );
int		GetPrevDayIndex( int iCurrentDayIndex );

int		GetNumShiftsOnDay( const SShiftInfo& sShiftInfo, int iCurrentDayIndex );

void	GetFirstShiftInfo(	const SShiftInfo& sShiftInfo, int iCurrentDayIndex, int& iShiftNum, 
                       int& iStartTime, int& iStopTime, int& iStopDayIndex );
void	GetSecondShiftInfo(	const SShiftInfo& sShiftInfo, int iCurrentDayIndex, int& iShiftNum, 
                        int& iStartTime, int& iStopTime, int& iStopDayIndex );
void	GetThirdShiftInfo(	const SShiftInfo& sShiftInfo, int iCurrentDayIndex, int& iShiftNum, 
                       int& iStartTime, int& iStopTime, int& iStopDayIndex );
void	GetLastShiftInfo(	const SShiftInfo& sShiftInfo, int iCurrentDayIndex, int& iShiftNum, 
                      int& iStartTime, int& iStopTime, int& iStopDayIndex );

void	GetShiftInfo( const SShiftInfo& sShiftInfo, int iCurrentDayIndex, 
                  int iShiftNum, int& iStartTime, int& iStopTime, int& iStopDayIndex );


// structures detailing the data that is saved for each board
// when implementing a shift simulation
struct SRawLaserData
{
	RAW_LASER_DATA		bot_raw_laser_data	;
	RAW_LASER_DATA		top_raw_laser_data	;
};

// by keeping this struct the same on edger and trimmer
// edger and trimmer can be compiled to read same ShiftSim Data
struct SBoardDataForShiftSim
{
    
#if ( NUM_LZ_PER_UNIT == 8 )
	PCELLHISTORY		PCellHistory		;
	SCANNEDOBJECTCOPY	LaserBuffer			;
#elif (!STD_824S1 && !STD_JS50)
	SRawLaserData		RawLaserData		;
#endif
    
#if (VISION_ENABLED) // for Vision systems
	VISION_IMAGE_DATA	vs_image_data		;
#endif
    
#if (!STD_824S1)	// just save raw laser data on hermary
	PLAN_DATA			RawLCData			;
#endif
    
	MANUAL_PRESELECTS	ManualPreselects	;
	int					iMinInputGrade		;  // for edger
	int					iMaxInputGrade		;  // for edger
	int					iInputGrade			;  // for trimmer
	int					iInputMachine		;
	int					iCategory			;
	UINT				io_num				;  // mainly for Hermary case; added 9/2018 sjf
};

//-----------------------------------------------------------------------------
// TODO: Make sure folowing expressions are immune by alignment issues,
// because the presence of History Items, throws alignemnt off in lastboard.
// Revisit this code, once you check operation w/other laser families
//-----------------------------------------------------------------------------

// slm: 06-25-14 Changed declaration to works for all the configuration + and clean-up
// This was required because some memory pads insert by compiler, at times, in between terms of the structure
// This resulted in miss-calculating the expression for SIM_BOARD_SIZE
// Now we mimic OptWin operation where there we use only sizeof(SBoardData)
// MWP: 12/2017 moved to Hi-Tech.h so optwin and optimizer use the same struct format.  
// structure for board data being Viewed
// a simple x,y point with Brightness data (if available)
struct LineLzPoint
{
    unsigned short x;	// X inch units  (0 to 65,535)  - Range value
    short y;			// Y inch units  (-32,768 to 32,767)  - along length; 0=center of head.
#if (STD_JS50)
    char brightness;	// 0 to 255 gray scale image
#endif
};

#define LINE_LZ_MAX_HEADS			28		// Maximum value expected for any system.

#define LINE_LZ_CNTS_PER_LUG		MAX_NUM_CNTS_PER_LUG
#if STD_JS50
#define LINE_LZ_MAX_POINTS 728
#else
#define LINE_LZ_MAX_POINTS			108
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

typedef struct SBoardData_Tag
{
	RAW_LASER_DATA		bot_raw_laser_data;
	RAW_LASER_DATA		top_raw_laser_data;
    
	LAST_BOARD_DATA		last_board_data;
	BOOLEAN_PF_MAP		pf_map;
    
	// slm: 06-23-17 - Imported from EDGER
	// V79x New: This is now always present, even if no VISION_ENABLED
#if (NEW_V79X || VISION_ENABLED)
	VISION_DEFECT_DATA	 vs_defect_data;  // top and bottom image defect data
#endif
#if( VISION_ENABLED )
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