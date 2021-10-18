/*****************************************************************************
|  Name:        |  OPT_STR.H
|  Description: |  Universal header file for optimizer programs
|  $Revision: 1.9.1.2 $
|  $Date: 1996/07/30 18:19:42 $
 * Revision 5.5.122506.9  95/04/21 06:37:09  unknown
 *
 * Revision 0.12  91/09/20  11:37:30  KWG
 * development version
 *
*******************************************************************************
*         Copyright (C) 1991-2007, Baxley Equipment, Inc.
*****************************************************************************/

#ifndef __OPT_STR_H__
#define __OPT_STR_H__

#pragma warning(disable:4200)           //disables zero length array warning in nblib.h
#include "nblib.h"
//#include "UpLift.h"
#include "Enumerations.h"
#include "SolutionConfig.h"
#include "VisionDefectStruct.h"

#ifdef VISION_ENABLED
#include "TAV_Interface.h"
#endif

#define TRIMMER						   1   // Compiling for Specific Project

#define VER64xx                        0   // compile to load board with this version
#define VER763x						   0   // compile to load boards with version 7.6.0 to 7.6.3
#define USE_BLOCK_XFER				   1


#define HELP		0	// set to 1 to display ID numbers in Optwin; Set to 0 for shipping version.

// slm: 05-08-08
// Use this Symbols to quickly change all over the program TIMING information and Resolution
#define ENGINE_TIME_RESOLUTION		(double)1000000.0	// Value stored are in uSeconds
#define DUSE_TO_SHOW_ENGINE_TIME	(double)1000.0		// Divide by this value to Show in mSec. (double)

// default compile with all flag disabled is for a standard M6 system
// set the following to compile for job specific changes.
// only one flag should be set TRUE.

/*****************  Extinct Options (Start) ********************************************/
//#define HARDWOOD                       0   // 32in lug space
//#define CORTLAND                       0   // 1314-02 hardwood
//#define JGW                            0   // 1246-09 hardwood
//#define KITCHENS                       0   // 1303-01 hardwood
//#define CLEARLAKE                      0   // 1256-03 hardwood
//#define LAMPE                          0   // 1230-03 hardwood
//#define STD_B900						 0   // (B8 with Light Curtain)
//#define WARD                           0   // 1339-01 Hardwood B8 fiber system
//#define CAFFALL                        0   // 1354-01 M24 with 1/8" LC
//#define TEMBEC                         0   // 1225-06 5 Species and Categories; M24
//#define MATAGAMI                       0   // 1224-14 5 Species and Categories; M6
//#define STD_20FT_M24                   0   // 1178-04 Standard M24 system GP-Columbia
// 1248-01 Standard M24 system Hyne
// 1182-05 Standard M24 system GP-BaySprings
/*****************  Extinct Options (End)  ********************************************/



/*****************  Moved to SolutionsConfig.h   (Start) ****************************************/
//#define STD_M6                         0   // Standard M6 (up to 26ft)						
//#define STD_B8						   0   // Standard B8 Ethernet System

//#define STD_3X00					   0  // CS3300 or CS3100
//#define VISION_ENABLED				   0  // Set to one when using CS3300 heads; else zero for lasers only

//#define STD_824S1					   1   // Profile only version of Hermary Lasers
//#define SCANNER_ANGLED				   0	// 1 if scanning at angle;  0 if scanning at 90 degrees.

/*****************  Moved to SolutionsConfig.h   (End) ******************************************/



// used to define resolution of lasers for CS_3X00
// can only have ONE of these set true; others set to zero.
#define _3X00_HALF						0		// 1/2 inch scan density   (edger default)				
#define _3X00_QUARTER					1		// 0.250 inch scan density (trimmer default)

// used for building with OptComm Ethernet PLC5 communications DLL
#define OPTCOMM_DLL						1		// 0 build without DLL support


#define ODD                            1   // Odd Lengths manufactured also
#define EVEN                           2   // Even Lengths ONLY
#define LENGTH                      EVEN   // Odd Length Saw at -1' from gate?

// MIN_LEN_BRD and MAX_LEN_BRD must be even integers
#define MIN_LEN_BRD                    2   // Min length board handled in feet
#define MAX_LEN_BRD		              26   // Max length board handled in feet



#define TRUE                           1
#define FALSE                          0
#define UP                          TRUE
#define DOWN                       FALSE
#define MAX_NUM_SAWS                  24  // Max number of programable saws
#define MAX_SAW_CUTS				  5  // edger cut boundaries; 5 saws.
#define MAX_NUM_BRDS				  4  // edger cut boundaries; 4 boards

#define MAX_NUM_SORT_SETS				90  // maximum number of sets sorter can handle


// LASER DATA

// define an inch as 64 because encoder counts are 1/64 inch
#if STD_3X00
#define CNTS_PER_PIXEL					18  // 450 enc cnts per inch / 25 pixels per inch
#define LZ_CNTS_PER_INCH_HARD				50  // 50*0.020 resolution = 1"
#define LC_INCH_HARD						LZ_CNTS_PER_INCH_HARD // same as lasers   
#elif (STD_824S1 || STD_JS50)
#define LZ_CNTS_PER_INCH_HARD				64  // uses a 512 cnt encoder (4 ticks per pulse)
#define LC_INCH_HARD					    LZ_CNTS_PER_INCH_HARD  // same as lasers   
#else
#define LC_INCH_HARD						64  // uses a 512 cnt encoder
#define LZ_CNTS_PER_INCH_HARD				16  // uses a 512 cnt encoder
#endif

#if (STD_824S1 || STD_JS50)
#define LUG_SPACING                   26  // Hermary to initially support up to 26; reduced to save memory
#elif (STD_3X00)
#define LUG_SPACING                   22  // Max distance between lugs in inches; reduced to save memory
#else
#define LUG_SPACING                   32  // Max distance between lugs in inches
#endif

#define MAX_NUM_CNTS_PER_LUG         (LUG_SPACING*LZ_CNTS_PER_INCH_HARD)



#if (STD_3X00)				    //CS3x00 heads 
#if ( _3X00_QUARTER )
#define NUM_LZ_PER_UNIT			96	// CS3300 number of lz points per head (1/4")
#define UNIT_SPACING          0.25  // distance between laser spots within unit, inches
#else
#define NUM_LZ_PER_UNIT			48  // CS3300 number of lz points per head (1/2")
#define UNIT_SPACING          0.5  // distance between laser spots within unit, inches
#endif
#define	NUM_RAW_CS3X00_LASERS			76 // number of test points returned per head for CS3X00 laser heads	
#define NUM_UNITS                   (MAX_LEN_BRD/2)  // top & bottom head is one unit
#define MAX_NUM_LASERS              (NUM_LZ_PER_UNIT*NUM_UNITS)   // actually number of pairs of heads

#define INITIAL_LZ_OFFSET           0    // distance of first laser from near end
#define INTERNAL_STAGGER            0    // 
#define LASER_STAGGER               0    // as mounted on frame.  
#define DIAGS                       4    // this is four with the 4-in lasers
#define DYNA_VISION_MODEL           120  // this is for heads on 0.20-in spacing
#define MAX_NUM_LASER_PCELLS        8    // just a place holder; not used
#define IMAGE_X_RES					25   // pixels per inch (length)			
#define IMAGE_Y_RES					25	 // pixels per inch (width)			
#define MAX_IMAGE_X_RES				(IMAGE_X_RES*20*12)
#define MAX_IMAGE_Y_RES				(IMAGE_Y_RES*LUG_SPACING)

#endif

#if (STD_824S1)				    //Hermary heads 
#define NUM_LZ_PER_UNIT				96  // Hermary 1/4" resolution lz points per head
#define NUM_UNITS                   (MAX_LEN_BRD/2)  // top & bottom head is one unit
#define MAX_NUM_LASERS              (NUM_LZ_PER_UNIT*NUM_UNITS)   // actually number of pairs of heads

#define UNIT_SPACING          0.25  // distance between laser spots within unit, inches

#define INITIAL_LZ_OFFSET           0    // distance of first laser from near end
#define INTERNAL_STAGGER            0    // 
#define LASER_STAGGER               ((3.0/8.0)*LZ_CNTS_PER_INCH_HARD)    // as mounted on frame.  
#define DYNA_VISION_MODEL           824  // this is for heads on 0.20-in spacing
#define DIAGS                       4    // this is four with the 4-in lasers
#define MAX_NUM_LASER_PCELLS        8    // just a place holder; not used

// note:  these values likely to change for Hermary Vision System.  sjf 4/2014
#define IMAGE_X_RES					TAV_CAM_OUTPUT_PIXEL_PER_INCH_WIDTH   // pixels per inch (length)			
#define IMAGE_Y_RES					TAV_CAM_OUTPUT_PIXEL_PER_INCH_HEIGHT	 // pixels per inch (width)			
#define MAX_IMAGE_X_RES				(TAV_SYSTEM_OUTPUT_PIXEL_WIDTH)
#define MAX_IMAGE_Y_RES				(TAV_SYSTEM_OUTPUT_PIXEL_HEIGHT)

#endif

#if (STD_JS50)					    // JoeScan JS50  heads 
#define NUM_LZ_PER_UNIT				96  // Hermary 1/4" resolution lz points per head
#define NUM_UNITS                   (MAX_LEN_BRD/2)  // top & bottom head is one unit
#define MAX_NUM_LASERS              (NUM_LZ_PER_UNIT*NUM_UNITS)   // actually number of pairs of heads

#define UNIT_SPACING          0.25  // distance between laser spots within unit, inches

#define INITIAL_LZ_OFFSET           0    // distance of first laser from near end
#define INTERNAL_STAGGER            0    // 
#define LASER_STAGGER               ((3.0/8.0)*LZ_CNTS_PER_INCH_HARD)    // as mounted on frame.  
#define DYNA_VISION_MODEL           824  // this is for heads on 0.20-in spacing
#define DIAGS                       4    // this is four with the 4-in lasers
#define MAX_NUM_LASER_PCELLS        8    // just a place holder; not used

// note:  these values likely to change for Hermary Vision System.  sjf 4/2014
#define IMAGE_X_RES					TAV_CAM_OUTPUT_PIXEL_PER_INCH_WIDTH   // pixels per inch (length)			
#define IMAGE_Y_RES					TAV_CAM_OUTPUT_PIXEL_PER_INCH_HEIGHT	 // pixels per inch (width)			
#define MAX_IMAGE_X_RES				(TAV_SYSTEM_OUTPUT_PIXEL_WIDTH)
#define MAX_IMAGE_Y_RES				(TAV_SYSTEM_OUTPUT_PIXEL_HEIGHT)

#endif

#if (STD_M6)	//M6 heads 4" spacing
#define NUM_LZ_PER_UNIT				6	// M6 number of lz per head
#define UNIT_SPACING                4.0  // distance between laser spots within unit, inches
#define NUM_UNITS                     (MAX_LEN_BRD/2)  // top & bottom head is one unit
#define MAX_NUM_LASERS                (NUM_LZ_PER_UNIT*NUM_UNITS)   // actually number of pairs of heads
#define INTERNAL_STAGGER              0  // inches time 16 counts/inch; will be zero for M6 units
#define DIAGS                         4  // this is four with the M6 4-in lasers
#define DYNA_VISION_MODEL             6  // this is for 6 spots on 4-in spacing
#define INITIAL_LZ_OFFSET           1.0  // distance of first laser from near end
#define MAX_NUM_LASER_PCELLS          8  // max number of photocells in a system
#define PCELL_BUFFER_SIZE             0  // Size for the B8 Photocell buffer
#endif


#if (STD_B8)	// B8 lasers
#define NUM_LZ_PER_UNIT				8  // B8 number of lz per head
#define UNIT_SPACING                3.0  // distance between laser spots within unit, inches
#define NUM_UNITS                     (MAX_LEN_BRD/2)  // top & bottom head is one unit
#define MAX_NUM_LASERS                (NUM_LZ_PER_UNIT*NUM_UNITS)   // actually number of pairs of heads
#define INTERNAL_STAGGER              0  // inches time 16 counts/inch; will be zero for B8 units
#define DIAGS                         9  // B8 offers more diags
#define PCELL_BUFFER_SIZE          1024  // Size for the B8 Photocell buffer
#define MAX_NUM_LASER_PCELLS          8  // max number of photocells in a system
#define INITIAL_LZ_OFFSET           1.0  // distance of first laser from near end
#define DYNA_VISION_MODEL             8  // this is for 8 spots on 3-in spacing


// on B8 systems the encoder goes to unsigned INT_MAX not SHORT like M24
typedef struct {
    unsigned uBegEncoderCnt;  // the first encoder count for this cant.  This gives absolute reference for pcells
    // are physically mounted from laser head
    unsigned leading_edge_count[MAX_NUM_LASER_PCELLS];   // in counts
    unsigned trailing_edge_count[MAX_NUM_LASER_PCELLS];  // in counts
    unsigned LaserLeadEdgeCount[MAX_NUM_LASER_PCELLS];  // in counts
} RAW_LASER_PCELL_DATA;  /* Data from photocells gathered by M24 heads */

typedef struct{
	int iStartCnt;
	int iEndCnt;
	PHOTOCELLST PCellData[PCELL_BUFFER_SIZE];
} PCELLHISTORY;


