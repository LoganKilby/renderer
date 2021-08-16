/*****************************************************************************
|  Name:        |  OPT_STR.H
|  Description: |  Universal header file for optimizer programs
|  $Revision: 1.3 $
|  $Date: 1996/11/22 20:30:02 $
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
//#include "nblib.h"
//#include "simulate.h"
//#include "UpLift.h"					// slm: 08-02-18
#include "enumerations.h"
#include "solution_config.h"
#include "windeftypes.h"
//#include "VisionDefectStruct.h"			// slm: 08-02-18

#ifdef VISION_ENABLED
//#include "TAV_Interface.h"
#endif

#define JCAM_DYNAMIC_LIB	// this setting requires that jcam_dll.dll be distributed with service
//#include "jcam_dll.h"

#define V5_NEW_WAY				1		// Set to Zero for V4 Parameters

#define EDGER					1		// Defines Edger for #ifdef's
#define VER34xx					0		// read board data without sawnthick
#define VER38xx					0		// read boards with 32" lug spacing; changed to 36" in ver39xx
#define VER310xx				0		// read boards of v3.10 or older, Number of Saws in profile_data
#define EDG_56x					0		// for the mean time; can set this to 1 to read/simulate version 5.6x board files. (engine only) use 5.6x optwin
#define USE_BLOCK_XFER			1

#define HELP					0		// set to 1 to display ID numbers in Optwin; Set to 0 for shipping version.

// slm: 05-08-08
// Use this Symbols to quickly change all over the program TIMING information and Resolution
#define ENGINE_TIME_RESOLUTION		(double)1000000.0	// Value stored are in uSeconds
#define DUSE_TO_SHOW_ENGINE_TIME	(double)1000.0		// Divide by this value to Show in mSec. (double)

// customer specific data
#define COMMONWEALTH        0		// SET to 1 for Commonwealth Plywood ONLY! everyone else = 0
#define SOUTHERN            0		// 1086-09 26ft system (M6)
#define _121407				0		// Hancock Pitsfield - has 38" lug space - BMX/M6 system (1214-07)
// ************************************************************************************ 
//   _121407   Any M6_Scanner.exe updates require a manual change to compile with 624 vs 590
// ************************************************************************************

//versions 3.11 and later ship with 550 products as standard. 
#if (!VER310xx && !VER38xx && !VER34xx)
#define HARDWOOD            1		// Hardwood configuration; 550 products; 40 grd_thk
#else
#define HARDWOOD            0		// Hardwood configuration; 550 products; 40 grd_thk
#endif

// define system type
#define STD_20FT_M24                   0   // EXTINCT
//#define STD_B900					   0   // EXTINCT  // (B8 with Light Curtain)

//#define STD_M6                         0  // Standard M6 (up to 26ft)						
//#define STD_B8						   1  // Standard B8 Ethernet System
//#define STD_JOESCAN					   0  // (JoeScan Linear Edger)

//#define STD_3X00					   0  // CS3300 or CS3100
//#define VISION_ENABLED				   0  // Set to one when using CS3300 heads; else zero for lasers only

// used to define resolution of lasers for CS_3X00
// can only have ONE of these set true; others set to zero.
#define _3X00_HALF						1		// 1/2 inch scan density   (edger default)				
#define _3X00_QUARTER					0		// 0.250 inch scan density (trimmer default)

// used for building with OptComm Ethernet PLC5 communications DLL
#define OPTCOMM_DLL						1		// 0 build without DLL support



#define USE_QC              1
#define MIN_WANE_ANGLE  TRUE


#define SLICKCHAIN  3   // no lugs and no zones. iUseSpeedZone = 3

// type of chain system
// set slickchain = 0 and speedzone = 0 for normal zoned operation.
//#if(COMMONWEALTH == 1)
//	#define SLICKCHAIN  1   // no lugs and no zones.
//  #define SPEEDZONE   0   
//#else
//	#define SLICKCHAIN  0   
//  #define SPEEDZONE   0   // speedzone is a way to do the calculations like slick chain but wait until zone 13 
//                          // to catch up and do PLC i/o.  Hazlehurst canter is this way.
//#endif

//****************************************************************************************************
#define LEAD  0
#define TRAIL 1
#define NONE  2
#define BOTH  3

// slm: 3.13.2.7 - For the new Override controls, it makes more sense in this way
#define SQ_NONE  0
#define SQ_LEAD  1
#define SQ_TRAIL 2
#define SQ_BOTH  3

//These values added for Cross-Cut solution tally.
#define TALLY_IN   1  // only tally as in product
#define TALLY_OUT  2  // only tally as out products
#define TALLY_BOTH 3  // tally as both in and out

#define NO_PUSH_PULL	0
#define PULL_SAW			1
#define PUSH_SAW			2
#define PUSH_PULL			3

#define ODD                            1   // Odd Lengths manufactured also
#define EVEN                           2   // Even Lengths ONLY
#define LENGTH                      EVEN   // Odd Length Saw at -1' from gate?
// MIN_LEN_BRD and MAX_LEN_BRD must be even integers
#define MIN_LEN_BRD                    2   // Min length board handled in feet

#if (STD_3X00)
#define MAX_LEN_BRD		               22   // Max length board handled in feet (save memory till 24ft needed)
#elif (SOUTHERN)
#define MAX_LEN_BRD		               26   // Only 26ft edger system to date.
#else
#define MAX_LEN_BRD		               24   // Max length board handled in feet
#endif


#define TRUE                           1
#define FALSE                          0
#define UP                          TRUE
#define DOWN                       FALSE
#define MAX_NUM_SAWS                  24  // Max number of programable saws
#define MAX_SAW_CUTS				  5  // edger cut boundaries; 5 saws.
#define MAX_NUM_BRDS				  4  // edger cut boundaries; 4 boards

//2-5-14 GMB
//Added defines to handle size of arrays used in last board data for history
#define HISTORY_MAX_ANGLES	9
#define HISTORY_MAX_FITS	300
#define HISTORY_MAX_TOP_FITS 50 //this is the number of fits after it's sorted by value that we'll save and decode (unwind) for the fits history


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


#if (_121407) //Hancock - Pitsfield
#define LUG_SPACING                 38  // distance between lugs in inches
#else
#define LUG_SPACING                 36  // distance between lugs in inches
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
#define MAX_IMAGE_X_RES				(IMAGE_X_RES*MAX_LEN_BRD*12)
#define MAX_IMAGE_Y_RES				(IMAGE_Y_RES*LUG_SPACING)

#define MAX_NUM_VISION_DEFECTS		500
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
#define IMAGE_X_RES					25   // pixels per inch (length)			
#define IMAGE_Y_RES					25	 // pixels per inch (width)			
#define MAX_IMAGE_X_RES				(IMAGE_X_RES*20*12)

#define MAX_IMAGE_Y_RES				(IMAGE_Y_RES*LUG_SPACING)

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
//#define NUM_LC_WORDS  2  //temp value; match to trimmer version later
typedef struct
{
    unsigned packetid;                                  /* internal use       */
    unsigned encoder;                                   /* encoder value      */
    unsigned short photocells;                          /* photocell bitmap   */
    unsigned short scanline[NB_MAXHEADS*NB_MAXSPOTS];   /* range readings     */
    //	unsigned short lightcurtain[2];
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


#if (STD_JOESCAN)
// -------------------------------------------------------------------------- 
// ---------------------   JoeScan Laser Heads Data ------------------------- 
// -------------------------------------------------------------------------- 

#define NUM_LZ_PER_UNIT            243     // JoeScan - linear edger
#define HEADS                        8     // Max number of heads in system.
#define MAX_DAS					   4	 // Maximum number of Data Acquisition Stations (Rings)
#define MAX_HEADS_PER_RING		   4	 // Maximum number of Heads in one Ring (Define space)
// slm: 09-12-11: Temporary fixup to use Filtering as it was for Darlington
#if (0)
#define MAX_HEADS_PER_RING_USED	   MAX_HEADS_PER_RING
#else
#define MAX_HEADS_PER_RING_USED	   2
#endif
//  #define DAS                          1     // Data Acquisition Stations (Rings), now read from param file
#define SPOTS          NUM_LZ_PER_UNIT     // Spots per head.
#define BAD               ((int)0x8000)    // Invalid measurement.
#define BAD2              ((int)0xFFFE)    // Invalid measurement.
#define NUMDIAGS                    24     // Number of 16-bit diagnostic data.  0 - 23
#define NUMDATA        NUM_LZ_PER_UNIT     // Number of coordinate data points.  24 -274
#define STANDOFF                 16000     // Blind area of sensor - add to range values.
#define SPOTCONV                    15     // Number of laser points per inch.
#define SCAN_PER_FOOT				  12

#define MAX_NUM_LASERS			(MAX_LEN_BRD*SCAN_PER_FOOT)
#define MAX_NUM_SECTIONS			MAX_NUM_LASERS
#define DAS_MAX_NUM_POINTS		(MAX_HEADS_PER_RING * SPOTS)    // Maximum Points for all heads in one ring/section. (Max of 4 heads per ring)

// slm: 08-11-20 - Added to allow to compile
// Added because code would not compile for JoeScan, now that some code is shared with Canter
#define MAX_NUM_POINTS			DAS_MAX_NUM_POINTS

// fake these for laser_map etc.
#define UNIT_SPACING                1	// in inches
#define INITIAL_LZ_OFFSET           1.0  // distance of first laser from near end
#define INTERNAL_STAGGER            0    // inches time 16 counts/inch; will be zero for original units
#define LASER_STAGGER              22   // as mounted on frame.  2 1/4 inches times 16 counts/inch
#define DYNA_VISION_MODEL			0	// JoeScan
#define MAX_NUM_LASER_PCELLS        8  // max number of photocells in a system
#define JS_MAXHEADS					HEADS //		
#define JS_MAXPROFILES (MAX_LEN_BRD*SCAN_PER_FOOT)				
#define JS_PROFILESTOREAD	25		// read up to this many profiles at one time from joescan dll

