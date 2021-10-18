/* date = October 18th 2021 3:24 pm */

#ifndef _T_A_V__INTERFACE_H
#define _T_A_V__INTERFACE_H
// Defines and structures in this file are used by both optimizer service
// and the TAV Scanner service.  These fields are required knowledge between
// the two apps for them to interface correctly with each other.


#define	TAV_SCANNER_XFER_SIZE		125000000	// 125MB for formatted image data

#define TAV_MAX_LUG_INCHES			22	// max lug size 32"

#define TAV_MAX_NUM_CAMERAS			12	// Max for top OR bottom not the total in system. 


// initial target format is 64 x 64 pixel density; may change later
#if STD_JS50	// JoeScan JS50 Vision
#define TAV_CAM_OUTPUT_PIXEL_PER_INCH_HEIGHT	20  //32	//64	// pixels along board width
#define TAV_CAM_OUTPUT_PIXEL_PER_INCH_WIDTH		20	// pixels along board length
#define TAV_CAM_OUTPUT_SCAN_LENGTH				24	// 24" length scanned per camera
#else	// Hermary Vision
#define TAV_CAM_OUTPUT_PIXEL_PER_INCH_HEIGHT	32	//64	// pixels along board width
#define TAV_CAM_OUTPUT_PIXEL_PER_INCH_WIDTH		32	// pixels along board length
#define TAV_CAM_OUTPUT_SCAN_LENGTH				24	// 24" length scanned per camera
#endif

// Camera size of re-sampled output data; smaller than raw input data
#define TAV_CAM_OUTPUT_PIXEL_HEIGHT		(TAV_CAM_OUTPUT_PIXEL_PER_INCH_HEIGHT*TAV_MAX_LUG_INCHES)			// Width of board	(Frame Height)
#define TAV_CAM_OUTPUT_PIXEL_WIDTH		(TAV_CAM_OUTPUT_PIXEL_PER_INCH_WIDTH*TAV_CAM_OUTPUT_SCAN_LENGTH)	// length of board	(Frame Width)
#define TAV_CAM_OUTPUT_CHANNELS			4
#define TAV_CAM_OUTPUT_PIXEL_BUFF		(TAV_CAM_OUTPUT_PIXEL_WIDTH * TAV_CAM_OUTPUT_CHANNELS)

// System pixel size definitions
#define TAV_SYSTEM_OUTPUT_PIXEL_HEIGHT	TAV_CAM_OUTPUT_PIXEL_HEIGHT	// lug space (board width)
#define TAV_SYSTEM_OUTPUT_PIXEL_WIDTH	(TAV_CAM_OUTPUT_PIXEL_WIDTH*TAV_MAX_NUM_CAMERAS)	// board length


// max size for 1/4" laser density on 26ft system.
#define TAV_MAX_NUM_LASERS			1248		// 13 heads * (96 lasers per head) 
#define TAV_LZ_MAX_CNTS_PER_LUG		2048		//max encoder scan lines (32" lug * 64cnts per inch)

// Calibration defines
#define MAX_CAL_DOTS_PER_CAMERA				4	// probably only two or three, but leaving room if needed.



/*  RAW LASER DATA  The measurement data from the last whole lug space. */
// note: the first three words are wasted space; only the 0 element of RAW_LASER_DATA
//       is used to hold these numbers.  The rest are zero.
typedef struct {
	int count;		// number of encoder counts to the end of data
	short range[TAV_LZ_MAX_CNTS_PER_LUG];   // as inches times 1000
} TAV_LASER_STRUCT;	/* from near end as pairs, use for both top and bottom.  */

typedef  TAV_LASER_STRUCT TAV_LASER_DATA[TAV_MAX_NUM_LASERS];

typedef struct
{
	unsigned int iFrameNum;
	unsigned int iStartingEncoderValue;
	TAV_LASER_DATA Top;
	TAV_LASER_DATA Bot;
} TAV_LASER_BUFFER;


typedef struct
{
	unsigned int num_spots; // actually is number of pairs of spots
	unsigned int Top_spot_status[TAV_MAX_NUM_LASERS]; // BROKE or OK
	unsigned int Bot_spot_status[TAV_MAX_NUM_LASERS]; // BROKE or OK
} TAV_LASER_MAP;


// struct definitions for image output data
typedef struct {
	int iHeightBegin;	// pixels used for width of board (Y Res)
	int iHeightEnd;
	int iWidthBegin;	// pixels used for length of board (X Res)
	int iWidthEnd;
    
	// Y = Height = lug space;  X = Width = Scanner Length;   Z = Color Channels
	UINT8 Pixels[TAV_SYSTEM_OUTPUT_PIXEL_HEIGHT][TAV_SYSTEM_OUTPUT_PIXEL_WIDTH][TAV_CAM_OUTPUT_CHANNELS];  
} TAV_IMAGE_STRUCT;


// formatted structure we plan to send back to Optimizer
// fyi as of 7/11/2017 struct sized at 302MB  - sjf
typedef struct
{
	unsigned int io_num;					// io_num from laser data repeated back
	unsigned int iStartingEncoderCnt;		// starting encoder for buffer
	TAV_IMAGE_STRUCT	sTopImageData;
	TAV_IMAGE_STRUCT	sBottomImageData;
	int iTopLaserVideoOffset[TAV_MAX_NUM_LASERS];
	int iBottomLaserVideoOffset[TAV_MAX_NUM_LASERS];
    
	VISION_DEFECT_DATA	vision_defect_data;	// top and bottom image defect data
    
	// may include meta-data from cameras
    
	// processing time in ms
	int iQuantizeTiming;
	int iStitchTiming;
	int iDefectDetectTiming;
} TAV_SCANNER_BUFFER_OUT;

// formatted structure we plan to send back to Optimizer
// fyi as of 7/11/2017 struct sized at 302MB  - sjf
typedef struct
{
	unsigned int io_num;					// io_num from laser data repeated back
	int iDefectDetectTiming;
	VISION_DEFECT_DATA	vision_defect_data;	// top and bottom image defect data
} TAV_SCANNER_DEFECT_DATA;

typedef struct
{
	unsigned int io_num;					// io_num from laser data repeated back
	TAV_IMAGE_STRUCT	sTopImageData;
	TAV_IMAGE_STRUCT	sBottomImageData;
} TAV_SCANNER_BUFFER_TO_TAV;


typedef struct
{
	// pixel centroid location of the black dots found on cal bar.
	POINT CalBarDots[MAX_CAL_DOTS_PER_CAMERA];
	int iNumDots;
}CAL_BAR_DOTS;

// structure to contain all the calibration constants needed
// for vision system to map pixel locations correctly.
typedef struct
{
	CAL_BAR_DOTS TopCalBarDotsPerHead[TAV_MAX_NUM_CAMERAS];	// top Calibration dot locations
	CAL_BAR_DOTS BotCalBarDotsPerHead[TAV_MAX_NUM_CAMERAS];	// bottom Calibration dot locations.
    
} TAV_CALIBRATION;


#endif //_T_A_V__INTERFACE_H