typedef struct
{
    unsigned packetid;                                  /* internal use       */
    unsigned encoder;                                   /* encoder value      */
    unsigned short photocells;                          /* photocell bitmap   */
    unsigned short scanline[NB_MAXHEADS*NB_MAXSPOTS];   /* range readings     */
} ONELINECOPY;

typedef struct
{
    int allocatedlines;     /* input            */
    int validlines;         /* internal use     */
    int firstscanline;      /* output           */
    int lastscanline;       /* output           */
    int firstphotoline;     /* output           */
    int lastphotoline;      /* output           */
    unsigned  reserved1;    /* internal use     */
    unsigned  reserved2;    /* internal use     */
    ONELINECOPY scanline[MAX_NUM_CNTS_PER_LUG];   /* output           */
} SCANNEDOBJECTCOPY;

#endif


// following values are used for brd_data.LaserStagger
#define LZ_STAGGER_36                 36   // as mounted on frame.  2 1/4 inches times 16 counts/inch
#define LZ_STAGGER_22                 22   // as mounted on frame.  1 3/8 inches times 16 counts/inch
#define LZ_STAGGER_6                   6   // as mounted on frame.  3/8 inches times 16 counts/inch
#define LZ_STAGGER_8                   8   // as mounted on frame.  1/2 inches times 16 counts/inch (this side to side stagger)
#define LZ_STAGGER_0                   0   // as mounted on frame.  0 inches times 16 counts/inch (this side to side stagger)


// following values are used for brd_data.UnitJump
#define UNIT_JUMP_4                  4   // All M6 and some M24 systems; default
#define UNIT_JUMP_3                  3   // Most M24 Systems over 16ft long

// END LASER DATA

#define MAX_NUM_SMART_TIPPLES         14
#define MAX_NUM_FT_OF_LIGHT_CURTAIN   (MAX_LEN_BRD + 1)

#if (STD_3X00)				    //CS3x00 heads
#if ( _3X00_QUARTER )
#define NUM_LIGHT_CURTAIN_ELEMENTS_FT 48  /* software assumes this will be even
            number.  LC_LYOUT.C must be revised if this is changed.  */
#else
#define NUM_LIGHT_CURTAIN_ELEMENTS_FT 24  /* software assumes this will be even
        number.  LC_LYOUT.C must be revised if this is changed.  */
#endif
#elif (STD_824S1 || STD_JS50)
#define NUM_LIGHT_CURTAIN_ELEMENTS_FT 48 
#else
#define NUM_LIGHT_CURTAIN_ELEMENTS_FT 24  /* software assumes this will be even
          number.  LC_LYOUT.C must be revised if this is changed from 24.  */
#endif


// added () so order of math operation not confused when used in program.  sjf 12/2016
#define NUM_LC_ELEMENTS_INCH  (NUM_LIGHT_CURTAIN_ELEMENTS_FT/12.0)   //Number of diodes per inch

#define CARDS_PER_FOOT	1  // note: this will always be 1; can remove code associated with this value.  sjf 4/2017
#define TOT_DIODES   (MAX_NUM_FT_OF_LIGHT_CURTAIN*NUM_LIGHT_CURTAIN_ELEMENTS_FT)
#define NUM_LC_WORDS (1+TOT_DIODES/16)	// 16 used because data is stored
// by bits in a 16-bit word format. Add 1 to round up





// was 10; reduced to 3 to save memory rarelly if never used anyway.  sjf 7/2011
#define MAX_NUM_MACHINE_CENTERS        3  // Normally 0, needs manual input if >

#if ( TEMBEC || MATAGAMI )
#define MAX_NUM_SPECIES               5
#define MAX_NUM_CATEGORY              5
#elif ( CAT_15 )
#define MAX_NUM_SPECIES               0
#define MAX_NUM_CATEGORY              15
#else
#define MAX_NUM_SPECIES               0
#define MAX_NUM_CATEGORY              3
#endif


#define MAX_NUM_PRODUCTS               75

#define MAX_GRD_THK_COMBO              40
#define MAX_GRD_WTH_COMBO              MAX_GRD_THK_COMBO
#define MAX_NUM_THICK                  28
#define MAX_NUM_WIDTH                  28
#define MAX_NUM_GRADE				   28

#define MAX_NUM_LEN                    24   // Number of programable lengths
#define MAX_GRADE_THICK_MC             10  //May need to change if MACH_CNTRS > 0
#define MAX_WIDTH_MC                   10  //May need to change if MACH_CNTRS > 0
#define MAX_QC_PRFL_MC				   25  //Max number of TxW QC profiles to monitor	
#define NUM_INTERVALS                  (6*14) // sized for samples at 10-min intervals, 14-hr max shift
#define MAX_NUM_PAMUX_BRDS             16
#define NUM_WORDS_PER_LC_BOARD         50  // sizes mem allocation
#define MAX_NUM_CUTTINGS               10  // max number of cutting sizes for grade
#define MAX_NUM_SOL_CUTS               20  // max number of cuttings per solution


#define IN_TO_MM                       25.40  //inches to millimeters
#define FT_TO_CM                       30.48  //feet to centimeters
#define FT_TO_M                        0.3048 //feet to meters
#define CUFT_TO_CUM                    0.02832	//Cubic feet to cubic meters
#define BDFT_TO_M                      0.0023598  //bdft to cubic meters
#define ENGLISH           0
#define FRENCH            1
#define FTIN              0
#define INCHES            1
#define MBF               0
#define PER_PIECE         1

/* PLC Types for Communication */
#define AC28			0
#define _6008_PLC5		1
#define _6008_SLC		2
#define OMRON			3
#define KTX_PLC5		4
#define KTX_SLC			5
#define ENET_PLC5		6	// uses optcomm dll
#define ENET_SLC		7	// uses optcomm dll
#define ENET_LOGIX		8	// uses EtNetIP and EtIPAdapater dlls
#define ENET_CLOGIX		9	// uses Optcomm dll

/* Defined number of 16 bit input and output words avaliable to PLC */
/* Can vary based on project; Gang may need 128 output, while trimmer only needs 64 */
#define EIP_NUM_INPUT_WORDS		16
#define EIP_NUM_OUTPUT_WORDS	64

/* Zone Diag Word:  Word used to update zone diag. in Block Xfer and ENET output.*/
#define ZONE_DIAG_WORD				0	// Array index into EIP_OutputArray

/* PLC_WatchDog Word:  Word used to update watchdog value in Block Xfer and ENET output.*/
#define WATCH_DOG_WORD				61	// Array index into EIP_OutputArray

/* Scanner word:  Bits used for Scanner alarms and more status bits */
#define SCANNER_WORD		22		// WORD index into EIP_OutputArray
#define STICKING_LZ_BIT		0x0001	// Bit0:  Laser or LC detected as sticking ON.
#define LC_ALARM_BIT		0x0002	// Bit1:  Light Curtian has detected card or data failure.
#define NO_LZ_DATA_BIT		0x0004	// Bit2:  No Laser data alarm
#define TIMING_ALARM_BIT	0x0008	// Bit3:  Detected possible error in Encoder timing.
#define TOO_MUCH_SLASH_BIT	0x0010	// Bit4:  Exceeded the max number of allowed consecutive slash solutions.
#define NO_LZ_DATA_ALM_BIT	0x0020	// Bit5:  Exceeded # of bad lasers
//#define SC_SPARE2			0x0040  // Bit6:  Future use for other scanner alarms
#define B8_COMM_LOST		0x0080  // Bit7:  B8 ENET Comm Lost (Scanner may be off)
#define QC_ALARM1_BIT		0x0100  // Bit8:  Reserved for QC Alarms
#define QC_ALARM2_BIT		0x0200	// Bit9:  Reserved for QC Alarms
#define LASERS_PWR_ON		0x0400	// Bit10: Indication to PLC when LASER power is on
#define NO_INC_ENCODER		0x0800	// Bit11: No Incremental Encoder movement detected.
#define NO_ABS_ENCODER		0x1000	// Bit12: No Zone or Absolute Encoder movement detected.
//#define SC_SPARE7			0x2000	// Bit13: Future use for other status bits or scanner alarms
//#define SC_SPARE8			0x4000	// Bit14: Future use for other status bits or scanner alarms
//#define SC_SPARE9			0x8000	// Bit15: Future use for other status bits or scanner alarms


/* Status word 1: Bits used to define status of system or solution.  */
#define STATUS_WORD1			23	// Array index into EIP_OutputArray
#define MARK_BRD_BIT		0x0001	// Bit0:  Board is present on Lug
#define SKEWED_BRD_BIT		0x0002	// Bit1:  Skew in lug detected
#define ODD_SAW_BIT			0x0004	// Bit2:  Drop Odd Length Saw
#define ALARM_LIGHT_BIT		0x0008  // Bit3:  Bit that controls yellow light on scanner.
#define REMAN_NE_BIT		0x0010	// Bit4:  NE Board must be Re-Edged
#define REMAN_FE_BIT		0x0020	// Bit5:  FE Board must be Re-Edged
#define RESAW_NE_BIT		0x0040	// Bit6:  NE Board must be Re-Sawn
#define RESAW_FE_BIT		0x0080	// Bit7:  FE Board must be Re-Sawn
#define DIVERTER_BIT		0x0100	// Bit8:  Move Diverter
#define SLASH_BIT			0x0200	// Bit9:  Board has no value; slash
#define RANDOM_LEN_BIT		0x0400	// Bit10: Solution is Random Length
#define WANE_DOWN_BIT		0x0800	// Bit11: Board is wane down
#define WANE_UP_BIT			0x1000	// Bit12: Board is wane up
#define OVER_THICK_BIT		0x2000	// Bit13: Too Thick for Trimmer
#define NO_TRIM_BIT			0x4000	// Bit14: Forced No Trim Solution
#define CONVEYOR_BIT		0x8000	// Bit15: Stop Conveyor

#define STATUS_WORD2			29	// Array index into EIP_OutputArray
#define MARK_TOP_BIT		0x0001	// Bit0:  Mark top of brd for Planer
#define MARK_BOTTOM_BIT		0x0002	// Bit1:  Mark bottom of brd for Planer
#define CAL_MODE_BIT		0x0004  // Bit2:  System is in calibration mode.
#define OVER_LEN_BIT		0x0008  // Bit3:  Board To long to fit in Trimmer; Stop Chain!
#define ST_SPARE4			0x0010  // Bit4:  Future use status bits
#define ST_SPARE5			0x0020  // Bit5:  Future use status bits
#define ST_SPARE6			0x0040  // Bit6:  Future use status bits
#define ST_SPARE7			0x0080  // Bit7:  Future use status bits
#define ST_SPARE8			0x0100  // Bit8:  Future use status bits
#define ST_SPARE9			0x0200  // Bit9:  Future use status bits
#define ST_SPARE10			0x0400  // Bit10;  Future use status bits
#define ST_SPARE11			0x0800  // Bit11:  Future use status bits
#define ST_SPARE12			0x1000  // Bit12:  Future use status bits
#define ST_SPARE13			0x2000  // Bit13:  Future use status bits
#define ST_SPARE14			0x4000  // Bit14:  Future use status bits
#define ST_SPARE15			0x8000  // Bit15:  Future use status bits

#define SCAN_STATUS_WORD		57	// Array index into EIP_OutputArray
#define NEW_LZ_DATA_BIT		0x0001	// Bit0:  Got first indication of new data available
#define PROCESSED_LZ_BIT	0x0002	// Bit1:  Finished Processing LZ data; ready for solution use
#define SOLUTION_START_BIT	0x0004  // Bit2:  Optimizer starts processing soluiton data.
#define SOLUTION_DONE_BIT	0x0008  // Bit3:  Optimizer completes solution process
#define PLC_OUTPUT			0x0010  // Bit4:  Optimizer sends solution to PLC
#define SCST_SPARE5			0x0020  // Bit5:  Future use status bits
#define SCST_SPARE6			0x0040  // Bit6:  Future use status bits
#define SCST_SPARE7			0x0080  // Bit7:  Future use status bits
#define SCST_SPARE8			0x0100  // Bit8:  Future use status bits
#define SCST_SPARE9			0x0200  // Bit9:  Future use status bits
#define SCST_SPARE10		0x0400  // Bit10;  Future use status bits
#define SCST_SPARE11		0x0800  // Bit11:  Future use status bits
#define ZONE_0_START_BIT	0x1000  // Bit12:  Start of Zone 0 Processing
#define SCST_SPARE13		0x2000  // Bit13:  Future use status bits
#define SCST_SPARE14		0x4000  // Bit14:  Future use status bits
#define SCST_SPARE15		0x8000  // Bit15:  Future use status bits

// These Items are compared with bits set in the registry to determine if
// a AddOn feature has been purchased an set to be enabled.
// Each feature uses 4 bits of a 32 bit word to encode the passcode to turn on.
// Use a value between 2 and E when setting up a new passcode Option.
//NOTE: If any items are added to this list, add a supervisor msg at the end
//      of the Init() to report that feature is enabled.  These features should
//      ONLY be enabled if customer has purchased the addon feature.
#define OPTION_ECOUSTIC			0x0000000E	// (can be 2-E)Future use Option bits
#define OPTION_SPARE2			0x00000010	// (can be 2-E)Future use Option bits
#define OPTION_PAINT_SPRAYER	0x00000A00  // Allows Paint Sprayer feature to work
#define OPTION_SPARE3			0x00001000  // (can be 2-E)Future use Option bits
#define OPTION_SPARE4			0x00010000  // (can be 2-E)Future use Option bits
#define OPTION_SPARE5			0x00100000  // (can be 2-E)Future use Option bits
#define OPTION_SPARE6			0x01000000  // (can be 2-E)Future use Option bits
#define OPTION_SPARE7			0x10000000  // (can be 2-E)Future use Option bits