// typically expect distance between all scan zones to be the same.
// Heads per Ring can be calculated by iNumActiveHeads / iNumScanZones
// iNumActiveHeads not to exceed HEADS defined in opt_str.h
// HEADS expected to be defined as 16 max on canter; 12 max on edger
// iNumPrflBtwnZones[0] = 0  for only one ring
// iNumPrflBtwnZones[0] = x  for at least two rings
// iNumPrflBtwnZones[x] = x  for more than two rings
typedef struct {
    int iNumActiveHeads;              // number of total joescan heads used in system
    int iNumScanZones;                // number of Scan Zones (rings)  (assume equal number heads per ring)
    int iHeadsPerRing;				// calculated from previous two items; stored here for easy reference
    int iNumPrflBtwnZones[MAX_DAS];	// number of profiles between Scan Zones (4 max)      
    int iMachineType;					// optimizer type, Edger, Sharp, etc.
    double dEncoderResolution;		// Encoder resolution, distance traveled per count
    int iSpare[7];                    // iSpare[0-1] are parsed, but others not implmented at this time
} JOESCAN_SCANNER;

#endif	// #if (STD_JOESCAN)


// following values are used for brd_data.LaserStagger
#define LZ_STAGGER_36                 36   // as mounted on frame.  2 1/4 inches times 16 counts/inch
#define LZ_STAGGER_22                 22   // as mounted on frame.  1 3/8 inches times 16 counts/inch
#define LZ_STAGGER_6                   6   // as mounted on frame.  3/8 inches times 16 counts/inch
#define LZ_STAGGER_8                   8   // as mounted on frame.  1/2 inches times 16 counts/inch (this side to side stagger)
#define LZ_STAGGER_0                   0   // as mounted on frame.  0 inches times 16 counts/inch (this side to side stagger)


// following values are used for brd_data.UnitJump
#define UNIT_JUMP_4                  4   // All M6 and some M24 systems; default
#define UNIT_JUMP_3                  3   // Most M24 Systems over 16ft long
#define UNIT_JUMP_CS3x00  UNIT_SPACING	 // CS3x00 systems; continuous scan

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
#define TOT_DIODES   (MAX_NUM_FT_OF_LIGHT_CURTAIN*NUM_LIGHT_CURTAIN_ELEMENTS_FT)
#define NUM_LC_WORDS (1+TOT_DIODES/16)	// 16 used because data is stored
// by bits in a 16-bit word format. Add 1 to round up

// slm: 06-27-14 - Used By SolutionHistory and PlanView
// How many LC Cells in between lasers
#define LC_UNITS_PER_LASER			(int)(UNIT_SPACING * NUM_LC_ELEMENTS_INCH)

#define MAX_NUM_MACHINE_CENTERS        0  // Normally 0, needs manual input if >

#define MAX_NUM_CATEGORY               3
#define MAX_NUM_SPECIES                0

#if (! V5_NEW_WAY)
#if ( COMMONWEALTH || HARDWOOD )
#define MAX_NUM_PRODUCTS          550 //200 //550 for Commonwealth Ply 12/10/97
#else
#define MAX_NUM_PRODUCTS          200
#endif
#else
#define MAX_NUM_PRODUCTS			75	// Due to parameter restructuring
#endif

#define MAX_GRD_THK_COMBO				40	// Due to parameter restructuring


#define MAX_NUM_THICK                  28
#define MAX_NUM_WIDTH                  28
#define MAX_NUM_GRADE                  28
#define MAX_NUM_LEN                    24   // Number of programable lengths
#define MAX_GRADE_THICK_MC             10  //May need to change if MACH_CNTRS > 0
#define MAX_WIDTH_MC                   10  //May need to change if MACH_CNTRS > 0
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
#define EIP_NUM_OUTPUT_WORDS	128	// changed from 64 to 128 for crosscut output


/* Scanner word:  Bits used for Scanner alarms and more status bits */
#define SCANNER_WORD		0		// WORD index into EIP_OutputArray
#define STICKING_LZ_BIT		0x0001	// Bit0:  Laser or LC detected as sticking ON.
#define LC_ALARM_BIT		0x0002	// Bit1:  Light Curtian has detected card or data failure.
#define NO_LZ_DATA_BIT		0x0004	// Bit2:  No Laser data alarm
#define TIMING_ALARM_BIT	0x0008	// Bit3:  Detected possible error in Encoder timing.
#define TOO_MUCH_SLASH_BIT	0x0010	// Bit4:  Exceeded the max number of allowed consecutive slash solutions.
#define NO_LZ_DATA_ALM_BIT	0x0020	// Bit5:  Exceeded number of lasers allowed with consecutive no data
//#define SC_SPARE2			0x0040  // Bit6:  Future use for other scanner alarms
#define B8_COMM_LOST		0x0080  // Bit7:  B8 ENET Comm Lost (Scanner may be off)
#define QC_ALARM1_BIT		0x0100  // Bit8:  Reserved for QC Alarms
#define QC_ALARM2_BIT		0x0200	// Bit9:  Reserved for QC Alarms
#define LASERS_PWR_ON		0x0400	// Bit10: Indication to PLC when LASER power is on
//#define SC_SPARE5			0x0800	// Bit11: Future use for other status bits or scanner alarms
#define WIDE_BOARD_STOP		0x1000	// Bit12: Flag telling PLC to Momentary Stop/Slow Down on chain
#define CALC_IN_PROG_BIT	0x2000	// Bit13: Optimizer calculations are in progress
#define SCAN_IN_PROG_BIT	0x4000	// Bit14: Linear Optimizer currently scanning data
//#define SC_SPARE9			0x8000	// Bit15: Future use for other status bits or scanner alarms


/* Status word 1: Bits used to define status of system or solution.  */
#define STATUS_WORD1			16	// Array index into EIP_OutputArray
#define CONVEYOR_BIT		0x0001	// Bit0:  Optimizer sees through-feed conveyor running
#define ALARM_LIGHT_BIT		0x0002  // Bit1:  Bit that controls yellow light on scanner.
#define SKEWED_BRD_BIT		0x0004	// Bit2:  Skew in lug detected
#define REMAN_BIT			0x0008	// Bit3:  Board must be Re-Edged
#define RESAW_BIT			0x0010	// Bit4:  Board must be Re-Sawn
#define MARK_BRD_BIT		0x0020	// Bit5:  Board is present on Lug
#define LEAD_PICKER			0x0040	// Bit6:  Need an extra picker for spear point board on lead
#define TRAIL_PICKER		0x0080	// Bit7:  Need an extra picker for spear point board on trail
//#define SW_SPARE3			0x0100	// Bit8:  Future use
#define WANE_DOWN_BIT		0x0200	// Bit9:  Board is wane down
#define FLIP_BRD_BIT		0x0400	// Bit10: Bit ON indicates to use board turner
#define SLASH_BIT			0x0800	// Bit11: Board has no value; slash
#define OVER_WIDTH_BIT		0x1000	// Bit12: Flitch is too wide for edger
#define OVER_THICK_BIT		0x2000	// Bit13: Board is too thick for edger
#define SHORT_BRD_BIT		0x4000	// Bit14: Board is too short for feed table
#define CAL_MODE_BIT		0x8000  // Bit15: System is in calibration mode.


/* Zone Diag Word:  Word used to update zone diag. in Block Xfer and ENET output.*/
#define ZONE_DIAG_WORD				18	// Array index into EIP_OutputArray

/* PLC_WatchDog Word:  Word used to update watchdog value in Block Xfer and ENET output.*/
#define WATCH_DOG_WORD				61	// Array index into EIP_OutputArray


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

/* Defining of tally messages to be displayed on Vorne. display_error_msg()*/
#define NUM_TALLY_MSG    2  // this should equal last valid tally_msg
#define BRDFT_OUT        0  // tally_msg
#define PIECES_OUT       1  // tally_msg  			
#define SLASH_PERCENT    2  // tally_msg displayed in boardfeet
#define BRDFT_IN         3  // tally_msg
#define SKEW_PERCENT     4  // tally_msg 

#define NUM_THK_COUNTS              1300

//*Defines to enable special modes, detail info, simulation, or special printing features*//
#define ENABLE_DETAILSIM	1	//Detail solution / allow simulation setup
#define PRESELECT_TAB		2	//Preselect tab of simulatio dialog
#define MODE_TAB			4	//Mode tab enable
#define FORCE_TAB			8	//Force tab enable
#define PRINT2FILE			16	//Allow print to file option on board list menu
#define SHOW_TIMING_DIALOG	32	//Allow to view Timing Dialog for every board in Screen's BdList (05-12-08)
#define SHOW_HISTORY_DIALOG	64	//Allow to view History Dialog for every board in Screen's BdList (03-05-14)

/* Defining of error codes from lowest to highest priority for display */
#define NUM_ERRORS            16

#define NO_ERROR_CODE				 0
#define EXCESS_DATA_DROPOUTS		 1
#define WTH_OFF_TARGET				 2
#define THK_OFF_TARGET				 3
#define ALARM_1						 4
#define ALARM_2						 5
#define REJECT_ALARM				 6
#define STICKING_LASER				 7
#define NO_LASER_DATA				 8
#define NO_LC_DATA					 9
#define NO_PCELL_DATA				10
#define SKEW_IN_LUG					11
#define NOT_EVEN_ENDED				12
#define NOT_POSITIONABLE			13
#define EMPTY_LUG					14
#define EXCEPTION_IGNORED			15
#define EXCEPTION_NOT_RECOVERABLE	16


/******************************************************************************
ERROR MESSAGES USED IN DISPLAY & ACQUIRE
All use param[0] and [1]
11 Uses 0,1,2,3
13 Uses 0,1 (and 2 with Rev: 4.0.0.10)
*******************************************************************************
    error_code.code				error_code.param[0]			error_code.param[1]
  0 NO ERROR
  1 EXCESSIVE DATA DROPOUTS		laser#						0=top, 1=bottom
  2 WTH OFF TARGET				Amount off target
  3 THK OFF TARGET				Amount off target			laser#
  4 ALARM 1 EXCEEDED			0=thk						target*1000
								1=wth						target*1000
  5 ALARM 2 EXCEEDED			0=thk, 1=wth				target*1000
  6 REJECT ALARM				0=slash, 1=resaw, 2=reman
  7 STICKING_LASER				laser# 
  8 NO LASER DATA				laser#						0=top, 1=bottom, 2=both
  9 NO LC DATA					board#
 10	NO PCELL DATA
 11 SKEW IN LUG					<0 = ?
								0  = similar to 2
								1  = bad data	
								2  = skew
								3  = New Bad Data			[1]-Min LZ, [2]-Max LZ, [3]-Min LC, [4]-Max LC
 12 NOT EVEN ENDED
 13 NOT_POSITIONABLE			'D'= Depth
								'O'= Other
								2  = 2Passes
 14 EMPTY_LUG					Used for CS31xx Web Mode

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


#define NUM_SPARES				72   // number of spares in IO_MAP; used below and in ioconfig.c 

#define NUM_LEN_COUNTS  ((MAX_LEN_BRD+1-MIN_LEN_BRD)*24)




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
    short iRemanAlarm;		// X out of 100 slashes to trigger alarm
    short SkewAlarm;			// Consecutive skews to trigger alarm
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
    int CrossCutSaw[MAX_NUM_SAWS];	// 1=Yes; 0=No
    int iAlarmPctLoss;		// Pct * 10;  123 = 12.3%
    int iAlarmPctResaw;		// Pct * 10;  123 = 12.3%
    int iAlarmPctPass;		// Pct * 10;  123 = 12.3%
    int iAlarmPctSlash;		// Pct * 10;  123 = 12.3%
    int iAlarmPctSkew;		// Pct * 10;  123 = 12.3%
    int iPosVerPresent;		// True if we need to send info to Position Verification System.
    int spares[30];			// spares for future use
    
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


#if (STD_JOESCAN)

typedef struct {
	double local_x;
	double local_y;       
	double theta;       
	double c_theta;
	double s_theta;
    double factor;
} L1HEAD;

typedef struct {
	L1HEAD heads[HEADS];
} L1CFG;


typedef struct {                  
    int x;
    int y;
    int order;                      // Number points 0-480 as they go around the log clockwise.
    int next;                       // Next point.
    int prev;                       // Prev point.
} RAW_POINT;

typedef struct {                  // This structure may not be needed anymore.
    int x[DAS_MAX_NUM_POINTS];
    int y[DAS_MAX_NUM_POINTS];
} RAW_POINT_FRAME;

typedef struct {                  
    int x[DAS_MAX_NUM_POINTS];
    int y[DAS_MAX_NUM_POINTS];
} RAW_POINT_DATA[MAX_NUM_SECTIONS];

typedef struct TAG_RAW_POINT_DATA {
	POINT xy[DAS_MAX_NUM_POINTS];
} XYPOINT_DATA[MAX_NUM_SECTIONS];

// LASER_TEST_DATA: The current reading from the laser measuring array ------
typedef struct {
    int Constant;
    int SupplyVoltPos;
    int InternalVoltPos;
    int InternalVoltNeg;
    int CaseTemp;
    int AmbientTemp;
    int FrameCount;
    int LaserOutput;
    int Reserved[16];
    int range[NUMDATA];    // raw range readings
    int x[SPOTS];          // readings translated to dist coordinate from line bar
    int y[SPOTS];          // readings translated to heighth from top of chain
} LASER_TEST_DATA[HEADS];

// L1 LASER CALIB DATA: The data from laser when calibrating. 
typedef struct {
    int x[HEADS][SPOTS];
    int y[HEADS][SPOTS];
} L1_LASER_CALIB_DATA;

#else

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


typedef struct {
    int height;	// pixels used for width of board (Y Res)
    int width;	// pixels used for length of board (X Res)
    UINT32 CsPixels[MAX_IMAGE_Y_RES][MAX_IMAGE_X_RES];
} CS3300_IMAGE_STRUCT;

typedef struct {
    CS3300_IMAGE_STRUCT	sTopImageData;
    CS3300_IMAGE_STRUCT	sBottomImageData;
} CS3300_IMAGE_DATA; 

#endif // CS3x00

// slm: 07-30-18 - Since using VisionDefectsStruct.h
#if (0)
#define MAX_NUM_VISION_DEFECTS		500

typedef struct {
    int iNumDefects;	
    UpliftDefect sDefects[MAX_NUM_VISION_DEFECTS];
} CS3300_DEFECT_STRUCT;

typedef struct {
    CS3300_DEFECT_STRUCT	sTopDefectData;
    CS3300_DEFECT_STRUCT	sBottomDefectData;
} CS3300_DEFECT_DATA;
#endif

// slm: 06-23-17
#if ( VISION_ENABLED )
typedef struct {
	int height;	// pixels used for width of board (Y Res)
	int width;	// pixels used for length of board (X Res)
	UINT32 Pixels[MAX_IMAGE_Y_RES][MAX_IMAGE_X_RES];
} VISION_IMAGE_STRUCT;

#if STD_JS50
// it possible, JS50 could drop these extra variables.  They are only used for Hermary vision; TAV_Scanner
typedef struct {
	UINT	iStartingEncoderValue;	// only field for TAV_Scanner image data
	VISION_IMAGE_STRUCT	sTopImageData;
	VISION_IMAGE_STRUCT	sBottomImageData;
	int iTopLaserVideoOffset[MAX_NUM_LASERS];
	int iBottomLaserVideoOffset[MAX_NUM_LASERS];
} VISION_IMAGE_DATA;
#else
typedef struct {  // CS33xx version
	VISION_IMAGE_STRUCT	sTopImageData;
	VISION_IMAGE_STRUCT	sBottomImageData;
} VISION_IMAGE_DATA;
#endif // STD_JS50
#endif // VISION_ENABLED

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
    int iSpare;		//was weblug for CS3x00 heads
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


// slm: 08-25-17
/* LC_MARKED_BAD:  Diodes marked as bad in lc_lyout.c are stored in
     lc_marked_bad.manual[]. Diodes marked as bad in lc_test.c are
     stored in  lc_marked_bad.auto_map[]. All diodes from either array
     marked bad are stored in light_curtain_map. Set bit to zero if bad. */
#if (0)
// Original declaration
typedef struct {
    unsigned short auto_map[NUM_LC_WORDS];
    unsigned short manual[NUM_LC_WORDS]; /* 16 used because data is stored
       by bits in a 16-bit word format. Add 1 to round up  */
} LC_MARKED_BAD;      // stored on disk in file named LC_MBAD.dat
#else
// Changed to be like either Trimmer v79x, since this declaration is used on the OptWin side only
// Note: Trimmer sizes for an extra element, to distinguich in between v7.4 + v7.5
typedef struct {
	unsigned short auto_map;
	unsigned short manual; /* 16 used because data is stored
						   by bits in a 16-bit word format. Add 1 to round up  */
} LC_MARKED_BAD[NUM_LC_WORDS];      // stored on disk in file named LC_MBAD.dat

#endif

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
    int lead_vne_x;   // inches times 1000 from the beginning of the lug
    int trail_vne_x;  // for Maximum Wane rule
    int lead_vne1_x;   // inches times 1000 from the beginning of the lug
    int trail_vne1_x;  // for Basic Wane rule
    int lead_thick_x;
    int trail_thick_x;
    int lead_scant_t;
    int trail_scant_t;
} PROFILE_GRADE_THICK;


// change here? Change above also!!
typedef struct {
    int thk_array[MAX_NUM_CNTS_PER_LUG];
    int wane_up_or_dn;    // wane up is TRUE - Edger only 
    int sawn_thick;       // used to find sawn face
    int exceeds_bow;      // profile is bowed is TRUE
    int lead_edge_x;
    int lead_edge_y;
    int trail_edge_x;
    int trail_edge_y;
    
#if (STD_JOESCAN)
    int bot_edge;			// for linear
    int top_edge;			// for linear
    int iLengthFromEvenEnd;	// for linear
#endif
    
    int max_thick;
    PROFILE_GRADE_THICK grade_thick[MAX_GRD_THK_COMBO];
    int cut[MAX_SAW_CUTS];    // place cut will take place for this station - Edger only
} PROFILE_DATA_STN;
typedef PROFILE_DATA_STN PROFILE_DATA[MAX_NUM_LASERS+1];  // used by engine only to keep thk_array


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

/* Photo Cell Calibration */
typedef struct {
    int pcell_curtain_offset; // offset calculated by cal from M24 laser to pcell
    int iPCell_Offset_Top[MAX_NUM_LASER_PCELLS];
    int iPCell_Offset_Bot[MAX_NUM_LASER_PCELLS];
    int positioner_laser[MAX_NUM_LASER_PCELLS]; //laser number at the Pcell.
    int iPCellTopDist[MAX_NUM_LASER_PCELLS];
    int iPCellBotDist[MAX_NUM_LASER_PCELLS];
    int iXdiffTop[ (23*13)+6 ];	// only used on M24 (23 laser spots x 13 max heads) + 6
    int iXdiffBot[ (23*13)+6 ];	// only used on M24 (23 laser spots x 13 max heads) + 6
} PC_CALIB;

// status using for chromascan system
typedef enum CSSTATUS
{
	CS_WAIT,	
	CS_READY,
	CS_ERROR
} CS_STATUS;

typedef struct {
	int iTopCnt;
	int iBotCnt;
} LZ_DROPOUT_CNT; 

typedef struct {
	char cUserName[20];
	char cUserPassword[10];
	int  iPermission;
} USER_PROFILE;

#define NUM_USERS 10