/* defining 6008 scanner to PLC transfer speed choices */
#define PLC_IO_57               0
#define PLC_IO_115              1
#define PLC_IO_230				2

/* Defining of Acquire Mode functions */
#define ACQUIRE_ALL				0 //default normal operation
#define ACQUIRE_TEST_BOARDS		1 //requires input from PLC
#define ACQUIRE_REMAN_SLASH		2
#define ACQUIRE_TRIM_PASS		3
#define ACQUIRE_BY_PRODUCT		4 // acquires only products selected
#define	ACQUIRE_QC_ALARM_BRDS	5
#define ACQUIRE_SLASH			6

/* Defining if profile has wane up, down, or square */
#define NO_WANE              -1
#define WANE_UP               0
#define WANE_DOWN             1

/* these items used to identify marking for planer mill */
#define BOTTOM_LEAD			0		// bottom face, leading edge (green trimmer ref)
#define BOTTOM_TRAIL		1		// bottom face, trailing edge (green trimmer ref)
#define TOP_LEAD			2		// top face, leading edge (green trimmer ref.)
#define TOP_TRAIL			3		// top face, trailing edge (green trimmer ref.)
#define SPRAY_TOP             1
#define SPRAY_BOT            -1



/* Defining of tally messages to be displayed on Vorne. display_error_msg()*/
#define NUM_TALLY_MSG    7  // this should equal last valid tally_msg
#define BRDFT_OUT        0  // tally_msg
#define TRIM_LOSS        1  // tally_msg displayed as percentage
#define REMAN_PERCENT    2  // tally_msg based on num pieces
#define SLASH_PERCENT    3  // tally_msg based on num pieces
#define BDFT_SLASH_PCT   4  // tally_msg based on bdft
#define UP_TIME_PCT      5  // tally_msg based on time running 
#define FULL_LUGS        6	// tally_msg based on full lugs		
#define BDFT_PROD_PCT    7  // tally_msg based on bdft


/* Defining of error codes from lowest to highest priority for display */
#define NUM_ERRORS                    13
/* If any changes are made in the following NUM_ERRORS list, change the error_list
   initialization in acquire.c */
#define NO_ERROR_CODE                  0
#define EXCESS_DATA_DROPOUTS           1
#define WTH_OFF_TARGET                 2
#define THK_OFF_TARGET                 3
#define ALARM_1                        4
#define ALARM_2                        5
#define REJECT_ALARM                   6
#define NO_LASER_DATA                  7
#define STICKING_LASER                 8
#define NO_LC_DATA                     9
#define SKEW_IN_LUG                   10
#define NOT_EVEN_ENDED                11
#define NOT_POSITIONABLE			  12
#define EMPTY_LUG					  13
#define NUM_THK_COUNTS              1300
/****************************************************************************
ERROR MESSAGES USED IN DISPLAY & ACQUIRE
    error_code.code          error_code.param[0]     error_code.param[1]
  0 NO ERROR
  1 EXCESSIVE DATA DROPOUTS    laser#                 0=top, 1=bottom
  2 WTH OFF TARGET             Amount off target
  3 THK OFF TARGET             Amount off target      laser#
  4 ALARM 1 EXCEEDED           0=thk, 1=wth           target*1000
  5 ALARM 2 EXCEEDED           0=thk, 1=wth           target*1000
  6 REJECT ALARM               0=slash, 1=resaw, 2=reman
  7 NO LASER DATA              laser#                 0=top, 1=bottom, 2=both
  8 STICKING_LASER             laser# 
  9 NO LC DATA                 board#
 10 SKEW IN LUG
 11 NOT EVEN ENDED
 12 NOT_POSITIONABLE
 13 EMPTY_LUG					Used for CS31xx Web Mode
****************************   End of NUM_ERRORS list ****************************/

typedef struct {
    int code;
    int param[5];
} ERROR_CODE;               // Warning Errors for display go here

// This section is used in conjunction with iVisionDefectTypes in the wane rule struct.
// These bits should be turned ON when the wane rule should include these defect types
// in the evaluation of the final solution.  (Requires use of CS3300 sensors)
// Usage: You reference at this as follow:
//		<expression> = GradeDefectType::GRADE_DEFECT_WANE;
// or
//		<expression> = GRADE_DEFECT_WANE;
// another example
// 		<expression>.iVisionDefectTypes = (GradeDefectType)((int)GRADE_DEFECT_KNOT |
//															(int)GRADE_DEFECT_PITCH_POCKET);
// a final example:
// if (<expression>.iVisionDefectTypes & GRADE_DEFECT_KNOT) iWhatEver++;

typedef enum
{
    GRADE_DEFECT_NO_DEFECT			= 0x0000,
	GRADE_DEFECT_KNOT				= 0x0001,
    GRADE_DEFECT_PITCH_POCKET		= 0x0002,
    GRADE_DEFECT_STAIN				= 0x0004,
    GRADE_DEFECT_OTHER				= 0x0008,
    GRADE_DEFECT_WANE				= 0x0010,
	// Insert in front this symbol any new defect, so the following is always last
	// Change to reflect the number of defects listed that have a bit set (ONLY)
	GRADE_NUM_DEFECTS_LISTED		= 5
} GradeDefectType;

// VisionDefectType moved to VisionDefectStruct.h;   sjf 10/19/2017


typedef enum
{
	TSS_NotDefined		= 0x0000,	// saw disabled
    TSS_EvenLength		= 0x0001,	// Typical Even Length Solutions
	TSS_OddLength		= 0x0002,	// Typical Odd Length Solutions
	TSS_RandomLength	= 0x0004,	// Length is cut to a variable range
    TSS_NomLength		= 0x0008,	// Cut exactly to nominal even length 8'-0; 12'-0 etc.
    TSS_40InchLength	= 0x0010,	// 40" Length Pallet Stock (Custom1)
    TSS_48InchLength	= 0x0020,	// 48" Length Pallet Stock (Custom2)
	TSS_Custom3			= 0x0040,
	TSS_Custom4			= 0x0080,
	TSS_SawDisabled		= 0x0100,	// consider saw disabled not usable.
    
	// Insert in front this symbol any new saw scheme, so the following is always last
	// Change to reflect the number of schemes listed that have a bit set (ONLY)
	TSS_NUM_SawSchemes		= 9
} TrimmerSawScheme;


// sjf 6/16/2017
// feature added to finite control how specific product solutions are considered. 
typedef enum
{
	PrdOp_Disabled		= 0x0000,	// product disabled; do not consider as possible solution.
	PrdOp_Enabled		= 0x0001,	// product enabled; ok to consider as possible solution.
	PrdOp_CenterOnly	= 0x0002,	// product enabled, but no strip save or can only be made with center saws
	PrdOp_Spare1		= 0x0004,	// spare reserved; may be used for other projects that already use this value.
	PrdOp_Spare2		= 0x0008,	// spare reserved; may be used for other projects that already use this value.
	PrdOp_NoReEdge		= 0x0010,	// product enabled, but do not consider for Re-Edge solutions.
	PrdOp_NoReSaw		= 0x0020,	// product enabled, but do not consider for Re-Saw solutions.
	PrdOp_NoCutInTwo	= 0x0040,	// (future feature) product enabled, but do not consider for Cut-in-two solutions.
	PrdOp_NoTrimSave	= 0x0080	// (future feature) product enabled, but do not consider for trim save
}ProductOptions;


#define THK_FLUTTER      75
#define SQR_DIFF         200
#define PASS_SAW_ADD     4000
#define BETWEEN_LZ       0
#define AT_LZ            1
#define NUM_LZ_PAST_POS  3

#define NO_TOP_OR_BOT    3
#define NO_BOT           2
#define NO_TOP           1
#define TOP_AND_BOT      0

//--------------------------------------------------------------------------------------------------
// sjf 5/2016
// STD_824S1 - added so that integration into other projects finds this update.
// Adding multi state status to laser map for Hermary and chromascan systems which
// only the bottom laser needs to be disabled.  This feature mainly implemented so that
// we could ignore warning messages of NO BOTTOM LASER DATA when we know locations blocked by chain.
// may evolve into other uses as time goes on.  But this was implemented to make code as backward
// compatible to previous usage of only two states -  OK and BAD.
//
//
// macro to determine multi-state status of spot
// both top and bottom bad		head_status = 0x0
// both top and bottom good		head_status = 0x1
// just top mapped out			head_status = 0x2
// just bottom mapped out.		head_status = 0x4
// replace laser_map.head_status[] with this macro to maintain backward compatibility
// example:   HeadStatusOK(laser_map.head_status[i])
#define HeadStatusOK(head_status)			(head_status & 0x1)	// macro returns true only if both spots ok	
#define HeadStatusOK_Top(head_status)		(head_status & 0x5)	// macro returns true if top spot ok
#define HeadStatusOK_Bot(head_status)		(head_status & 0x3)	// macro returns true if bottom spot ok	
#define HeadStatusOK_TopOnly(head_status)	(head_status & 0x4) // macro returns true if ONLY top spot ok	
#define HeadStatusOK_BotOnly(head_status)	(head_status & 0x2)	// macro returns true if ONLY bottom spot ok

typedef enum
{
	// using bitwise values and checking with new macro HeadStatusOK(head_status)
	LASER_DISABLED_BOT				=  0x4,	// New option to disable only the bottom laser
	LASER_DISABLED_TOP				=  0x2,	// New option to disable only the top laser
	LASER_DISABLED_BOTH				=  0x0,	// backward compatible to original way
	LASER_ENABLED_BOTH				=  0x1	// backward compatible to original way
} LaserEnabled;

//---------------------------------------------------------------------------------------------------


/* stuff to define the resident library functions */
#define RES_SIG    0xa55a     /* unique fuction number for INT 16h  */

#define NUM_SPARES				91   // number of spares in IO_MAP; used below and in ioconfig.c 

#define NUM_LEN_COUNTS  ((MAX_LEN_BRD+1-MIN_LEN_BRD)*24)

#define MAX_NUM_SAVED_SOL 3   //Save the top three solutions found for reference




typedef struct {
	int	  iNomLength;  // inches * 10
	float fCrookAmount[7][4];
} CROOK[10];

typedef struct {
	int	  iNomLength;  // inches * 10
	float fTwistAmount[6][4];
} TWIST[9];

typedef struct {
    char name[16];
} MACHINE_CENTER;


/* SPECIES:	Used with density to find a category */
typedef struct	{
    struct {
        int MinDensity;
        int MaxDensity;
        int CategoryNum;
    } density[3];
    char name[16];
} SPECIES;


/* BRD_DATA:  Contains all parameters necessary to classify a piece and
*             determine the "good wood" which may be contained in a given
*             section of the piece  */