/* GENERAL STUFF (to be input at start-up by Hi-Tech ?)  */
typedef struct  {
    char cust_name[40];       /* customer name */
    char cust_loc[30];        /* customer location */
    char cust_passwordADMN[10];   /* customer defined password */
    unsigned long job_num;    /* Hi_Tech Job number for special stuff */
    int right_or_left;        /* HAND for specific installation,right=TRUE */
    int Lug_Spacing;	// Size of lug spacing in inches; can not exceed max defined
    int UseMachineCenters;	// Specify how to collect QC data
    int calib_bar_thick;      //Thickness of calibration bar, inches times 1000
    int calib_bar_width;      //Width     of calibration bar, inches times 1000
    int language;             // ENGLISH=0, FRENCH=1
    int plc_io_speed;         /* offer choice of plc/io speed 57k or 114k */
    unsigned long validation; /* Reserved for future use */
    USER_PROFILE UserProfile[NUM_USERS];
    MACHINE_CENTER machine_center[MAX_NUM_MACHINE_CENTERS+1];
    // slm: 04-25-17 - Assigned spares as for TRIMMER in Preparation of code Merge
    int				CubicFt;				// TODO: N.U. - display as Cubic Ft instead of BrdFt
    int				B800RangeOffset;		// TODO: N.U. - default 17000; some installs require 10000
    float				fDistPerLzEncoderTick;	// default (1.0/LZ_CNTS_PER_INCH_HARD)
    int				iEncoderCntPerInch;		// only on JS50, Encoder resolution adjustable
    int				spare[20];
} CUST_DATA;  // stored in disk file named CUSTDATA.DAT




#define NUM_SOL_SPEC 4

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
    int reman[NUM_SOL_SPEC];       // needs to be re-edged and possibly resawn
    int resaw[NUM_SOL_SPEC];       // needs to be resawn only
    // edger pass is no resaw or reman indication
    int diverter;          // needs to be diverted
    int lc_good;           // true if lc data is valid
    int alarm;             // enunciates alarm
    int stop_chain;        // solution not positionable, inhibits chain
    int bowed_board;       // set for bowed boards
    
    int ne_posn;           // which near end positioner to raise - Edger
    int fe_posn;           // which far end positioner to raise - Edger
    
    int fe_gate_movement;  // when ioconfig specified use far end gate
    // depth of cut on near end lead along with
    //    far end depth tells where to position
    int ne_cut_depth;
    int fe_cut_depth;
    int ne_width;          // width at near end positioner
    int fe_width;          // width at far end positioner
    int wane_down;         // true if wane is down - Edger
    int flip_board;        // true if board turner to be used - Edger
    int ne_pos_lz;         // lz location of near end positioner - Edger
    int fe_pos_lz;         // lz location of far end positioner - Edger
    int iFirstLz;			 // first lz having valid data
    int iLastLz;			 // last lz having valid data
    // slm: 08-25-17 - changed name to be as in Trimmer v79x (Next 2 items)
    int beg_ele;			 // First LC/vision Diode (was iFirstDiode)
    int end_ele;			 // Last LC/vision diode (was iLastDiode)
    int iNeSawLoc;         // location of saw at initial laser read - Linear Edger
    int iFeSawLoc;         // location of saw 6' from end - Linear Edger
    int iCrossCutSaw;		 // array reference to cross cut trim saw location
    // slm: 05-22-14 Used as Mouse trap to debug OptWin croscut issues in BdList View  
    int iSolutionPhase;	 // 0= No CrossCut (CC) solution, 1=CC (full Length), 2=CC NE, 3=CC FE
    int MeasuredVolumeIn;	 // volume of piece as is coming in. 
    int iVisionDataGood;	 // True if vision data is valid.
    int iSpare[10];		 // spares
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
    
    //Added defines for Kitchens multi-grade select
    int iLeadGrade;						// Lead board grade input
    int iCenterGrade;					// Center board grade input
    int iTrailGrade;					// Trail board grade input
    
    int product;              // product classification
    int rdm_width;            // raw width of random width solution
    int grade_thick;          // grade-thickness classification - Edger
    int grade;                // grade classification
    int thick;                // thickness classification
    int width;                // width classification
    int length;				// length classification
    int board_relative_worth; // price or volume
    int FarBegin;				// last station to start good wood
    int beg_station;          // beginning station of this piece
    int actual_len;           // actual length of solution
    int iStatus;              // Multi-use status word added for Lampe & Malphrus
    int pass_const;           // tells whether piece needs resaw, re-edge, etc.
    int beg_square;           // beginning of square section
    int len_square;           // length of square solution
    CUTTING_SOL Cuttings;
} SOL_SPEC_BRD;
typedef SOL_SPEC_BRD SOLUTION_SPEC[NUM_SOL_SPEC];  // set for the improbable case
// of four boards
/******************** Optimization Data - Fit Information *****************/
/*
  This structure is designed to have all necessary data to
  re-create the image and solution for later debugging.
*/

#define MAX_ERROR_CODES 4
//Defines the error codes used in conjunction with the erro type below for product fit errorcodes array
enum FAILURE_ERROR_CODES{
	NO_ERRORS,
	THK_UNDER_MIN,
	THK_OVER_MAX,
	THK_BASIC_WANE,
	THK_MAX_WANE,
	THK_SCANT_WANE,
	WID_UNDER_MIN,
	WID_OVER_MAX,
	WID_BASIC_WANE,
	WID_MAX_WANE,
	WID_SCANT_WANE,
	LEN_UNDER_MIN,
	LEN_OVER_MAX,
	LEN_BASIC_WANE,
	LEN_MAX_WANE,
	LEN_SCANT_WANE,
	FACE_UNDER_MIN,
	FACE_OVER_MAX,
	FACE_BASIC_WANE,
	FACE_MAX_WANE,
	FACE_SCANT_WANE,
	INTERFERENCE,
	NOT_POS_GENERAL,
	NOT_POS_SOL_WID_0,
	NOT_POS_NE,
	NOT_POS_FE,
	NOT_POS_HIT_LEAD,
	NOT_POS_HIT_TRAIL,
	CHIP_DEPTH_LEAD,
	CHIP_DEPTH_TRAIL,
	FREEDOM,
	MAN_OVERRIDE_GRADE,
	MAN_OVERRIDE_THICK,
	MAN_OVERRIDE_WIDTH,
	GRADE_FAILED_DOWNGRADE,
	WANE_LIMIT_NOT_SET,			// slm: 01-07-15
	DEFECT_ERROR,				// slm: 04-01-15
	LAST_ITEM
};


typedef struct
{ /* !!! index = 0 is not used, so that negative multiboard indices
                                                                            are unique.  Otherwise 0 == -0 */
    int NbrBoards;
    int Index1;              /* index to SortedList if (NbrBoards = 1)
                                or index to BoardFits if (NbrBoards > 1)
                                or -1 if end of fit list */
    
    int Index2;              /* index to BoardFits if (NbrBoards > 1) */
    
    int iGrade;				 /* the actual grade of the product. */
    int NEPin;               /* near end pin number */
    int FEPin;               /* far end pin number */
    int NearBegin;
    int FarBegin;
    int FinalBegin;          /* Beginning of combo from lumber line -
                                comes from WidthFit[].Line[][].FinalBegin */
    int TrlMostLdEdge;       /* Location of lead edge with combo against
                                trailing limit - calculated from
                                (WidthFit[].Line[][].TrlLWaneLimit - thk) */
    int LdMostTrlEdge;       /* same (only different) */
    
    int LdFace;              /* lead edge face limit */
    int TrlFace;             /* trail edge face limit */
    
    int lead_edge;           /* lead edge for board */
    int trail_edge;          /* trail edge for board */
    
    int Width;               /* includes Kerf for multiboards */
    long int Value[MAX_NUM_LEN]; /* cents.  Includes deducts */
    
    // slm: 3.13.2.9 - Renamed + Used for multibits operation
    //  int PassFlag;            /* 0 if not pass, NZ if pass */
    int Flags;				 // Bit 0 Set = Pass
    // Bit 1 Set = Not Positionable: Chip Depth
    // slm: 4.0.0.10
    // Bit 2 Set = Not Positionable: Servo Limit NE
    // Bit 3 Set = Not Positionable: Servo Limit FE
    // Bit 4 Set = Not Positionable: Side Wall Crash Lead
    // Bit 5 Set = Not Positionable: Side Wall Crash Trail
    // Bit 6 Set = Not Positionable: Saw Shift Violation
    // Bit 7 Set = Not Positionable: Limited By Width
    // Bit 8 Set = Not Positionable: Invalid Pin Selection
    
	// slm: 06-12-14 - Expanded usage with Bit 16..23 to contain code path execution to justify addition of a new fit to list
	// The above additional bits utilization is enabled/controlled from inside ThickFit()
    
	int TClass;              /* thickness class to find thickness flags */
    int TCombined;           /* true if this is a combo that uses more
                                than one thickness - optimizer only combines
                                two different thicknesses i.e. stack and jacket
                                boards can be different thicknesses */
    int LClass1;             /* Length class to find min lens */
    int LClass2;             /* Length class to find min lens */
    int LCombined;           /* true if this is a combo */
    int NbrBoardsStack;      /* number of boards in the solution stack */
	int iLeadTrailPos;		//The position this board was fitted for.  0 = Lead, 1 = Trail in ProductFits
    int RandomWidth1;        // 0 if not a Random Width Solution
    int RandomWidth2;        // 0 if not a Random Width Solution
    int RandomWidth3;        // 0 if not a Random Width Solution
    int RandomWidth4;        // 0 if not a Random Width Solution
    
    int iPushOrPull;         // For Hardwood.  0 if neither, 1 if push, 2 if pull
    int iPullWidth;          // Quantify pull width
    int iPushWidth;          // Quantify push width
    int iPushWidthValue;     // How much is it worth?
    
	int iCrashValue;		 // Value is used at this time as Diagnostic, since PLC does not use it yet
	CUTTING_SOL Cuttings;
	bool bTrashCase;
    
	// Used by Solution History	
	FAILURE_ERROR_CODES eErrorCodes[MAX_NUM_LEN]; //error codes added GMB 3/4/14 for edger board debugging features
	TCHAR strFailLocation[25]; //the location in the code where the failure occured
	int iFailureLineNum; //linenumber of failure
    
} BOARDFITS;