typedef struct {
    int iSpare0;			// Scheduled Downtime  int default_category;
    int default_category;
    int acquire_mode;			// 0=normal; 1=Test Boards; 2=Reman/Slash; 3=Pass/Trim
    int LzNoDatAlm; // Number of Lasers with consecutive no data to send alarm
    int handling;             /* cost in cents of handling a cut-in-two */
    int HandlingPcnt;		// percent deduct for boards to be cut-in-two	0 - 100
    int MaxTimingLimit;       // inches*1000; typically one or two inches less than lug spacing
    int MinTimingLimit;       // inches*1000; typically four or five inches less than lug spacing
    int LaserStagger;         // allow standard program to work with "standard" or "near zero" laser stagger.
    
    int min_len_handled;      /* minimum length handled (MIN_LEN_BRD to MIN_LEN_BRD+6')   */
    int max_len_handled;      /* maximum length handled (?' to 24')   */
    int keep_best_end;		// Cosmetic option to keep end with less wane.
    bool OddSaw[MAX_NUM_SAWS];// Flag to designate saw for use with odd solutions only
    int spare107; 			// Spare use to be part of odd_len[]
    short ThkRemoved;			// Estimated thickness to be removed at planer. (inches*1000)
    short WthRemoved;			// Estimated width to be removed at planer.     (inches*1000)
    int NoTrimLength;			// Additional length above min_len to allow No Trim option.
    short NoTrim0ftSaw;		// Allow Optimizer to raise zero ft saw on min_len brds
    short spare110b;			// Spare use to be part of odd_len[]
    short UseAvgThkWthQC;		// Use average thk & Wth for QC instead of detailed measurements.
    short MarkForPlaner;		// TRUE or FALSE; TRUE = Spray Marker in Use.
    short PlanerReference;	// Top/Trail, Bottom/Trail...;FACE/EDGE used for ref
    short PctDiffToMark;		// Minimum Percentage diff before board will be marked
    short EdgeWaneWeightPct;	// Weighting applied to edge wane
    short FaceWaneWeightPct;	// Weighting applied to face wane
    int ResawAlarm;			// X out of 100 slashes to trigger alarm
    int RemanAlarm;			// X out of 100 slashes to trigger alarm
    int SlashAlarm;			// X out of 100 slashes to trigger alarm
    int MaxThickToTrimmer;		// Max thickness trimmer can pass; send alert to PLC if greater
    int iMaxGateOnReman;		// Maximum gate on reman operation
    short block_xfer;			// Allow Block Transfer to PLC
    short TrimReman;			// Trim Reman boards within 2ft of solution before sending back into mill.
    int StRockGateUsed;		// Push back style gate; True or False value
    int IngnorePreselectsForLowerGrades; // Apply manual defect trim only for grade specified; ignore on lower grades (True or False)
    eLUMBER_TYPE iLumberType;	// Flag to use special cases that apply to hardwood and/or planner mills.		        
    int num_gate_increments2; // number of increments for far-end gate
    int min_length_to_edger;  // Min Length that will transport to edger in lc elements
    int odd_length_saw_loc;   // Odd Length saw is inside 0' saw by how much
    int num_lc_eq_notch;      // number of lc elements < min_w to be a notch
    int metric;               // display in metric or english units
    eTRIM_TYPE saw_type;      // 0=1-2 saws; 1=multi-saw trimmer; 2=-1ft saw; 3=-1ft saw w/precut. 
    double spare2;
    int expand_chain_runners; // True increases len when brd ends at runner
    // False assumes brd ends at runner
    char second_brd;          // 'S' = Sorter, 'T' = Tipple, 'D' = Divert
    char dummy_c;             // hold for later use
    short spare_c;            // holding place until second_brd is converted to int
    int spare11;
    // max dist in lc elements ne can be from even end before tippled or
    // chain stooped
    int max_displace_frm_ee_to_trim;
    int MakeLongestSolution;  // Make longest single board, non-reman solution
    int min_thk_convey;       // min thick to convey in thousandths of an inch
    int lc_present;           // True if light curtain installed.
    int reman_available;      // True if able to change width, False if not
    int simplified_reman_resaw;// True if nominal piece sizes wanted at any cost,
    // False if optimum solution wanted at any cost
    int reman_cost;           // cost in cents of handling a re-edge or resaw
    int EdgePcnt;			// percent deduct for boards to be edged	0 - 100
    int trim_end_preference;  // End preferred to cut excess good wood,
    // Near End (1) or Far End
    int percent_face_preference;// amount of face preferred on piece before using
    // trim preference
    int use_6008;       // 0=PAMUX, 1=PLC5, 2=SLC500, 3=OMRON
    // amount of wafer to remove if extra good wood available in LC elements
    int ne_wafer_amount;      // Near End
    int fe_wafer_amount;      // Far End
    int gate_increment;       // increment the gate can be moved in lc elements
    int num_gate_increments;  // number of increments
    int numAlarms1;           // consective off targets for alarm 1
    int numAlarms2;           // consective off targets for alarm 2
    int piecesForSRRAlarm;    //Consective pieces for slash/resaw/reedge alarm
    int max_bow_allowed;      // max bow from chain before cut-in-two
    int dry_density;          // density of chips in inches ##.##
    int chip_price;           // price $*100
    int chip_value;           // (dry_density*chip_price/2204.62)*100
    int cut_bows;             // Cut bowed boards in two.  
    int divert_parcel;        // 1 out of divert_parcel will be diverted
    int num_bad_diodes;       // Number of bad diodes not mapped out to set alarm        
    int divert_slashes;       // divert slashes, True or False
    int max_len_to_trimmer;   // in lc elements
    int pass_saddle;          // pass saddle wane pcs instead of cut-in-two,
    // True or False
    int cut_in_two;           // cut in two solutions are available T/F
    int max_length_to_edger;  // Max Length that will go thru edger in lc elements
    int num_saws;             // Number of programmed saw locations
    int saw_location[MAX_NUM_SAWS];// in lc elements from near end
    int NumFaultyStations;  // must be two or greater
    int NumOverThick;       // must be two or greater
    int NumOverWidth;       // must be two or greater
    int flitch_pb;            // push button to signify flitch grade
    int max_thick_to_edger;   // Max Thick to go thru edger in thousandths
    int trim_save;            // True to activate, False to deactivate
    int num_smart_tipples;    // Number of smart tipple locations
    int smart_tipple_loc[MAX_NUM_SMART_TIPPLES]; // in lc elements from near end
    int flitch_gate;          // gate flitch to save specified length
    int first_thk;           // t_class array element to smallest thick
    int first_wth;           // w_class array element to smallest width
    int first_len;           // l_class array element to shortest length
    int length_option;       // 0 = ft-inches, 1 = inches
    int max_dist_2_even_end;  // max dist board can be from gate after trim
    // in lc elements
    int min_len_convey;      // length in LC elements that can be conveyed
    int ResawPcnt;			// percent deduct for boards to be resawn	0 - 100
    int resaw_cost;              // addit'l cost of resaw over reman(may be neg.)
    int first_prd;
    
    int iFeatherEdgeAllowed;	// Allow mill to set value to evaluate feathers. (0.250" default)
    int iBoatPaddleLength;	// length in LC elements allowed to force trim on boat paddle type boards
    int iNumSortBays;			// Number of Sort Bays; compared against active set assignments
    int iUseEdgerCode;		// Evaluate edger solutions using Baxley edger code
    int iSawScheme[MAX_NUM_SAWS];	// helps define which saws can be used for which type solutions
    int iAlarmPctTrimLoss;	// Pct * 10;  123 = 12.3%
    int iAlarmPctResaw;		// Pct * 10;  123 = 12.3%
    int iAlarmPctRedge;		// Pct * 10;  123 = 12.3%
    int iAlarmPctSlash;		// Pct * 10;  123 = 12.3%
    int iAlarmPctReman;		// Pct * 10;  123 = 12.3%
    int spares[31];			// spares for future use
    
#if MAX_NUM_SPECIES
    SPECIES species[MAX_NUM_SPECIES];
#endif
    
} BRD_DATA;   // stored in disk file named BRD_DATA.DAT


//*** Structs for the plc ethernet setup ***
typedef enum PLCDataTypes
{
	DISCRETE_WRITE = 0,	// Opt to PLC
	DISCRETE_READ,		// PLC to Opt
	BLOCK_WRITE,		// Opt to PLC
	BLOCK_READ			// PLC to Opt
} PLCDataType;


typedef struct {
	unsigned short usFileNum;
	unsigned short usEleNum;
	unsigned short usFileLngth;
	unsigned short spare1;
}ENET_LOC;

typedef struct {
	char cTagName[32];
	int  iLength;
}ENET_DEVICE;

// LocInfo Array:		0=Discrete Input; 1=Discrete Output; 2=Block Xfer Input; 3=Block Xfer Output
// DeviceInfo Array:	0=Input; 1=Output; 2=Spare; 3=Spare
typedef struct {
	ENET_LOC		LocInfo[4];
	ENET_DEVICE		DeviceInfo[4];			//info specific to Control Logix PLC
	int				iLogixProcSlot;
	int				iSpare[15];
    
	BYTE			byteIPAddress[4];
	BYTE			byteIPAddressSpare1[4];
	BYTE			byteIPAddressSpare2[4];
}ENET_COMM;

/*  LASER_MAP  This table stores the status of each head as either BROKE(0)
     or OK(1)  */
typedef struct {
    unsigned int num_heads; // actually is number of pairs of heads
    unsigned int head_status[MAX_NUM_LASERS]; // BROKE or OK in pairs
    unsigned int dist_from_even_end[MAX_NUM_LASERS]; /* position of laser,
       measured from even end, in hundredths of inches   */
    unsigned int dist_in_ele[MAX_NUM_LASERS+1]; /* position of laser,
       measured from even end, in lc elements */
}  LASER_MAP;  // stored in disk file named ZMAPDATA.DAT


/*  RAW LASER DATA  The measurement data from the last whole lug space. */
// note: the first three words are wasted space; only the 0 element of RAW_LASER_DATA
//       is used to hold these numbers.  The rest are zero.
typedef struct {
#if (!STD_M6) // M6 requires specific struct format to read data from driver.
    unsigned short height;	// pixels used for width of board (Y Res)
    unsigned short width;		// pixels used for length of board (X Res)
    unsigned int io_num;
    unsigned int count;		// number of encoder counts to the end of data
    unsigned int iStartingEncoderValue;	// encoder value of first scan line.
#if ((STD_824S1 || STD_JS50) && !SCANNER_ANGLED)
    int range[MAX_NUM_CNTS_PER_LUG];   // as inches times 1000  must handle numbers larger than 32k
#else
    short range[MAX_NUM_CNTS_PER_LUG];   // as inches times 1000  (-32000 to 32000)
#endif
#else
    unsigned int io_num;
    short range[MAX_NUM_CNTS_PER_LUG];   // as inches times 1000
    int count;		// number of encoder counts to the end of data
    
#endif
    
} RAW_LASER_DATA_STRUCT;/* from near end as pairs, use for both
                                      top and bottom.  */
typedef  RAW_LASER_DATA_STRUCT RAW_LASER_DATA[MAX_NUM_LASERS];  


#if (!STD_3X00)
// LASER_TEST_DATA:  The current reading from the laser measuring array.
typedef struct {
    int diag[DIAGS][MAX_NUM_LASERS*2];
    int iChainRunning;  // special flag used to indicate type of data in diag. (for B8 system)
} LASER_TEST_DATA;
#endif

#if (STD_3X00)				    //CS3x00 heads 0.200" spacing	

// LASER_TEST_DATA:  The current reading from one CS3x00 laser head.
typedef struct {
	int			iViewIndex;	// 0=left  1=right side of head
	int			iCount;		// Number of Profiles reporting
	int			dDeviceID;	// SN of laser head
	int			iSpare[32]; // reserved for future use
	short int points[NUM_RAW_CS3X00_LASERS][2]; // [0]=x; [1] = y = range value
} LASER_TEST_DATA;


typedef struct {
    int height;	// profile data points used for width of board (Y Res)
    int width;	// profile data points used for length of board (X Res)
    short int CsRange[MAX_NUM_CNTS_PER_LUG][MAX_NUM_LASERS];
} CS3300_RANGE_STRUCT;

typedef struct {
    CS3300_RANGE_STRUCT	sTopRangeData;
    CS3300_RANGE_STRUCT	sBottomRangeData;
} CS3300_RANGE_DATA; 

#endif // CS3x00

// slm: 06-23-17
//#define MAX_NUM_VISION_DEFECTS		500

// slm: 06-23-17
#if ( VISION_ENABLED )
typedef struct {
	int height;	// pixels used for width of board (Y Res)
	int width;	// pixels used for length of board (X Res)
	UINT32 Pixels[MAX_IMAGE_Y_RES][MAX_IMAGE_X_RES];
} VISION_IMAGE_STRUCT;

typedef struct {
	UINT	iStartingEncoderValue;	// only field for TAV_Scanner image data
	VISION_IMAGE_STRUCT	sTopImageData;
	VISION_IMAGE_STRUCT	sBottomImageData;
	int iTopLaserVideoOffset[MAX_NUM_LASERS];
	int iBottomLaserVideoOffset[MAX_NUM_LASERS];
} VISION_IMAGE_DATA;
#endif

#if 0
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

typedef struct {
	int iNumDefects;
	VISION_DEFECT sDefects[MAX_NUM_VISION_DEFECTS];
} VISION_DEFECT_STRUCT;

typedef struct {
	VISION_DEFECT_STRUCT	sTopDefectData;
	VISION_DEFECT_STRUCT	sBottomDefectData;
} VISION_DEFECT_DATA;

#endif

// slm: 09-05-08 - Only first 2 methods are initially Allowed and Released
typedef enum
{
	eCenterOnly	= 0,				// Center/Split Taper Angle only (For backward compatability)
	eBestAngle,						// After solution is generated, rework grade for best angle only
	eAllAngles,						// Solve cuttings in all angles
	eSolveAsNoVisionPresent			// Used only to run code without regenerating a new version
} VISION_SOLUTION_METHOD;

typedef enum 
{ 
	RANDOM,
	RIP,
	CROSS_CUT,
	NUM_CUTTING_POSSIBLES			// ALWAYS HAVE THIS LAST IN THE ENUMERATION!!
}CUTTING_OPTIONS;

typedef struct
{
	int		knotThreshold;			/* percentage of intensity */
	int		darkThreshold;			/* percentage of intensity */
	int		waneThreshold;			/* mils */
	int		iUseRandomCutSearch;	/* Turn random start point in cuttings searh on and off */
	VISION_SOLUTION_METHOD	eSolutionMethod;	// How cuttings are generated
	CUTTING_OPTIONS			eCuttingOptions;	// Rip, Crosscut, or random
	int		ispare[61];				/* Spares int for future use */
    
	double	defectMaxNormalArea;	/* square inches */
	double	defectMinKnotArea;		/* square inches */
	double	dspare[32];				/* Spares doubles for future use */
} VISION_PARAMETERS;

typedef struct {
    int iEncoderCount;
    int iPCellCount;
    int iZoneEncoderValue;
    int iWebLugID;	//CS3x00 heads
    int iCntsPerLug;  //available on Hermary and BMX systems
} ENCODER_TEST_DATA;