//to track the skew information per angle so we can recreate boards in optwin for debugging GMB 3-4-14
typedef struct {
	int		iAngleNumber;		// Initial angle thread was created for (-4,-3,-2,-1,0,1,2,3,4) original 9 angles, 0 split taper, others are degrees of full taper on each side
	int		iAngleDegrees;		// Signed value away from Centerline in degrees in multiples of 7 degrees (-28,-21,-14,-7,0,7,14,21,28)
	int		iCaseID;			// Intended to operate as engine 4..12: Value 0..9.  Used to keep track of which thread is running which engine in MT environment
	int		iAngleIndex;	    // Converts the signed AngleNumber to an unsigned index that is used for storage array locations of each angle's information
	int     iSkewDegrees;		// Calculated in Skew() (Value is skew value * 10000).  Actually the slope that skew uses to align the laser data to solve with
	int		iSpare;
	double  dSlope;				// slm: 07-01-14
} ANGLE_INFO;

/************* input output maps  **************************/
#define IO_IN	0
#define IO_OUT -1

typedef struct {
    int addr;
    int len;
    int in_or_out;
} INPUT_OUTPUT_MAP;

typedef struct {
    
    //transverse I/O
    //inputs
    INPUT_OUTPUT_MAP encoder;              // Electro - Cam sequencer (8 max; input)
    INPUT_OUTPUT_MAP display_select;       // Select Vorne display option (3-i)
    INPUT_OUTPUT_MAP grd_input;            // grd pb input (16-i)
    INPUT_OUTPUT_MAP ne_cutoffs;           // Near end cutoffs (4-i)
    INPUT_OUTPUT_MAP fe_cutoffs;           // Far end cutoffs (4-i)
    INPUT_OUTPUT_MAP force_lead;           // pb to force a fixed width board on lead side (1-i)
    INPUT_OUTPUT_MAP force_center;         // pb to force a fixed width board in center (1-i)
    INPUT_OUTPUT_MAP force_trail;          // pb to force a fixed width board on trail side (1-i)
    INPUT_OUTPUT_MAP force_pb1;            // pb to force a fixed width board (1-i)
    INPUT_OUTPUT_MAP force_pb2;            // pb to force a fixed width board (1-i)
    INPUT_OUTPUT_MAP force_pb3;            // pb to force a fixed width board (1-i)
    INPUT_OUTPUT_MAP force_pb4;            // pb to force a fixed width board (1-i)
    INPUT_OUTPUT_MAP force_pb5;            // pb to force a fixed width board (1-i)
    INPUT_OUTPUT_MAP force_pb6;            // pb to force a fixed width board (1-i)
    INPUT_OUTPUT_MAP force_pb7;            // pb to force a fixed width board (1-i)
    INPUT_OUTPUT_MAP force_pb8;            // pb to force a fixed width board (1-i)
    INPUT_OUTPUT_MAP force_pb9;            // pb to force a fixed width board (1-i)
    INPUT_OUTPUT_MAP bark_allow_lead;      // pb to force a Lead bark allowance (1-i)
    INPUT_OUTPUT_MAP bark_allow_trail;     // pb to force a Trail bark allowance (1-i)
    INPUT_OUTPUT_MAP scan_in_progress;     // board currently being scanned (1-i)
	// Select man inputs as override/preselect (1-i)
    INPUT_OUTPUT_MAP machine_center_select; // (4-i)
    INPUT_OUTPUT_MAP manual_preselect_toggle;
    INPUT_OUTPUT_MAP cat_pb;               // input category pb (5-i)
    
    //outputs
    INPUT_OUTPUT_MAP scanner_alarm;        //scanner alarm status (1-o)
    INPUT_OUTPUT_MAP calc_in_progress;     // optimizer calculations are in progress (1-o)
    INPUT_OUTPUT_MAP positioner_mask;      // which positioner to raise (5-o)
    INPUT_OUTPUT_MAP positioner_usable_mask; // which pos'ners are enabled (5-o)
    INPUT_OUTPUT_MAP zone_diagnostic;      // res_opt zone feedback (3-o)
    INPUT_OUTPUT_MAP edger_word0;          // multiplexed raw thick and product thick for top head (16-o)	
    INPUT_OUTPUT_MAP edger_word1;          // first word multiplexed data (16-o)
    INPUT_OUTPUT_MAP edger_word2;          // second word multiplexed data (16-o)
    INPUT_OUTPUT_MAP edger_word3;          // third word multiplexed data (16-o)
    INPUT_OUTPUT_MAP edger_word4;          // fourth word multiplexed data (16-o)
    INPUT_OUTPUT_MAP conveyor_enable;      // scanner through feed chains running (1-o)
    INPUT_OUTPUT_MAP alarm;                // board out of spec (1-o)
    INPUT_OUTPUT_MAP skewed_board;         // no solution; brd skewed (1-o)
    INPUT_OUTPUT_MAP reman;                // ne or fe reman (2-o)
    INPUT_OUTPUT_MAP resaw;                // ne or fe resaw (2-o)
    INPUT_OUTPUT_MAP mark_board;           // mark a board for test (board in lug) (1-o)
    INPUT_OUTPUT_MAP ne_valid;             // edger_words = near end data (1-o)
    INPUT_OUTPUT_MAP fe_valid;             // edger_words = far end data (1-o)
    INPUT_OUTPUT_MAP saw_valid;            // edger_words = saw width data (1-o)
    INPUT_OUTPUT_MAP wane_down;            // board is wane down (1-o)
    INPUT_OUTPUT_MAP board_turner;         // board turner (1-o)
    INPUT_OUTPUT_MAP slash;                // bit indicates no brd value(1-o)
    INPUT_OUTPUT_MAP overwidth_board;      // signal board is overwidth (1-o)
    INPUT_OUTPUT_MAP overthick_board;      // signal board is overthick (1-o)
    INPUT_OUTPUT_MAP underlength_board;    // signal board is underlength (1-o)
    
    // Begin add for Lampe & Malphrus
    INPUT_OUTPUT_MAP flitch_pb;            // pb to indicate flitch (1-i)
    INPUT_OUTPUT_MAP pass_pb;              // pb to indicate flitch "pass" (1-i)
    INPUT_OUTPUT_MAP thk_selector;         // selector switch for 5/4-7/4 (1-i)
    INPUT_OUTPUT_MAP num_brds_pb;          // pb for # brds in a cant (4-i)
    INPUT_OUTPUT_MAP timber_pb;            // pb to select timber size (2-i)
    
    INPUT_OUTPUT_MAP PLC_WatchDog;         // (8-i) Input counter from PLC is repeated back in Block Xfer.
    INPUT_OUTPUT_MAP ForceSquareEdge;      // (2-i) Input to force sqr edge along board
    INPUT_OUTPUT_MAP B8_COM_Lost;          // (1-o) Output to tell PLC if B8 ENET Com Lost
    INPUT_OUTPUT_MAP CalModeBit;           // (1-o) Output to tell PLC System is in Calibrate Mode
    INPUT_OUTPUT_MAP test_board;           // (1-i) 
    INPUT_OUTPUT_MAP Lz_On_Flag;           // (1-i) Input: Flag to Opt to disable or enable lasers; 1=LZ ON; 0=LZ OFF
    INPUT_OUTPUT_MAP Lz_Pwr_ON;			 // (1-o) Output: Flag to PLC; Scanner is ON and READY
    INPUT_OUTPUT_MAP ForcedWidth;			 // (5-i) Input forced width in decimal form
    
    
    /* Linear I/O for JoeScan Linear Edger Project; moved and added at bottom of list */	
    //// Linear I/O
    ////inputs
    INPUT_OUTPUT_MAP data_acknowledge;	 // (1-i) (Linear Only) acknowledgement that PLC got data (1-i)
    INPUT_OUTPUT_MAP skew_inhibit;		 // inhibit skewing of saws (1-i)
    
    ////outputs
    INPUT_OUTPUT_MAP target_thick;		 //thickness for top head (1-o)
    INPUT_OUTPUT_MAP raw_length;			 //board length in inches (1-o)
    INPUT_OUTPUT_MAP nearend_cut;			 //cutline distance from linebar at 0"
    INPUT_OUTPUT_MAP farend_cut;			 //cutline distance from linebar at 72"
    INPUT_OUTPUT_MAP lead_width;			 //target width for single or linebar side
    INPUT_OUTPUT_MAP trail_width;			 //target width for second board
    
    ////outputs -- status bits
    INPUT_OUTPUT_MAP data_valid;       // solution is complete; data sent to PLC (1-o)
    INPUT_OUTPUT_MAP strip_test;			//(1-i)Force edger to cut 1/2" strip
    INPUT_OUTPUT_MAP ForceLz_Reset;		//(1-i)Force lasers to reset from PLC input
    INPUT_OUTPUT_MAP PLC_WatchDog_Out;     // (4-o) Output counter to PLC is repeated back.
    
    INPUT_OUTPUT_MAP CrossCutEnable;     // (1-i) Input Allow PLC to disable Crosscut solutions.
    
    INPUT_OUTPUT_MAP WidthForceLead;     // (8-i) Binary Input defines width PB.
    INPUT_OUTPUT_MAP WidthForceCenter;   // (8-i) Binary Input defines width PB.
    INPUT_OUTPUT_MAP WidthForceTrail;    // (8-i) Binary Input defines width PB.
    
    INPUT_OUTPUT_MAP GradeInputLead;     // (8-i) Binary Input defines grade PB.
    INPUT_OUTPUT_MAP GradeInputCenter;   // (8-i) Binary Input defines grade PB.
    INPUT_OUTPUT_MAP GradeInputTrail;    // (8-i) Binary Input defines grade PB.
    
    INPUT_OUTPUT_MAP UsePinEnableBit8;    // (1-i) handle how enable bits are addressed.
    
    // Spares
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
    int ne2;				 // cut 2' off near end
    int ne4;				 // cut 4' off near end
    int ne6;				 // cut 6' off near end
    int ne8;				 // cut 8' off near end
    int fe2;				 // cut 2' off far end
    int fe4;				 // cut 4' off far end
    int fe6;				 // cut 6' off far end
    int fe8;				 // cut 8' off far end
    int ne_cutoff;		 // position in lc elements of selected NE cutoffs
    int fe_cutoff;		 // position in lc elements of selected FE cutoffs
    int gate;				 // perform programmed gate action
    int fe_gate;			 // perform programmed gate action on fe
    // Begin add for Lampe & Malphrus
    int flitch;            // evaluate this board as a flitch
    int pass;              // "pass" this flitch/board
    int thk_selector;      // 0 = 5/4; 1 = 7/4; thickness of boards
    int three_board_cant;  // make three boards
    int six_board_cant;    // make six boards
    int nine_board_cant;   // make nine boards
    int twelve_board_cant; // make twelve boards
    int timber1_cant;      // make a small timber
    int timber2_cant;      // make a large timber
    int cant_selected;     // TRUE if any of prev six TRUE
    // End add for Lampe & Malphrus
	// Begin add for Hardwood edger
    int force_lead;        // pb to force a fixed width board on lead side 
    int force_center;      // pb to force a fixed width board in center
    int force_trail;       // pb to force a fixed width board on trail side 
    int force_width;       // pb to force a fixed width board 
    
    // Added defines for Kitchens multi-grade select
    int iLeadGrade;				 // forced lead grade
    int iCenterGrade;			 // forced center grade
    int iTrailGrade;			 // forced trail grade
    
    int bark_allow_lead;   // pb to force a lead bark allowance 
    int bark_allow_trail;  // pb to force a trail bark allowance 
    
    // slm: Added with 3.13.2.7 - Required for Battle Lbr Co.
    int iForceSquareEdge;  // SQ_NONE, SQ_LEAD, SQ_TRAIL, SQ_BOTH
    int test_board;   // board was marked for board test
    
    int NE_Crush;		// record value of expected crush on Near-End
    int FE_Crush;		// record value of expected crush on Far-End
    int spares[6];
} MANUAL_PRESELECTS;