/* structure for reading in m24 initialize file  FACTOR.DAT */
typedef struct {
    int lead;       // debounce on lead
    int trail;      // debounce on trail
    int maxscan;    // maximum encoder scans before reset
    int minspots;   // minimum number of spots to start scan
    int revcount;   // maximum number of counts to reset scan
    int spare;    // extend leading edge buffer in encoder pulses
} FACTOR; 

/* LIGHT_CURTAIN_MAP:  Each light curtain section is an array of photo cells.
*    These cells span continuously for the entire board length, but areas of
*    the array may be blocked by chain runners, dirt or defective elements.
*    This table maps out sections of the array that should not be used.  Set
*    bit to zero to map out. */
typedef struct {
    int dist_from_even_end; /* location of initial element, in hundredths of
       an inch. Assume all others follow on centers determined by the value of
       NUM_LIGHT_CURTAIN_ELEMENTS_FT.  When this is 24, elements are on
        1/2-inch centers.  Note that the initial element may be, and probably
        is, a negative number.  */
    unsigned short section[NUM_LC_WORDS]; /* 16 used because data is stored
       by bits in a 16-bit word format. Add 1 to round up  */
} LIGHT_CURTAIN_MAP;  // stored on disk in file named LCM_DATA.DAT

/* LC_MARKED_BAD:  Diodes marked as bad in lc_lyout.c are stored in
     lc_marked_bad.manual[]. Diodes marked as bad in lc_test.c are
     stored in  lc_marked_bad.auto_map[]. All diodes from either array
     marked bad are stored in light_curtain_map. Set bit to zero if bad. */
typedef struct {
    unsigned short auto_map;
    unsigned short manual; /* 16 used because data is stored
       by bits in a 16-bit word format. Add 1 to round up  */
} LC_MARKED_BAD[NUM_LC_WORDS+1];      // stored on disk in file named LC_MBAD.dat
// +1 added just so file size would be different between formats of 7.4 and 7.5 conversions.

/* TEST LIGHT CURTAIN DATA.  Starting from the near end, each light curtain
     diode is bit mapped into 16-bit integers. The bit is set to 1 if the
     diode is dark. */
typedef struct {
    union {
        unsigned short word[NUM_LC_WORDS];
        unsigned char byte[NUM_LC_WORDS][2];
    };
} TEST_LC_DATA;


/* PROFILE_DATA:  Data returned from laser slave which has been reduced by
*                    applying thickness parameters to the raw data.  */
typedef struct {
    unsigned int lead_vne_x;   // inches times 1000 from the beginning of the lug
    unsigned int trail_vne_x;  // for Maximum Wane rule
    unsigned int lead_vne1_x;   // inches times 1000 from the beginning of the lug
    unsigned int trail_vne1_x;  // for Basic Wane rule
    unsigned int lead_thick_x;
    unsigned int trail_thick_x;
    unsigned int lead_scant_t;
    unsigned int trail_scant_t;
    //  int sqr_indication;     // one bit for each width class (bit 0 = width class[0])
} PROFILE_GRADE_THICK;

typedef struct {
    
#if ( ! VER763x )
    unsigned int lead_edge_hole_x;	// hole or split detection
    unsigned int trail_edge_hole_x;	// hole or split detection       
#endif
    
#if ( !VER64xx )
    unsigned int wane_up_or_dn;  
    unsigned int sawn_thick;       // used to find sawn face
#endif
    unsigned int exceeds_bow;      // profile is bowed is TRUE
    unsigned int lead_edge_x;
    unsigned int lead_edge_y;
    unsigned int trail_edge_x;
    unsigned int trail_edge_y;
    unsigned int max_thick;
    PROFILE_GRADE_THICK grade_thick[MAX_GRD_THK_COMBO];
    int cut[MAX_SAW_CUTS];    // place cut will take place for this station - Edger only
    int thk_array[MAX_NUM_CNTS_PER_LUG];	// added 6/2016
} PROFILE_DATA_STN;
typedef PROFILE_DATA_STN PROFILE_DATA[MAX_NUM_LASERS+1]; // stored as part of LAST_BOARD_DATA


// PLAN_DATA:  Leading and trailing edge data from the light curtain.
typedef struct {
    unsigned int lead_edge; // in encoder counts of 1/64 inch
    unsigned int trail_edge;//      ditto
} LC_PLAN_DATA;
typedef LC_PLAN_DATA PLAN_DATA[TOT_DIODES];  // stored as part of LAST_BOARD_DATA


/* LASER_CALIBRATION:  Each laser head can be assigned a constant to correct
*          its range output (y axis) and position on the board (x axis).
*          These values are applied to the raw data by the laser slave.  */
typedef struct {
    int top_range;               // as inches times 1000
    int bot_range;               // as inches times 1000
    unsigned int top_offset;     // always positive
    unsigned int bot_offset;
} LASER_CALIBRATION_STRUCT;
typedef LASER_CALIBRATION_STRUCT LASER_CALIBRATION[MAX_NUM_LASERS];

/* LIGHT CURTAIN CALIBRATION */
typedef struct {
    int beam_width;  // stored encoder counts of 1/64 inch
} LC_CALIB_STRUCT;
typedef LC_CALIB_STRUCT LC_CALIB[TOT_DIODES];
typedef struct {
	int iOffset[TOT_DIODES];
} LC_CALIB_OFFSET;


// structure is an array of the center positions of each laser profile, after rotation is done.
// It is stored as count, range.
typedef struct{  
	int iX;		// Count of the Center position of the profile, After profile is rotated
	int iY;		// Range reading of the center of the profile, After profile is rotated
}CENTERVALUE[MAX_NUM_LASERS];

// a - values define the x,y value to normalize an entire piece, if normalizing to this profile
// b - values define the x,y value to normalize an individual profile to itself.
typedef struct { 
	int Xa0; // a0 x,y will always be the rotation point at a lower elevation in space
	int Ya0;
	int Xa1; // a1 x,y will be the rotation point higher in elevation in space
	int Ya1;
	int Xb0; // b0 x,y will always be the leading edge rotation point
	int Yb0;
	int Xb1; // b1 x,y will always be the trailing edge rotaion point
	int Yb1;
} EVAL_POINTS[MAX_NUM_LASERS];

// status using for chromascan system
typedef enum CSSTATUS
{
	CS_WAIT,	
	CS_READY,
	CS_ERROR
} CS_STATUS;

typedef struct {
	char cUserName[20];
	char cUserPassword[10];
	int  iPermission;
} USER_PROFILE;

#define NUM_USERS 10

/* GENERAL STUFF (to be input at start-up by Hi-Tech ?)  */
typedef struct  {
    char				cust_name[40];			/* customer name */
    char				cust_loc[30];			/* customer location */
    char				cust_passwordADMN[10];	/* customer defined password */
    unsigned long		job_num;				/* Hi_Tech Job number for special stuff */
    int				right_or_left;			/* HAND for specific installation,right=TRUE */
    int				Lug_Spacing;			// Size of lug spacing in inches; can not exceed max defined
    int				UseMachineCenters;		// Specify how to collect QC data
    int				calib_bar_thick;		//Thickness of calibration bar, inches times 1000
    int				calib_bar_width;		//Width     of calibration bar, inches times 1000
    int				language;				// ENGLISH=0, FRENCH=1
    int				plc_io_speed;			/* offer choice of plc/io speed 57k or 114k */
    unsigned long		validation;				/* Reserved for future use */
    USER_PROFILE		UserProfile[NUM_USERS];
    MACHINE_CENTER	machine_center[10+1];	//left hardcoded to 10 so it did not mess up struct size.
    int				CubicFt;				// display as Cubic Ft instead of BrdFt
    int				B800RangeOffset;		// default 17000; some installs require 10000
    float				fDistPerLzEncoderTick;	// default (1.0/LZ_CNTS_PER_INCH_HARD)
    int				iEncoderCntPerInch;		// only on JS50, Encoder resolution adjustable
    int				spare[20];
} CUST_DATA;  // stored in disk file named CUSTDATA.DAT

/* IMPLEMENT_DATA:  Data supplied by the optimizer engine used to tell
/* a decision implementation subsystem the actions specified by the
/* optimizer engine.  It will specify movement of any gates, saws and
/* tipples carry out the decision of the optimizer engine */

typedef struct {
    int block_worth;       // How much are 2'/4'/6' blocks worth?
    int gate_movement;
    // cut with this one?
    int cut_with_saw[MAX_NUM_SAWS];
    // cut with this one?
    int odd_length_saw;
    // open this one?
    int open_tipple[MAX_NUM_SMART_TIPPLES];
    int reman[2];          // needs to be re-edged and possibly resawn
    short wane_up_or_down; // defines how wane is positioned on board
    short PlanerMark;      // Defines which side to mark for planer (top or bottom)
    int spare2;
    int resaw[2];          // needs to be resawn only
    int beg_ele;           // beginning of scanned board data in LC elements
    int end_ele;           // end of scanned board data in LC elements
    int diverter;          // needs to be diverted
    int lc_good;           // true if lc data is valid
    int alarm;             // enunciates alarm
    int stop_chain;        // solution not positionable, inhibits chain
    int bowed_board;       // set for bowed boards
    int fe_gate_movement;  // when ioconfig specified use far end gate
    
    int density;           // density of board
    int MeasuredVolume;    // calculated volume
    int raw_thk;
    int raw_wth;
    int iVisionDataGood;	 // True if vision data is valid.
    int iSpare[15];		 // spares
} IMPLEMENT_DATA;        // Information of how to implement decision

typedef struct {
    char cutting_name[16];
    RECT CuttingSize;
} CUTTING_SOL_STRUCT;

typedef struct {
	int iNumCuttings;		// number of cuttings found for this solution
	int iYieldPercent;		// Achieved yield of this solution  Percent*10;  87.4 = 874
	CUTTING_SOL_STRUCT CUTTINGS[MAX_NUM_SOL_CUTS];	// type and size
} CUTTING_SOL;

typedef struct {
    int category;             // category classification
    int product;              // product classification
    int rdm_width;            // Raw Width for Random Width solutions.
    int grade;                // grade classification
    int thick;                // thickness classification
    int width;                // width classification
    int length;               // length classification
    int board_relative_worth; // price or volume
    int beg_station;          // beginning station of this piece
    int actual_len;           // actual length of solution
    int bias;                 // gives general position of piece
    int pass_const;           // tells whether piece needs resaw, re-edge, etc.
    int beg_square;           // beginning of square section
    int len_square;           // length of square solution
    int twist;				// twist in product
    int bow;					// bow in product
    int crook;				// crook in product
    int EqFaceWane;
    int EqEdgeWane;
    int PctClrFace;			// percentage of clear face in product.
    int Volume;				// Volume of cut length - used for accustic graders
    int CutLength;			// Cut length - used for accustic graders
    int spare[14];
} SOL_SPEC_BRD;
typedef SOL_SPEC_BRD SOLUTION_SPEC[2];  // set for 2 board solutions

typedef SOLUTION_SPEC  TOP_SOL_SPEC[MAX_NUM_SAVED_SOL];  //save top solutions for reference

/************* input output maps  **************************/
#define IO_IN	0
#define IO_OUT -1

typedef struct {
    int addr;
    int len;
    int in_or_out;
} INPUT_OUTPUT_MAP;

/* To add new items to this list, see instructions in the file ioconfig.c.
   All items in this list are set via input in ioconfig.c which is called
   thru AF12 from EXEC UTILITIES menu */
typedef struct {
    INPUT_OUTPUT_MAP encoder;              // Electro - Cam sequencer (8 max; input)
    INPUT_OUTPUT_MAP second_saw_sol;       // If more than 16 Saws (8 max; output)
    INPUT_OUTPUT_MAP saw_solenoids;        // Trim Saws (16 max; output)
    INPUT_OUTPUT_MAP tipple_solenoids;     // 1st. set of tipples (8 max-o)
    INPUT_OUTPUT_MAP gate_solenoids;       // fence (8 max-o)
    INPUT_OUTPUT_MAP fe_gate_solenoids;    // far-end fence; Canadian (8 max-o)
    INPUT_OUTPUT_MAP category;             // binary value of cat_pb (3-o)
    INPUT_OUTPUT_MAP cut_length;           // cut_len of sol. inches*10 (16-o)
    INPUT_OUTPUT_MAP nom_length;           // nom_len of sol. inches*10 (16-o)
    INPUT_OUTPUT_MAP slash;                // bit indicates no brd value(1-o)
    INPUT_OUTPUT_MAP conveyor_enable;      // (1-o)
    INPUT_OUTPUT_MAP mark_board;           // mark a board for test (1-o)
    INPUT_OUTPUT_MAP alarm;                // board out of spec (1-o)
    INPUT_OUTPUT_MAP fe_gate_action;       // Gate action (# gate incr.) (7-i)
    INPUT_OUTPUT_MAP display_select;       // Select Vorne display option (3-i)
    INPUT_OUTPUT_MAP saws_to_slash;        // Saw locations with lumber (16-o)
    // Select man inputs as override/preselect (1-i)
    INPUT_OUTPUT_MAP manual_preselect_toggle;
    INPUT_OUTPUT_MAP ne_cutoffs;           // Near end cutoffs (4-i)
    INPUT_OUTPUT_MAP fe_cutoffs;           // Far end cutoffs (4-i)
    INPUT_OUTPUT_MAP gate_action;          // Gate action (# gate incr.) (7-i)
    INPUT_OUTPUT_MAP nom_thick;            // Nominal thickness passed to sorter (16-o)
    INPUT_OUTPUT_MAP nom_width;            // Nominal width passed to sorter (16-o)
    INPUT_OUTPUT_MAP skewed_board;         // no solution; brd skewed (1-o)
    INPUT_OUTPUT_MAP resaw;                // ne or fe resaw (2-o)
    INPUT_OUTPUT_MAP reman;                // ne or fe reman (2-o)
    INPUT_OUTPUT_MAP cat_pb;               // input category pb (5-i)
    INPUT_OUTPUT_MAP grd_input;            // grd pb input (16-i)
    INPUT_OUTPUT_MAP zone_diagnostic;      // res_opt zone feedback (3-o)
    INPUT_OUTPUT_MAP output_grd_pb;        // binary value of grd_pb for ne (5-o)
    INPUT_OUTPUT_MAP alarm_1_and_2;        // alarm1 and alarm2 (2-o)
    INPUT_OUTPUT_MAP alarm_slash_reject;   // alarm for consec. slashes or reject (3-o)
    INPUT_OUTPUT_MAP divert_reject;        // Flag to PLC for Divert Parcel (1-o)
    INPUT_OUTPUT_MAP sec_saws_to_slash;    // more that 16 saws (8-o)
    INPUT_OUTPUT_MAP machine_center_select; // (4-i)
    INPUT_OUTPUT_MAP odd_length_saw;       // (1-o)
    INPUT_OUTPUT_MAP output_grd_pb_2;      // binary value of grd_pb for fe(5-o)
    INPUT_OUTPUT_MAP scanner_alarm;        // (8-o) signal plc if scanner malfunctions
    INPUT_OUTPUT_MAP weight_input;         // (16-i) wieght used for density
    INPUT_OUTPUT_MAP species_output;       // (3-o) species used with wieght
    INPUT_OUTPUT_MAP density;              // (10-o) Denisty lb/ft3*10
    INPUT_OUTPUT_MAP test_board;           // (1-i) Used with acquire_mode for board test
    INPUT_OUTPUT_MAP random_length;        // (1-o) Flag to indicate solution is Random Length
    INPUT_OUTPUT_MAP no_trim;              // (1-i) input to handle no-trim case
    INPUT_OUTPUT_MAP force_trim;           // (1-i) input to disable reman and resaw, forces trim solution
    INPUT_OUTPUT_MAP fe_grade_input;       // (5-i) grd pb input (binary number 1-16)
    INPUT_OUTPUT_MAP cut_in_two_loc;       // (4-i) input to define cut-in-two location (binary number 1-15ft).
    INPUT_OUTPUT_MAP center_defect_trim;   // (3-i) input to define center defect trim  (binary number 1-7ft).
    INPUT_OUTPUT_MAP brd_volume;           // (16-o) output for volume of raw length brd
    INPUT_OUTPUT_MAP wane_up_or_down;      // (2-o) LSB-wane down; MSB-wane up
    INPUT_OUTPUT_MAP OverThkForTrimmer;    // (1-o) Output turns on when boards is to thick to pass thru Trimmer.
    INPUT_OUTPUT_MAP PLC_WatchDog;         // (4-i) Input counter from PLC is repeated back in Block Xfer.
    INPUT_OUTPUT_MAP UpTimePct;            // (16-i) Input of Pct up time from PLC. Displayed on Vorne. Format 78.2% = 782
    INPUT_OUTPUT_MAP B8_COM_Lost;          // (1-o) Output to tell PLC if B8 ENET Com Lost
    INPUT_OUTPUT_MAP MarkForPlaner;        // (2-o) Output to define best planer orientation LSB=Top; MSB=Bottom.			
    INPUT_OUTPUT_MAP Grader_ID;            // (4-i) Input of Grader ID; Binary value of (1-15)			
    INPUT_OUTPUT_MAP CalModeBit;           // (1-o) Output to tell PLC System is in Calibrate Mode
    INPUT_OUTPUT_MAP Lz_On_Flag;           // (1-i) Input: 1=LZ ON; 0=LZ OFF
    INPUT_OUTPUT_MAP Lz_Pwr_ON;			 // (1-o) Output: Flag to PLC; Scanner is ON and READY
    INPUT_OUTPUT_MAP PLC_WatchDog_Out;     // (4-o) Output counter to PLC is repeated back.
    INPUT_OUTPUT_MAP PalletRunMode;		 // (8-i) Input Special for Beasley Job 1429-01.
    INPUT_OUTPUT_MAP InputLugID;			 // (16-i) Input Lug ID from PLC; passed back to PLC with output solution
    INPUT_OUTPUT_MAP spare[NUM_SPARES];    // NUM_SPARES defined above
} IO_MAP;

/* if new items are added, ioconfig.c must be revised to include name in the
     list. */

typedef struct {
    char input[MAX_NUM_PAMUX_BRDS*4] ;
    char output[MAX_NUM_PAMUX_BRDS*4] ;
} IO_BIT_MAP;

/* MANUAL_PRESELECTS specify when an operator pre-selects a cutoff or gate
   action to remove a visual defect */
typedef struct {
    int ne2;        // cut 2' off near end
    int ne4;        // cut 4' off near end
    int ne6;        // cut 6' off near end
    int ne8;        // cut 8' off near end
    int fe2;        // cut 2' off far end
    int fe4;        // cut 4' off far end
    int fe6;        // cut 6' off far end
    int fe8;        // cut 8' off far end
    int PalletRunMode; // job specific input for Beasley 1429-01
    int ne_cutoff;  // position in lc elements of selected NE cutoffs
    int AutoNoTrim; // Board is at min length; force an Auto No Trim.
    int fe_cutoff;          // position in lc elements of selected FE cutoffs
    int CenterDefectTrim;   // length of center defect trim
    int gate;               // perform programmed gate action
    int category;     // input Category from operator
    int fe_gate;      // perform programmed gate action on fe
    int graderID;     // ID of Grader; tracks which grader gave input.		
    int species;	  // Species given by operator
    int grade_input;  // grade input for ne or full length
    int weight;	      // weight of board from PLC
    int CenterCut;    // location of forced cut-in-two
    int test_board;   // board was marked for board test
    int force_trim;   // reman and resaw disabled for this board solution.
    int no_trim;      // board marked as no-trim
    int fe_grade_input; // grade input for fe
    int rdm_length_prd;	// force output of random length data.
    int InputLugID;		// Input Lug ID from PLC to track solution.
    int spares[14];
} MANUAL_PRESELECTS;

// It used overlay the spares in MANUAL_PRESECT, before expansion
// times should be stored in uSeconds
typedef struct
{
    union
    {
        struct
        {
            unsigned int engine1;				// 0
            unsigned int engine2;				// 1
            unsigned int engine3;				// 2
            unsigned int engine4;				// 3
            unsigned int engine5;				// 4
            unsigned int engine6;				// 5
            unsigned int engineTotalSolution;	// 6
            unsigned int CS3x00_SocketTLz;		// 7 Top Lasers
            unsigned int CS3x00_SocketTVz;		// 8 Top Vision
            unsigned int CS3x00_SocketBLz;		// 9 Bottom Lasers
            unsigned int CS3x00_SocketBVz;		// 10 Bottom Vision
            unsigned int CS3x00_SocketTotal;	// 11 total Socket Read Time
            unsigned int CS3x00_FindDefects;	// 12 time to find defects (uplift)
            unsigned int CS3x00_ParseTime;		// 13 total CS3x00 time; time from first socket read until uplift processing is complete
            unsigned int DefectEval;			// 14 Time to DefectEval for Grade Defects w/Vision Present
            unsigned int Lz_Rotate;				// 15 Time to process bow and twist in engine1
        } ByName;
        unsigned int ByPhase[32];	// There are some Un-named Spares(16)
    } Value;
} TIMING;


/* GOOD_WOOD_ELE points to the beginning and length of good wood boundaries
/* for a grade_thick-width-pass_const combo. Sized for two boards. */
typedef struct {
    int beg;
    int len;
    int beg1;
    int len1;
} GOOD_WOOD_ELE;

/* GOOD_WOOD_LEN stores the beginning and length of good wood boundaries
/* for each grade_thick-width-pass_const combo. Sized for two boards. */
typedef struct {
    GOOD_WOOD_ELE product[MAX_NUM_PRODUCTS];
} GOOD_WOOD_LEN_STRUCT;

typedef GOOD_WOOD_LEN_STRUCT GOOD_WOOD_LEN[4];

/* NO_WANE_LEN stores the beginning and length of no wane boundaries
/* for each grade_thick-width-pass_const combo. Sized for two boards. */
//typedef struct {
//    GOOD_WOOD_ELE product[MAX_NUM_PRODUCTS];
//} 
typedef GOOD_WOOD_LEN_STRUCT NO_WANE_LEN[4];

/* PASS_WOOD_LEN stores the beginning and length of good wood ignoring % wane
/* for each grade_thick-width-pass_const combo. Sized for two boards. */
//typedef struct {
//    GOOD_WOOD_ELE product[MAX_NUM_PRODUCTS];
//} 
typedef GOOD_WOOD_LEN_STRUCT PASS_WOOD_LEN[4];

typedef struct {         // 0 = overall_pass, 1 = over max thk
    int w_pass[MAX_NUM_PRODUCTS]; // 2 = over max width, 4 = under min width
} BOOLEAN_PF;            // 8 = under min face or any combination thereof
typedef BOOLEAN_PF BOOLEAN_PF_MAP[MAX_NUM_LASERS+1];  // was MAX_NUM_PROFILES

// typically tally stored to database every 10 minutes.
// to track the last 60 minutes, we will store the last six 10 minute intervals.
// a time value will be stored in struct for actual reference.
#define NUM_SNAP_TALLY 6

typedef struct {
    time_t			logged_time;		// time/date last updated 
    unsigned long int bdft_in;			// amount of lumber entering optimizer
    unsigned long int bdft_out;			// amount of lumber to sorter
    unsigned long int bdft_slash;			// amount of lumber slashed
    unsigned long int bdft_reman;			// amount of lumber sent to be re-edged
    unsigned long int bdft_resawn;		// amount of lumber sent to be resawn only
    unsigned long int in;					// # coming into system excluding reman&resaw
    unsigned long int out;				// # going to sorter(T)/trimmer(E)
    unsigned long int num_slashes;		// # slash into two ft blocks
    unsigned long int num_resawn;			// # boards to be resawn only
    unsigned long int num_reman;			// # boards to be re-edged
    unsigned long int num_skews;			// # skews  (adds to total in)
    unsigned long int spare[9];			// reserved for future use
} SNAP_TALLY;