// slm: 07-16-09 - Expanded to break down Process Laser + Assigned all remaing spares
// times should be stored in uSeconds
typedef struct
{
    union
    {
        struct
        {
            unsigned int engine1;				// 0
            unsigned int engine1a;				// 1
            unsigned int engine2;				// 2
            unsigned int engine3;				// 3
            unsigned int engine4;				// 4 SplitTaper
            unsigned int engine5;				// 5 1st Skew
            unsigned int engine6;				// 6
            unsigned int engine7;				// 7
            unsigned int engine8;				// 8
            unsigned int engine9;				// 9
            unsigned int engine10;				// 10
            unsigned int engine11;				// 11
            unsigned int engine12;				// 12 Last Skew
            unsigned int engine13;				// 13
            unsigned int engineTotalSkew;		// 14
            unsigned int engineTotalSolution;	// 15
#if (STD_3X00)
            unsigned int CS3x00_SocketTLz;		// 16 Top Lasers
            unsigned int CS3x00_SocketTVz;		// 17 Top Vision
            unsigned int CS3x00_SocketBLz;		// 18 Bottom Lasers
            unsigned int CS3x00_SocketBVz;		// 19 Bottom Vision
            unsigned int CS3x00_SocketTotal;	// 20 total Socket Read Time
            unsigned int CS3x00_FindDefects;	// 21 time to find defects (uplift)
            unsigned int CS3x00_ParseTime;		// 22 total CS3x00 time; time from first socket read until uplift processing is complete
            unsigned int na23;
            unsigned int na24;
            unsigned int na25;
#elif (STD_JOESCAN)
            unsigned int JS_RcvProfiles;		// 16 Time required to receive profiles from start of board to end	
            unsigned int JS_MoveToXLat;			// 17 Move profiles to XLat
            unsigned int JS_StopSyncMode;		// 18 Stop sync mode on JoeScan heads 
            unsigned int JS_ProcessLasers;		// 19 Total Process laser data: [23+24+25]
            unsigned int JS_TotalScanTime;		// 20 Time from start of receiving profiles to end of process laser data
            unsigned int na21;
            unsigned int na22;
            unsigned int JS_ProcessLasers0;		// 23: Process laser data: Phase[0]
            unsigned int JS_ProcessLasers1;		// 24: Process laser data: Phase[1]
            unsigned int JS_ProcessLasers2;		// 25: Process laser data: Phase[2]
#else
            unsigned int na16;		
            unsigned int na17;
            unsigned int na18;
            unsigned int na19;
            unsigned int na20;
            unsigned int na21;
            unsigned int na22;
            unsigned int na23;
            unsigned int na24;
            unsigned int na25;
#endif
            unsigned int na26;
            unsigned int na27;
            unsigned int Skew;					// 28 Skew time for the angle we have chosen
#if (STD_3X00)
            unsigned int DefectEval;			// 29 Skew time for the angle we have chosen
#else
            unsigned int na29;
#endif
            unsigned int na30;
            unsigned int na31;					// 31: Reserved to report Engine # that solved
        } ByName;
        unsigned int ByPhase[32];
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
    
    time_t clear_time;    /* time/date last cleared, DOS format */
    time_t time_running;  /* same as res_data.time_running */
    time_t time_enabled;  /* same as res_data.time_enabled */
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
    unsigned long int iCrossCutNum;
    unsigned long int iCrossCutBdftGained;
    unsigned long int iCrossCutValueGained;
    unsigned long int iCrossCutValidLength;
    unsigned long int iCrossCutWidthMismatch;
    int spares[11];
} TALLY_DATA_STRUCT;
typedef TALLY_DATA_STRUCT TALLY_DATA[MAX_NUM_CATEGORY];   // Production tally information

#define MAX_PV_PROFILES			 312		// 26ft @ 1" density (max data expected)

// PosVerData Struct
typedef struct {
	int Location;				// inches*100	- Distance of Laser from NE.
	int CenterLine;				// inches*1000	- center line location of solution for reference
	int LocLeadFaceBreakPt;		// inches*1000	- X encoder location of face break point
	int DistLeadFaceFromSaw;	// inches*1000	- distance between face and edge of board.
	int LocTrailFaceBreakPt;	// inches*1000	- X encoder location of face break point
	int DistTrailFaceFromSaw;	// inches*1000	- distance between face and edge of board.
	int StripFaceLead;			// inches*1000	- amount of face expected on strip
	int StripFaceTrail;			// inches*1000	- amount of face expected on strip.
	int StripWidthLead;			// inches*1000	- amount of width expected on strip
	int StripWidthTrail;		// inches*1000	- amount of width expected on strip.
} POS_VER_PROFILE_DATA;

typedef struct {
    int BrdID;					// ID number of Data set
    int TargetThk[MAX_NUM_BRDS];	// inches*1000 for each board cut
    int TargetWth[MAX_NUM_BRDS];	// inches*1000 for each board cut
    int iNumPrfl;					// number of valid profiles in this data
    int iPinUsedNE;				// if pin used on NE; location given in inches*100
    int iPinUsedFE;				// if pin used on FE; location given in inches*100
    POS_VER_PROFILE_DATA PrflData[MAX_PV_PROFILES];
} POS_VER_STRUCT; 
typedef POS_VER_STRUCT POS_VER_DATA[10];  // for now will hold rolling buffer of last 10 through optimizer



#define MAX_ANGLES 100   /* maximum size for angle arrays */
#define MAX_FITS   2400  /* max number of solutions for one optimization pass */
#define MAX_BRDS    30   /* Why this many ? (Legacy from Canter ???) */

// SLM: 4.0.2.6
// The last 2 boards do not contain any edge board details
#define BEST_PASS_SOLUTION_SLOT		0
#define BEST_RESAW_SOLUTION_SLOT	1

// slm: Used since V4.0.0.6 of Solutione Engine
//==========================================================================================
#define PRIMARY_ANGLES		9	// Used by current Edger Optimization Engine
#define SECONDARY_ANGLES	4	// Used by future Edger Optimization Engine to Refine Solution
// Results for Secondary angles are stored in array after primary
#define TOTAL_ANGLES		(PRIMARY_ANGLES + SECONDARY_ANGLES)

// TODO: change later
// after Secondary angle solution logic has been implemented
//#define LAST_USED_ANGLE		(TOTAL_ANGLES-1)
#define LAST_USED_ANGLE		(PRIMARY_ANGLES-1)
#define MID_ANGLE_CONVERT	(LAST_USED_ANGLE / 2)
//==========================================================================================

typedef struct {
    int LdEdgeLimit;    /* 0 indicates null entry for [2] fits */
    int TrlEdgeLimit;   /* limit based on vne */
    int NearBegin;
    int FarBegin;
    int FinalBegin;     /* chosen because of lwane */
    int LdLWaneLimit;   /* limit based on lwane evaluation */
    int TrlLWaneLimit;
    
	// Used by Solution History
	FAILURE_ERROR_CODES eErrorCodes; //error codes added GMB 3/4/14 for edger board debugging features
} PRODUCTFIT;

/* for every product , there is a set of these - 
   they correspond to the scanner "slices" */
typedef struct {
    PRODUCTFIT ProductFit[MAX_NUM_PRODUCTS][MAX_NUM_LEN][2];
    
    int NumBF;  /* number of board fits in board_fits */
    int IndexLst[MAX_BRDS];
    
    BOARDFITS BoardFits[MAX_FITS];
}FITS;

//used to describe all necessary details about each board in a solution.  Used in the Top Fits List as an array
typedef struct
{ 
    int Index;              // Category.brd_prd index that describes the board used in this position of a FIT
    int iSingleFitIndex;    // Single Fit Index ID - Used to justify the way the solution was unwinded
    int LClass;             /* Length class to find min lens */
    int LdMostTrlEdge;      
    int TrlMostLdEdge;      /* Location of lead edge with combo against
                                trailing limit - calculated from
                                (WidthFit[].Line[][].TrlLWaneLimit - thk) */
    int LdFace;             /* lead edge face limit */
    int TrlFace;            /* trail edge face limit */
    int lead_edge;          /* lead edge for board */
    int trail_edge;         /* trail edge for board */
    int Width;              /* includes Kerf for multiboards */
    long int Value;			/* cents.  Includes deducts */
    int RandomWidth;        // 0 if not a Random Width Solution
    int iPushOrPull;        // For Hardwood.  0 if neither, 1 if push, 2 if pull
    int iPullWidth;         // Quantify pull width
    int iPushWidth;         // Quantify push width
    int iPushWidthValue;    // How much is it worth?
	int iLeadTrailPos;		// The position this board was fitted for.  0 = Lead, 1 = Trail in ProductFits
	int iSpare[3];			// In case we forgot something else, sing RW is untested at this time
	PRODUCTFIT ProductFit;  //describes the lead / trail limits, near, far and final begin
	POINT cpBoardCorners[4];//x,y coordinates for all 4 corners.  X is in Lasers, Y is Inches * 1000
} SINGLE_BOARDFIT; //holds pertinent info on a single fit entry, later used in an array to describe a complete board fit for history

//expanded fits list struct used to create a unwound version of the top 100 best fits for History GMB
typedef struct
{
	SINGLE_BOARDFIT Board[MAX_NUM_BRDS]; //array of single boards that make up this solution
    int			NbrBoardsStack;     /* number of boards in the solution stack */
	int			Flags;				//meaning the same as in BoardFits
	long int	lTotalValue;
	int			iTotalSolutionWidth;
	int			iCrashValue;		 // Value is used at this time as Diagnostic, since PLC does not use it yet
	bool		bTrashCase;
	int			iFitIndex;
	int			iFreedom;			//difference between the limits - solution width including kerfs
	int			iLeadBoardLeadEdge;	 //absolute limit of the lead board pushed all the way to the lead edge
	int			iTrailBoardTrailEdge;//absolute limit of the trail board pushed all the way to the trail
}TOP_FITS_LIST;

typedef struct{
	int iFitIndex;
	int iOrigIndex1;
	int iOrigIndex2;
	int iOrigLClass;
	int iOrigFitValue;
	int iWidth;		// slm: 08-08-14
}FITS_BY_VALUE;

//===============================================================================================
// slm: 4.0.2.6 - expose a portion of the solution record for new use
//===============================================================================================
typedef struct {
    int ThickCode;							// thickness code
    int WidthCode;							// Width code
    int LenCode;							// length code
    int Product;							// product code
    int Nbr;								// number of these boards (center Stack)
    int Begin;								// start of board in LC Units
	CUTTING_SOL Cuttings;
} ONE_DESCRIPTION;

typedef struct
{
	int iCategory;							// Used by Optwin only
	int nCandidatesPass;					// How many where of this type 
	int nCandidatesResaw;					// How many where of this type 
	int Array[MAX_NUM_PRODUCTS];			// 0= Possible Edge, 1= Resaw, 2= Pass
	ONE_DESCRIPTION Pass;					// More "Convenient" of the Pass Type	
	ONE_DESCRIPTION Resaw;					// More "Convenient"of the Resaw Type
} PRELIMINARY_SOLUTION;
//===============================================================================================


typedef struct { 
    int Status;              /* bits
                              0 Not positionable,
                              1 early stop, time limited
                              2 added board(s) to reach chip limit,
                              3 manual override
                              //---------------------------------------------------
                              4 bad laser data
                              5 cant too wide
                              6 canter size limited solution
                              7 infeed table limited solution
                              //---------------------------------------------------
                              8 Lampe & Malphrus, pass flitch
                              9 off center due to chip or throat limit
                              10 cant stack requires reedging  , hardwood use--push
                              11 shifted to get legal linebar  , hardwood use--pull
                              //---------------------------------------------------
                              12 bad light curtain data
                              13 cant too high
                              14 invalid pins, only one selected.
                              15 both legal linebar and chipdepth not possible
  
                               ------------------------------------------------------
                               Defined in V4.0.0.6 of Edger Engine
                               ------------------------------------------------------
                                  Bit 16..18 How solution was obtained
                                    000= Live/Loaded - Default state
                               16 LSB 0 \ 
                               17     1  > 1..7 = See Simulation Mode in Control Panel
                               18 MSB 2 /         See Simulate.h (Operating Mode)
  
                                  Bit 19..20 - Simulation Extension mode
                               19 1= LS= Limited Solution Range via Simulation control Panel
                               20 1= FS= Forced Solution via Simulation Control Panel
  
                                  Bit 21..22 - Set only while Simulating in Sweep mode
                                    00 = Not Sweeping - Normal State
                                    01 = Sweep/Start
                                    11 = I= Intermediate Direction while in Sweep mode
                                    10 = Sweep/End
                               21 1= SS= This solution uses the Most Extreme Direction (Sweep/Start)
                               22 1= SE= This solution uses the Most Extreme Direction (Sweep/End)
                               
                                  Bit 23 and 24 if on, are on one or the other
                               23 1= OS= Override - Simulated via control Panel 
                               24 1= OO= Override - Original Overrides with one in Simulation Control Panel 
  
                               25
                               26 
                               27 
                              //---------------------------------------------------
                               28 1= EO= Engine limited Solution because of Override control
                               29 1= EL= Engine limited Solution because of Other Factors (Appearance, customer Id, etc)
                               30 1= MO= Manual Override controls found in original data to solve for
                               31 Reserved - do not use
                             */
    long int Value;          /* solution Value:  lumber Value less deducts */
    int TrlMostLdEdge;       /* location of lead edge with solution in
                                the trailmost position */
    int LdMostTrlEdge;       /* same for other side */
    int LineBar;             /* LineBar limit */
    int OffsetPositionable;  /* Offset that makes board positionable */
    int Width;               /* Width of solution, total stack height */
    int Angle;               /* Angle scanned-image rotated for solution */
    int CantHeight;          /* cant thickness */
    int Gang;                /* destination Gang */
    ONE_DESCRIPTION Stack[MAX_BRDS];       /* Up to 9 sideboards with center Stack */
    int LdFace;              /* lead edge face limit */
    int TrlFace;             /* trail edge face limit */
    
    char NEPin;              /* near end pin number */
    char FEPin;              /* far end pin number */
    int NEPos;               /* servo position of near end pin */
    int FEPos;               /* servo position of far end pin */
    int LdSawCut[2];         /* location of lead edge cut */
    int TrlSawCut[2];        /* location of trail edge cut */
    double line_m;
    int line_b;
    
} SOLUTION;

enum eREMAN_BIT_DEF : char {
	NO_REMAN = 0x0000,
	RE_EDGE = 0x0001,
	PASS_WIDTH = 0x0002,
	RE_SAW = 0x0004,
	PASS_THK = 0x0008,
};

typedef struct {
    float LdAngle;            /* taper for lead vne in Width counts/profile */
    float TrlAngle;           /* taper for trail vne */
    int Angles[MAX_ANGLES];   /* Sequence of tapers optimized */
    int CantWidth;            /* maximum Width of cant */
    int CantLength;           /* maximum length of cant */
    //    char BoardWidthClass[MAX_NUM_PRODUCTS];
    //                              /* bit 0 1/0 = 1 == this Product needs re edge  */
    //                              /* bit 1 2/0 = 2 == has min wth for min lth     */
    //    char BoardThickClass[MAX_NUM_PRODUCTS]; /* bit 0 1/0 = 1 == this Product needs resaw */
    //                                            /* bit 1 2/0 = 2 == has min thk for min lth  */
    //    int pass_beg[MAX_NUM_PRODUCTS];  /* beg w/ no resaw/reman */
    //    int pass_len[MAX_NUM_PRODUCTS];  /* length w/ no resaw/reman */
	eREMAN_BIT_DEF RemanStatus[MAX_NUM_PRODUCTS][MAX_NUM_LEN];
    /* bit 0 1/0 = 1 == this Product needs re edge  */
    /* bit 1 2/0 = 2 == has min wth for min lth     */
    /* bit 2 3/0 = 4 == this Product needs resaw    */
    /* bit 3 4/0 = 8 == has min thk for min lth     */
	char spare[MAX_NUM_PRODUCTS];
    int pass_beg[MAX_NUM_PRODUCTS][MAX_NUM_LEN];  /* beg w/ no resaw/reman */
    int pass_len[MAX_NUM_PRODUCTS][MAX_NUM_LEN];  /* length w/ no resaw/reman */
} PREEVAL;

/*********** Cant: Scan data and Solution *******************/
typedef struct {
    
    PROFILE_DATA Profile;    /* laser data */
    
    int CantStatus;          /* bits: (lsb)0 operator inputs,
                                 1 light curtain data, 2 laser #1 data,
                                 3 laser #2 data, 4 scan data filtered,
                                 5 optimized, 6 made, 7 tallied */
    
    int FirstProfile;        /* first profile on board */
    int LastProfile;         /* last profile on board */
    int BeginProfile;        /* profile after ne_cutoff excluded */
    int EndProfile;          /* profile after fe_cutoff excluded */
    
    struct{
        char grade;              /* grade number: > 0  highest grade
                                                 < 0  only legal grade */
        char GangThk[4];        /* thickness code (1-16) for gangs, 0=n/a */
    } Oper;
    
    PREEVAL PreEval;
    SOLUTION Solution;		// slm: 3.13.2.10 - Created typedef for various uses
    
} CANT;


#define NUM_THKS_MIN_WANE 17  // 16 divisions; 17 points
typedef struct {
    int thks[NUM_THKS_MIN_WANE];
    float divisor;
} MINWANE;

// Bit 0 of Global Reason must be ON, else ignore all structure data display
typedef struct
{
	int iGlobalReason;					// Why the forced solution failed on this angle
	int iReasonByBoard[NUM_SOL_SPEC];	// Used to identify failure at board level (if possible)
} SOL_PF_MAP;

// Container for a ProductFit from the original list
typedef struct
{
	int			OriginalIndex;	// Index to backtrack source of this data from original list
	PRODUCTFIT	Limit[2];		// Product limit: Lead + Trail
} PRODUCTFIT_DIAG;

// Container for a BoardFit from the original list
typedef struct
{
	int			OriginalIndex;	// Index to backtrack source of this data from original list
	BOARDFITS	Limit;
} BOARDFIT_DIAG;

// Solution justification + details about solution for the angles attempted
// At this time WhyNot structure is not filled in
// Probably it will be completed in Version 5 of Engine
#define BOARDFITS_DIAG_SIZE		7
typedef struct {			// For this angle...
	SOL_PF_MAP		WhyNot;			// Data valid only when in FORCE mode (Certain Boards and no other)
	SOLUTION		Solution;		// Solution Description for this direction
    // Room for products saw generated + 1 Strip save
	PRODUCTFIT_DIAG ProductFit[NUM_SOL_SPEC+1];
	BOARDFIT_DIAG	BoardFits[BOARDFITS_DIAG_SIZE];	// Subset of the whole Fit List used by BestSolution
	int				nFitsUsed;		// Utilized of MAX_FITS
	int				iWidth[NUM_SOL_SPEC+1];			// Solution Target Width (Dimension / RW)
	int				iBeginLC[2][NUM_SOL_SPEC+1];	// Solution Product Begin [0]=Near, [1]=Far
	int				BoardFitIndex[3];	// Where Double, triple, etc... being in Fit List
	int				iCrashValue;		// Value is used at this time as Diagnostic to justify reason for shifting solution
	int				bAddedFits;			// slm: 07-09-14 Without, not all the work done by Engine is exposed in Solution History
	int				iFreedom;			// slm: 12-11-14 - Carried over from Engine
	float			fSkew;				// slm: 12-18-14 Value calculated in Engine, added to help debugging
	int				iMostLeadEdge;		// slm: 01-05-15 For the Rotated Skewed Data (Needed to simplify drawing in Solution History, not used in View Drawing)
	int				iMostLeadProfile;	// slm: 01-05-15 For the Rotated Skewed Data (Profile with most negative Leading Edge if present, else -1)
	int				AmountAddedBySkew;	// slm: 02-08-15 - Good to Know + Used in Solution History to Plot Failed Boards
	int				AmountAddedBySkewLC;// slm: 02-08-15 - Good to Know
	int				nRWBoards;			// slm: 10-14-15 - Use to use value reported in iFreedom or not (Ignore Freedom if > 0, since it is Zero)
	int				nRWCuttings;		// slm: 10-14-15 - Use to use value reported in iFreedom or not (Ignore Freedom if > 0, since it is Zero)
	int				Spare[6];			// Room to grow
} SOLUTION_NEW;


// calculated on every board being solved
typedef struct {
	int	iBestDirection;			// Signed value: MIN_SWEEP_ANGLE to MAX_SWEEP_ANGLE
	int	iBestDirectionIndex;	// Translated Directions into Solution[]: 0 to MAX_ANGLES-1
	int	iBoardId[NUM_SOL_SPEC];	// Normally other than -1 when valid (FORCE)
    // This section was extracted from PREEVAL
    float LdAngle;				//   taper for lead vne in Width counts/profile
    float TrlAngle;				//   taper for trail vne
    int Angles[HISTORY_MAX_ANGLES];		//   Sequence of tapers optimized
    int CantWidth;				//   maximum Width of cant
    int CantLength;				//   maximum length of cant
    // End
    // Used to compare/validate solution at the Office
	SOLUTION_NEW SolutionSummary[HISTORY_MAX_ANGLES];	// of this LAST_USED_ANGLE+1 are shown
	int	iCaseID;				// slm: 06-20-08: This is the MT engine that produced the Best Solution
	float fSlopeNoSolution;		// slm: 07-09-14
	int	Spare[4];				// Room to grow
} DETAILS_BY_DIRECTION;



// LAST BOARD DATA
typedef struct {
    unsigned int io_num;
    PLAN_DATA plan_data;			// 10-30-17 (raw plan_data)
    PLAN_DATA smooth_plan_data;	// 10-30-17 (New Addition)
    PROFILE_DATA_STN profile_data[MAX_NUM_LASERS+1];
    
#if ( DYNA_VISION_MODEL == 8 )
    RAW_LASER_PCELL_DATA photocell_data;
#endif
    
#if (STD_JOESCAN)
    //  RAW_LASER_DATA     raw;
    RAW_POINT_DATA     raw_pt_data;
    RAW_POINT_DATA     xlat_pt_data;
#endif
#if (STD_JOESCAN)
    joescan::jsProfile jsRawProfile[JS_MAXHEADS][JS_MAXPROFILES+JS_PROFILESTOREAD]; // allow overrun baseD on number of profiles read at a time
#endif
    
    PASS_WOOD_LEN pass_wood_len;
    NO_WANE_LEN no_wane_len;
    GOOD_WOOD_LEN good_wood_len;
    //  BOOLEAN_PF_MAP pf_map; 
    IMPLEMENT_DATA implement_data;
    SOLUTION_SPEC solution_spec;
    ERROR_CODE error_code;
    MANUAL_PRESELECTS manual_preselects;
    TIMING Timing;
    
#if( STD_B8 )
    PCELLHISTORY PCellHistory;
    SCANNEDOBJECTCOPY LaserBuffer;
#endif
    
    // Added defines for Kitchens multi-grade select
	int iMinInputGrade;		// min grade entered
	int iMaxInputGrade;		// max grade entered
    //  int input_grade;
    
#if (STD_JOESCAN)
    int input_skew_zero;
#endif
    int input_machine;
    
    // Used by Solution History
    BOARDFITS History_BoardFits[HISTORY_MAX_ANGLES][HISTORY_MAX_FITS];//save the history of the board fits for debugging in optwin 2-5-14 GMB
    BOARDFITS History_NoFits[HISTORY_MAX_ANGLES][HISTORY_MAX_FITS];   //save the history of the NON-board fits for debugging in optwin 3-4-14 GMB
    DETAILS_BY_DIRECTION DetailSolution;								//best fit for each angle
    ANGLE_INFO AngleInfo[HISTORY_MAX_ANGLES];							//save the details on the amount of skew etc. for each angle
    TOP_FITS_LIST TopFitsList[HISTORY_MAX_ANGLES][HISTORY_MAX_TOP_FITS];//The top N fits decomposed into single indexes that point to the singles in the board_fits list.  No unwinding necessary
    
    //  CANT cant_solution;
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
	QC_THK_ALARMS thk[MAX_GRADE_THICK_MC];
	QC_WTH_ALARMS wth[MAX_WIDTH_MC];
	QC_LEN_ALARMS len[MAX_NUM_LEN];
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
	QC_WIDTH width[MAX_NUM_MACHINE_CENTERS+1][MAX_WIDTH_MC];
	QC_THICK thick[MAX_NUM_MACHINE_CENTERS+1][MAX_GRADE_THICK_MC];
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

typedef struct {   // used by QC shift thickness trend program
    
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
    
	TREND_THICK thick[MAX_NUM_MACHINE_CENTERS+1][NUM_INTERVALS][MAX_GRADE_THICK_MC];
	int next_tnum[MAX_NUM_MACHINE_CENTERS+1][MAX_GRADE_THICK_MC];   // next interval to fill
	TREND_WIDTH width[MAX_NUM_MACHINE_CENTERS+1][NUM_INTERVALS][MAX_WIDTH_MC];
	int next_wnum[MAX_NUM_MACHINE_CENTERS+1][MAX_WIDTH_MC];         // next interval to fill
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
    int MaxKnotDiameter;			// maximum single knot size allowed
    int SumKnotDiameterAllowed;	// maximum sum of knots in a given sub length
    int SectionLengthForSum;		// length of section defined for SumKnotDiameterAllowed
    int KnotDistanceFromSawline;	// keep knots x distance for edge of board (reduce chance of edge knots)
    int spare[10];
    CUTTINGS sCuttings;  // subproduct descriptions for cutting units
} G_CLASS;




typedef struct L1 {  // length data stored in sorted order (all categories are the same)
    int target;   // all length dimensions in inches*10
    int upper;
    int lower;
} SORTED_LEN[MAX_NUM_LEN];

// replace Cat and Brd_Products structures for new way
/* Board Product contains full discription for manufactured products */
/*** Items marked with '@' filled at dn_load ***/
#if (V5_NEW_WAY)
typedef struct {
    int lengthID;
    int index;           // used for display sorting
    unsigned int scant_len_allowed; //@ LC elements
    unsigned int end_len_wane_limit;  //LC elements; limits wane dist from end
    int price;                      // price/piece
    unsigned int bdft;              // total bdft for product
    int product_enable;             // 1=make product, 0 = disable
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
	//COleDateTime date;
	VARIANT_BOOL IsMetric;
	CAT category[MAX_NUM_CATEGORY];
	TALLY_DATA_STRUCT tally[MAX_NUM_CATEGORY];
} TALLY_DATA_STRUCT_DELTA;

#endif

// uncomment after decel-specifiers defined.
typedef struct {
	unsigned int crefWidthColor[ MAX_NUM_WIDTH ];
}WIDTHCOLORREF;

typedef  struct {				// or -1 = any or not used - depending on the context
	int ProductId;				// 0 to MAX_NUM_PRODUCTS-1 
	int LengthId;				// 0 to MAX_NUM_LEN-1
}  PRODUCT_BY_LENGTH;

typedef  struct {				// or -1 = any or not used - depending on the context
	int ProductId;				// 0 to MAX_NUM_PRODUCTS-1 
	int LengthId;				// 0 to MAX_NUM_LEN-1
	int iFitsIndex;		//points to the original fit index in FitsPtr->BoardFits[]
}  FITS_UNWIND;

typedef struct {
	int iUseSimParams;
	MANUAL_PRESELECTS manual_preselects;
	int iMode;
	int iAngleNo;
	int iGradeMin;
	int iGradeMax;
	int iTest[6];
	int iBoardFitToShowFrom;	// 4.0.1.4: Valid Value if > 0
	int iBoardFitLastShown;		// 4.0.1.4: Used by Engine only, reset by OptWin
	PRODUCT_BY_LENGTH iProductID[4];	// V5 - changed
	PRODUCT_BY_LENGTH iProductUse[4];
	int spare[20];						// V5 - new
}SIM_PARAMS;




#endif