/* TALLY_DATA:  Data supplied by the optimizer engine used to leave a paper
/* trail of the decisions the optimizer engine made and report production
/* to mill management */
typedef struct {
    struct {
        unsigned long int in;			// # coming into system including reman&resaw
        unsigned long int out;		// # going to sorter(T)/trimmer(E)
        unsigned long int wth_in;		// total width of pieces coming into system including reman&resaw (inches*1000)
        unsigned long int wth_out;	// total width of pieces going to sorter(T)/trimmer(E) (inches*1000)
        unsigned long int len_in;		// total length of pieces coming into system including reman&resaw (inches*10)
        unsigned long int len_out;	// total length of pieces going to sorter(T)/trimmer(E) (inches*10)
        
        // next three specify why trim took place
        unsigned long int num_scant_t;  // # trims for scant thk
        unsigned long int num_scant_w;  // # trims for scant wth
        unsigned long int num_waney;    // # trims for real wane
        
        unsigned long int num_slashes;  // # slash into two ft blocks
        unsigned long int num_resawn;   // # boards to be resawn only
        unsigned long int num_reman;    // # boards to be re-edged
        unsigned long int num_1ft_cutoff;// # 1-ft cutoffs
        unsigned long int num_2ft_cutoff;// # 2-ft cutoffs
        unsigned long int num_3ft_cutoff;// # 3-ft cutoffs
        unsigned long int num_4ft_cutoff;// # 4-ft cutoffs
        unsigned long int num_6ft_cutoff;// # 6-ft or greater cutoffs
        unsigned long int num_trimsave;     // # trim save boards
        unsigned long int num_cut_in_two;   // # boards cut into two boards
        unsigned long int bdft_trimmed;	// used to calculate trim-loss by product
        
        // Begin adds for Edger only
        unsigned long int num_passes;   // # boards passed from edger portion
        unsigned long int num_nonpositional; // # boards can't be positioned
        // # boards where depth of cut of side head exceeded limit
        unsigned long int num_exceed_depth_of_cut;
        // # boards where depth of cut of top head exceeded limit
        unsigned long int num_exceed_top_depth_of_cut;
        // # boards where the needed stroke was greater than upper limit
        unsigned long int num_stroke_upper_limit;
        // # boards where the needed stroke was less than lower limit
        unsigned long int num_stroke_lower_limit;
        // # boards that would have hit lead side plate
        unsigned long int num_limit_lead_plate;
        // # boards that would have hit trail side plate
        unsigned long int num_limit_trail_plate;
        int spare[16];
    } product[MAX_NUM_PRODUCTS][MAX_NUM_LEN];
    unsigned long int num_trimsave;
    unsigned long int num_cut_in_two;   // # boards cut into two boards
    unsigned long int num_man_ovrd;     // # overrides by operator;
    unsigned long int num_lugs;         // total number of lugs; 
    
    /* next six items are in units of board-feet times 100 */
    unsigned long int bdft_in;          // amount of lumber entering optimizer
    unsigned long int bdft_out;         // amount of lumber to sorter
    unsigned long int bdft_slash;       // amount of lumber slashed
    unsigned long int bdft_in_trough;   // amount of lumber into trough, slashes + cutoffs
    unsigned long int bdft_reman;       // amount of lumber sent to be re-edged
    unsigned long int bdft_resawn;      // amount of lumber sent to be resawn only
    unsigned long int ActualBdFtOut;    // Actual bdft includes actual length of Random Lengths
    unsigned long int bdft_passed;      // amount of lumber passed - Edger Only
    
    time_t			clear_time;    /* time/date last cleared, DOS format; changed from long to time_t in VC2005 */
    time_t			time_running;  /* same as res_data.time_running */
    time_t			time_enabled;  /* same as res_data.time_enabled */
    unsigned long int num_skews; // number of skew-in-lugs
    unsigned long int RoundedBdFtOut;    // Bdft_out rounding each board to the nearest whole bdft
    unsigned long int LPM;			// Lugs Per Minute
    unsigned long int PulsesPS;		// Encoder Pulses Per Second
    
    // Begin adds for Edger only
    unsigned long int in;  // # coming into system excluding reman&resaw
    unsigned long int out;              // # going to sorter(T)/trimmer(E)
    unsigned long int num_slashes;      // # slash into two ft blocks
    unsigned long int num_resawn;       // # boards to be resawn only
    unsigned long int num_reman;        // # boards to be re-edged
    unsigned long int num_passes;       // # boards passed from edger portion
    unsigned long int num_nonpositional; // # boards can't be positioned
    unsigned long int num_reruns;       // # strips from 2-saw machine rerun
    unsigned long int num_single_brd;   // total number of single board solutions
    unsigned long int num_two_brd;      // total number of two board solutions
    unsigned long int num_three_brd;    // total number of three board solutions
    // Begin add for Lampe & Malphrus
    unsigned long int num_flitch;               // # flitches
    unsigned long int num_pass;                 // # "pass" flitches
    // for these four, array element 0 is 5/4, 1 is 7/4
    unsigned long int num_three_board_cant[2];  // # single triples
    unsigned long int num_six_board_cant[2];    // # double triples
    unsigned long int num_nine_board_cant[2];   // # double and single triples
    unsigned long int num_twelve_board_cant[2]; // # double double triples
    // for these two, array element 0 is 4" cant, 1 is 6" cant
    unsigned long int num_timber1_cant[2];      // # smaller timbers
    unsigned long int num_timber2_cant[2];      // # larger timbers
    // End add for Lampe & Malphrus
    time_t uLastScanTime;
    time_t uProcessTime;
    unsigned long int u2Pass;
    
    SNAP_TALLY		SnapTallyData[NUM_SNAP_TALLY];		// snap shot of last 60 minutes in 10 minute chuncks.
    
    int spares[16];
} TALLY_DATA_STRUCT;
typedef TALLY_DATA_STRUCT TALLY_DATA[MAX_NUM_CATEGORY];   // Production tally information


#define MAX_NUM_PF_ITEMS	24

// These items are Length Specific (32 reasons allowed)
#define	pfTWIST				0x00000001	// Failed to Meet Twist Requirements
#define pfBOW				0x00000002	// Failed to Meet Bow Requirements
#define pfCROOK				0x00000004	// Failed to Meet Crook Requirements
#define pfSQR_END			0x00000008	// Failed to Meet Sqr End Requirements
#define pfBASIC_WANE		0x00000010	// Not Enough GW Length for Basic Wane Requirements
#define pfPRICE				0x00000020	// More valuable solution found
#define pfMAX_WANE			0x00000040  // Exceeds Max Wane Limits for this Minimum Length
#define pfEQ_WANE			0x00000080  // Failed to Meet Equivalent Wane Area
#define pfSURFACE_AREA		0x00000100  // Failed to Meet Equivalent Wane Area
#define pfWANE_ON_2_EDGES	0x00000200	// Failed because wane was detected on two edges.
#define pfMAX_ORDER_FILLED	0x00000400  // Product has reached max bdft out allowed for this shift
#define pfSHORT_LENGTH		0x00000800  // Board physically too short to make this length
#define pfTAPER				0x00001000  // Failed NE-FE allowed taper for random width
#define pfNO_PRD_RESAW		0x00002000  // This Product not allowed for Resaw
#define pfNO_PRD_REEDGE		0x00004000	// This Product not allowed for Re-edge
#define pfNO_SAW_OR_GATE	0x00008000	// This Product mechanically limited by gate and saw options

// These items are product Specific (32 reasons allowed)
#define pfNOTCH_FOUND		0x00000100	// Dectect Notch in Light Curtian data
#define pfSPARE1			0x00000200	// spare : replaced meaning
#define pfBELOW_MIN_THK		0x00000400  // Thickness is below minimum required
#define pfBELOW_MIN_WTH		0x00000800  // Width is below minimum required
#define pfINPUT_GRADE		0x00001000  // Grade PB# not allowed for the input grade.
#define pfNO_RESAW			0x00002000  // Resaw is not an option for this thickness class
#define pfOVER_MAX_WIDTH	0x00004000	// Width is over maximum allowed 
#define pfOVER_MAX_THICK	0x00008000	// Thick is over maximum allowed
#define pfFAILED_VISION		0x00010000	// Failed to pass vision parameter tests.

// These items are non-product related.
#define pfMAX_LEN_TRIMMER	0x00010000	// Exceeds Max Length to Trimmer; forcing gate trim
#define pfMAX_LEN_EDGER		0x00020000	// Exceeds Max Length to Edger
#define pfMANUAL_PRESELECT	0x00040000	// Manual Preselect given for solution
#define pfMIN_LEN_CONVEY	0x00080000  // Overall Length is below Minimum Len Conveyable
#define pfMIN_LEN_TO_EDGER	0x00100000  // Overall Length is below Minimum Len Conveyable at Edger


/* PASS_FAIL_FLAGS stores information regarding what optimizer found
/* as it processed the solution.  Stores info for each product and length.*/

/* expanded from short to int so more pass-fail reason could be used.  sjf 6/15/2016 */
typedef struct {
	int pf_len;  //Product-Length related pass-fail issues.
} LEN_PF_MAP;

typedef struct {
	int pf_prd;  //product related pass-fail issues.
    LEN_PF_MAP Len[MAX_NUM_LEN];  
} PRD_PF_MAP;

typedef struct {
    PRD_PF_MAP Prd[MAX_NUM_PRODUCTS];  
	int Other;  //non-product related pass-fail issues.
} SOL_PF_MAP;

// LAST BOARD DATA
typedef struct {
    unsigned int io_num;
    PLAN_DATA plan_data;			// 11-30-17 (raw plan_data)
    PLAN_DATA smooth_plan_data;	// 11-30-17 (New Addition)
    PROFILE_DATA profile_data;
    PASS_WOOD_LEN pass_wood_len;
    NO_WANE_LEN no_wane_len;
    GOOD_WOOD_LEN good_wood_len;
    IMPLEMENT_DATA implement_data;
    SOLUTION_SPEC solution_spec;
    ERROR_CODE error_code;
    MANUAL_PRESELECTS manual_preselects;
    TIMING Timing;
    SOL_PF_MAP sol_pf_map;
    CENTERVALUE iCenterTopValue;
    RAW_LASER_DATA NormRawLasersBottom;
    RAW_LASER_DATA NormRawLasersTop;
    TOP_SOL_SPEC TopSolSpec;
    
#if (NUM_LZ_PER_UNIT == 8)
    SCANNEDOBJECTCOPY LaserBuffer;  // B8/B900 data
#endif
    
    int Twist[MAX_NUM_LASERS];
    int iEdgeThkData[MAX_NUM_LASERS+1];
    int input_grade;
    int input_machine;
} LAST_BOARD_DATA;


typedef struct{
    unsigned ram_check;
    time_t time_running;  // in timer ticks
    time_t time_enabled;  // in timer ticks
} RES_DATA;

typedef struct {
    unsigned int io_num;  // current board number
    int zone;             // current zone
} POSITION_STATUS;


typedef struct {
	char thk_name[16];
	int index;              // used for display sorting
	unsigned int spare;     // inches * 1000
	unsigned int target_t;  // inches * 1000
	int alarm1_plus;        // inches * 1000
	int alarm1_minus;       // inches * 1000
	int alarm2_plus;        // inches * 1000
	int alarm2_minus;       // inches * 1000
} QC_THK_ALARMS;

typedef struct {
	char wth_name[16];
	int index;              // used for display sorting
	unsigned int spare;     // inches * 1000
	unsigned int target_w;  // inches * 1000
	int alarm1_plus;        // inches * 1000
	int alarm1_minus;       // inches * 1000
	int alarm2_plus;        // inches * 1000
	int alarm2_minus;       // inches * 1000
} QC_WTH_ALARMS;

typedef struct {
    int index;           // used for display sorting
    int spare;          // inches times 10
    int qc_min_len;     // inches times 10
    int qc_target_len;  // inches times 10
    int qc_max_len;     // inches times 10
} QC_LEN_ALARMS;

typedef struct {
	char prfl_name[16];
	int index;              // used for display sorting
	unsigned int target_t;  // inches * 1000
	unsigned int target_w;  // inches * 1000
	int alarm1_plus;        // inches * 1000
	int alarm1_minus;       // inches * 1000
	int alarm2_plus;        // inches * 1000
	int alarm2_minus;       // inches * 1000
	int spare[4];			// future; may allow alarm for Thk and Wth seperatly
} QC_PRFL_ALARMS;

typedef struct {
	QC_THK_ALARMS thk[MAX_GRADE_THICK_MC];
	QC_WTH_ALARMS wth[MAX_WIDTH_MC];
	QC_LEN_ALARMS len[MAX_NUM_LEN];
	QC_PRFL_ALARMS profile[MAX_QC_PRFL_MC];
} QC_ALARMS;  // used by quality control program

// used to save QC Screen format data
typedef struct {
	float fMeanAxisRange;
	float fMeanAxisDelta;
	float fSigmaAxisRange;
	float fSigmaAxisDelta;
} QC_TEN_BRD;

typedef struct {  // 0-thick view, 1-width view
	QC_TEN_BRD ten_brd_view[2];
	float five_pt_view[2];
	bool hist_view[2];
} QC_CONFIG;


/* QC data for the last 10 square edge boards of each width catagory,
   and the last 10 boards for each thickness ignoring width.
   Engine updates with each board. 
   *** THIS VALUE MAY INCREASE TO SAMPLE LARGER DATA SET  ***/
#define NUM_QC_BRD_SAMPLES	25	


/* These values not intended to change, 
   requires code changes in optwin and engine
   if these ever change from 6 and 5. */
#define NUM_QC_THK_SAMPLES	6	// THIS VALUE MUST ALWAYS BE 6
#define NUM_QC_WTH_SAMPLES	5	// THIS VALUE MUST ALWAYS BE 5

typedef struct {
	struct {
        int wth_measure[NUM_QC_WTH_SAMPLES];    // inches times 1000
        int reject_flg;
        int reedge_flg;
        int resaw_flg;
        int slash_flg;
        long  int trimbdf;
        long  int totbdf;
	} board_num[NUM_QC_BRD_SAMPLES];
	int next_num;            // next element of board_num to fill
	int do_trend;            // flag that 10 boards are full
	int target_w;
} QC_WIDTH;

typedef struct  {
	struct {
        int thk_measure[NUM_QC_THK_SAMPLES];    // inches times 1000
        int reject_flg;
        int reedge_flg;
        int resaw_flg;
        int slash_flg;
        long  int trimbdf;
        long  int totbdf;
	} board_num[NUM_QC_BRD_SAMPLES];
	int next_num;            // next element of board_num to fill
	int do_trend;            // flag that 10 boards are full
	int target_t;
} QC_THICK;

typedef struct {
	struct {
        int thk_measure[NUM_QC_THK_SAMPLES];    // inches times 1000
        int wth_measure[NUM_QC_WTH_SAMPLES];    // inches times 1000
        int reject_flg;
        int reedge_flg;
        int resaw_flg;
        int slash_flg;
        long  int trimbdf;
        long  int totbdf;
	} board_num[NUM_QC_BRD_SAMPLES];
	int next_num;            // next element of board_num to fill
	int do_trend;            // flag that 10 boards are full
	int target_t;
	int target_w;
} QC_PROFILE;  // used by quality control program/machine centers

typedef struct {
	QC_WIDTH	width[MAX_NUM_MACHINE_CENTERS+1][MAX_WIDTH_MC];
	QC_THICK	thick[MAX_NUM_MACHINE_CENTERS+1][MAX_GRADE_THICK_MC];
	QC_PROFILE	profile[MAX_QC_PRFL_MC];
} QC;  // used by quality control program/machine centers


/* Length of every square edged board */
typedef struct {  // used by QC length program
    unsigned int count[NUM_LEN_COUNTS]; /* sized for MIN_LEN_BRD to MAX_LEN_BRD+1 by 1/2-inches.
       MIN_LEN_BRD-0" to MIN_LEN_BRD-0.49" is array element 0, etc */
}  LENGTH_IN; 

/* Thickness trend data. Engine adds new set every 10-minutes (10 boards for
   machine centers > 1) */
typedef struct   {
    float thk_mean;
    float thk_within;
    float thk_between;
    int num_re_edged;
    int num_resawn;
    int num_reject;
    int num_slash;
    int bf_trim;
    int bf_input;
    int spares[10];
} TREND_THICK;

typedef struct   { 
    float wth_mean;
    float wth_within;
    float wth_between;
    int num_re_edged;
    int num_resawn;
    int num_reject;
    int num_slash;
    int bf_trim;
    int bf_input;
    int spares[10];
} TREND_WIDTH;

typedef struct   { 
    float thk_mean;
    float thk_within;
    float thk_between;
    float wth_mean;
    float wth_within;
    float wth_between;
    int num_re_edged;
    int num_resawn;
    int num_reject;
    int num_slash;
    int num_samples;
    int bf_trim;
    int bf_input;
    int spares[10];
} TREND_PROFILE;


// system statistics tracked over the shift
// pct values are *10; ex.  14.7% = 147
// defined as shorts to save memory; values never over 1000 = 100.0%
typedef struct {
    short pctFullLugs;		// pct number of full lugs
    short pctTimeRunning;	// pct minutes chain has been running
    short pctTrimLoss;		// pct trim loss
    short pctEdge;			// pct number of boards edged
    short pctResaw;			// pct number of boards resawn
    short pctReject;		// pct number of boards re-edged AND/OR resawn
    short pctSlash;			// pct number of boards Slashed
    short pctSkew;			// pct number of skews detected.
    short pctSpare[4];
    int   BdftOut;			// Bdft Out during interval
    int   Spare[4];
} TREND_SYSTEM;

typedef struct {   // used by QC shift trend program
	TREND_THICK thick[MAX_NUM_MACHINE_CENTERS+1][NUM_INTERVALS][MAX_GRADE_THICK_MC];
	int next_tnum[MAX_NUM_MACHINE_CENTERS+1][MAX_GRADE_THICK_MC];   // next interval to fill
    
	TREND_WIDTH width[MAX_NUM_MACHINE_CENTERS+1][NUM_INTERVALS][MAX_WIDTH_MC];
	int next_wnum[MAX_NUM_MACHINE_CENTERS+1][MAX_WIDTH_MC];         // next interval to fill
    
	TREND_PROFILE profile[NUM_INTERVALS][MAX_QC_PRFL_MC];
	int next_pnum[MAX_QC_PRFL_MC];         // next interval to fill
    
	TREND_SYSTEM system[NUM_INTERVALS];
	int next_sysnum;	// next interval to fill
} TREND;


typedef struct { // used to transfer data between resident and transient for special tests
    int test[4];
} SPL_TEST;

typedef struct TAG_SORTED_THICK{
    /* NOTE:  This structure is repeated in dn_load.c.  Any changes made here
       must be duplicated in dn_load.c   */
    unsigned int thk;    // thickness in thousandths
    int t;               // grd-thk combo
    int ele;             // 0=min_t, 1=min_vne, 2=min_vne1, 3=scant_t
}SORTED_THICK[MAX_GRD_THK_COMBO * 4];  // four elements stored per thick

/* Need Grade-Thick combinations to find thickness break points for 
    each laser profile.  Sort with Smallest mon_t, Smallest min_t, and
    Smallest min_vne1 at array element Zero*/
typedef struct {  
    int index;                    // used for sorting
    unsigned int min_t;           // inches*1000
    unsigned int min_vne;         // maximum wane;  inches*1000
    unsigned int min_vne1;        // basic wane allowed
    unsigned int grd_thk_mask;    /* grade element number is MSB; thick element number is LSB */
    int spare[3];
} GRD_THK_COMBO;


/* Board Product contains full discription for manufactured products */
/*** Items marked with '@' filled at dn_load ***/

/* Thickness Discription. Array reference used by BRD_PRODUCTS */
typedef struct {
    char thk_name[16];
    int index;              // used for display sorting
    unsigned int nom_t;     // inches * 1000
    unsigned int min_t;     // inches * 1000
    unsigned int max_t;     // inches * 1000
    unsigned int target_t;  // inches * 1000
    unsigned int final_t;   // inches * 1000
    int resaw_available;    // reduce thk to make this product?
    int SB_OK;              // Sharp Chain Only      
    int UseSawnFace;        // measure face at sawn thk instead of min_thk
    int spare[16];      
} T_CLASS;  

/* Width Discription. Array reference used by BRD_PRODUCTS */
typedef struct {
    char wth_name[16];
    int index;              // used for display sorting
    unsigned int nom_w;     // inches * 1000 - random width set to zero
    unsigned int min_w;     // inches * 1000
    unsigned int max_w;     // inches * 1000
    unsigned int target_w;  // inches * 1000
    unsigned int final_w;   // inches * 1000
    int iRdmWthTaperAllowed;  //inches * 1000; amount of Taper allowed on Pass boards    
    int spare2;             //
    int iShrinkageAllowance;// Random width shrinkage allowance - pct times 10
    int iPushPullSetting;   // 0 = Disabled, 1 = Only Pull Saws In, 2 = Only Push Saws Out, 3 = Push or Pull Saws.
    int width_pushbutton;   // Edger Only
    int spare[16];   
} W_CLASS;                   

/* Length Discription. Array reference used by BRD_PRODUCTS */
typedef struct {
    char len_name[16];
    int index;           // used for display sorting
    int nom_len;        // inches times 10
    int min_len;        // LC Elements
    int max_len;        // LC Elements
    int cut_len;        // LC Elements - Trimmer Only
    int odd_len;        // TRUE or FALSE - Trimmer Only
    int spare[5];
} L_CLASS;


typedef struct {
    char cutting_name[16];
    int iValid;			// flag to indicate cutting is being used.
    int iMinWth;			// minimum width of cutting (inches*1000)
    int iMaxWth;			// max width of cutting (inches*1000)
    int iWthIncrements;	// valid increaments to use between min & max (inches*1000)
    // sjf: Changed from inches (10-03-07) 
    int iMinLength;		// min length of cutting (inches * 10)
    int iMaxLength;		// max length of cutting (inches * 10)
    int iLenIncrements;	// valid increments to use between min & max (inches * 10)
    int spare[8];			// future use
} CUTTING_SIZES;

typedef struct {
    int iValid;			// slm: 08-30-07 Added to make easier the entering of data/programming
    int iNumCuttingSizes;	// Num of cutting sizes to be defined for this grade (not to exceed max allowed)
    int iBasicYieldPct;	// surface area of cuttings must yield minimum % of board to make grade
    int iFaceToUse;		// grade bassed on bottom=4, top=3, best=2, poorest=1, or both faces=0
    int iMaxNumCuts;		// Max number of cuts allowed; zero if don't care.
    int spare[16];			// future use
    CUTTING_SIZES	sCutSizes[MAX_NUM_CUTTINGS];
} CUTTINGS;

/* Grade Discription. Array reference used by BRD_PRODUCTS */
typedef struct {
    char wane_rule_name[16];
    int index;           // used for display sorting
    int grade_pushbutton;
    int basic_pct_thick;     // pct times 10
    int basic_pct_width;     // pct times 10
    int max_pct_thick;       // pct times 10
    int max_pct_width;       // pct times 10
    int len_wane_percentage;   //max rule; expressed pct times 1
    int scant_length_pct;     // pct times 1
    int scant_thk;
    int scant_width;
    int eq_pct_thick;  // pct times 10
    int eq_pct_width;  // pct times 10
    int EnableEqWane;  // use equivalent wane for this grade
    int iPctClearFace; // pct times 10
    int WaneAllowedOnOneEdgeOnly;
    int DoNotUseWanePercentages;  //thk and wth values stored as inches*1000 instead of percent
    GradeDefectType iVisionDefectTypes;		// 32 bit field indicating which type of defects to use or ignore for grade
    int MaxKnotDiameter;			// maximum single knot size allowed (diameter)
    int SumKnotDiameterAllowed;	// maximum sum of knots in a given sub length (diameter)
    int SectionLengthForSum;		// length of section defined for SumKnotDiameterAllowed
    int KnotDistanceFromEdge;		// keep knots x distance for edge of board (reduce chance of edge knots)
    int MaxKnotArea;				// maximum single knot size allowed (area)
    int SumKnotAreaAllowed;		// maximum sum of knots in a given sub length (area)
    eSUM_FACES eSumPreference;	// how to sum face(s) for SumKnotDiameterAllowed/SumKnotAreaAllowed
    int spare[7];
    CUTTINGS sCuttings;  // subproduct descriptions for cutting units
} G_CLASS;




typedef struct L1 {  // length data stored in sorted order (all categories are the same)
    int target;   // all length dimensions in inches*10
    int upper;
    int lower;
} SORTED_LEN[MAX_NUM_LEN];


/* Board Product contains full discription for manufactured products */
/*** Items marked with '@' filled at dn_load ***/


typedef struct {
    int lengthID;
    int index;           // used for display sorting
    unsigned int scant_len_allowed; //@ LC elements
    unsigned int end_len_wane_limit;  //LC elements; limits wane dist from end; sqr end length
    int price;                      // price/piece
    unsigned int bdft;              // total bdft for product
    int product_enable;             // 1=make product, 0 = disable, see also enum ProductOptions 
    int CurveLimitID;               // Cant and Sharp Chain Only
    int iBow;                       // bow allowed for this length; inches*1000
    int iCrook;                     // crook allowed for this length; inches*1000
    int iTwist;                     // twist allowed for this length; inches*1000
    int iSetAssignment;			  // Sorter Set Assignment
    int iMaxOrderBdFt;			  // Stop making this product once X BdFt reached
    int iSqrEndStartDist;			  // how far in to start checking for sqr end length
    int spare[2];
} LENGTH_DATA;

typedef struct {
    int valid;      // set TRUE when data is valid for use
    int index;           // used for display sorting
    int gradeID;
    int thickID;
    int widthID;
    LENGTH_DATA LenData[MAX_NUM_LEN];
    int combo_num;     //@ array element to matching GRD_THK_COMBO/GRD_WTH_COMBO
    unsigned int min_face;          //@ maximum wane;  inches*1000
    unsigned int min_face1;         //@ basic wane allowed
    int iAcquireToList;             //  selective acquire by products
    int spare[15];
} BRD_PRODUCTS;   

typedef struct {
	int prdID;
	int lenID[MAX_NUM_LEN];
} SORTED_ID;

/* Category (Species).  Multiple Categories may be defined.  Because of size
   limitations on QC t_class, w_class, and l_class MUST contain the same data
   for all Categories.  g_class can be different for each Category. */
typedef struct {
    int catPB;  /* This is non-ambigious number between 1 and NUM_CATEGORIES
      dn_load will use (this - 1) as array index for category */
    char			category_name[16];
    int			default_grade_push_button;
    int			num_gtc;			// number of grade_thick_combos
    int			lastPriceOption;	// $/MBF or $/piece; controls scrn write default
    int			DownGradeOptions[MAX_NUM_GRADE];  //can work up to 32 Max Num Grades
    int			spare[32];			// future use
    SORTED_ID		sortedID[MAX_NUM_PRODUCTS];	  // sort brd_prd in some defined order
    BRD_PRODUCTS  brd_prd[MAX_NUM_PRODUCTS];
    GRD_THK_COMBO grd_thk_combo[MAX_GRD_THK_COMBO]; // limited grd_thk combinations
    SORTED_THICK  sorted_thk_data;    // thickness sorted increasing order 
    G_CLASS       g_class[MAX_NUM_GRADE];
    T_CLASS       t_class[MAX_NUM_THICK];
    W_CLASS       w_class[MAX_NUM_WIDTH];
    L_CLASS       l_class[MAX_NUM_LEN];
} CAT;
typedef CAT CATEGORY[MAX_NUM_CATEGORY];

typedef struct {
	int shift;
	VARIANT_BOOL IsMetric;
	CAT category[MAX_NUM_CATEGORY];
	TALLY_DATA_STRUCT tally[MAX_NUM_CATEGORY];
} TALLY_DATA_STRUCT_DELTA;
//typedef TALLY_DATA_STRUCT_DELTA TALLY_DATA_DELTA;   // Production tally information

#ifdef USE_DLL
// slm: 03-05-20
// FYI: While working to compile for 64 bit, discovered that code no longer compiles if it is set to Zero
#define USING_EDGER_CODE		1
#else
#ifdef USE_LIB
#define USING_EDGER_CODE	1
#else
#define USING_EDGER_CODE	0
#endif
#endif

// This section brings in definitions imported from the edger environment
// needed to communicate with the DLL (Opt_Str.h - Edger flavor)
// Replace than, the following section with the statement #include "Opt_Str_Common.h"
#if (USING_EDGER_CODE)
#define RULES			CAT
#define SORTED_LIST     CAT


#include "Opt_Str_Common.h"

#include "EdgerSet.h"
#include "EngineParams.h"
#endif


#endif
