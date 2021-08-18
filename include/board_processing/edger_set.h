// EdgerSet.h

// These data elements are brd_data elements used by the Edger engine
// plus new data elements needed to implememt the Edger engine.
// G - General Setup
// I - Input Pushbutton Setup
// F - Frame Setup
// T - Table Setup
// C - Cost Setup
// E - Edger Saw Setup
// O - Algorithm Specific Setup

#ifndef _EDGER_SET_H
#define _EDGER_SET_H

#define MAX_NUM_POSITIONERS    8  // 16'- 3, 20'- 4, 24'- 5, 28'-?

#define NUM_EDGER_SET_SPARES	 86  // used in edg_data
/****************** Machine Specifications **********************/
typedef struct {
    int NumAngles;    //O      /* number of angles to attempt */
    int AngleStep;    //O      /* angle step size */
    
    unsigned OptionBits; //O   /* option bits for optimizer:
    /*  bit 0: 1 = use full opto for dest. select.
            1-15 not used.   */
    struct {
        int MouthWidth; //E-115  /* Maximum width canter will accept              */
        int ChipDepth;  //E-116  /* Maximum chip depth (0 = Unlimited)            */
        long ChipArea;  //E-117  /* Maximum chip area - Not Used                  */
        int EdgeFixed;  //C-118  /* deduct for boards needing edged (before %)    */
        int EdgePcnt;   //C-119  /* percent deduct for boards to be edged         */
        int ResawFixed; //C-120  /* deduct for resaw decision (before %)          */
        int ResawPcnt;  //C-121  /* percent deduct for resaw decision             */
        int CutInTwoFixed; //C   /* RGM deduct for cut-in-two decision            */
        int CutInTwoPcnt;  //C   /* RGM percent deduct for cut-in-two decision    */
        int Accuracy;   //T-122  /* total system position Accuracy                */
        int MisPosPcnt; //C-123  /* percent deduct for mispositioning (0-50) [20] */
    } Canter;
    
    struct {
        int NumSaws;    //E-124
        int MouthHeight;//E
        int ThkCode;    //E       /* index to thick dimensions */
        int Kerf;       //E-125   /* Kerf in width units  */
        int LdReSaw;   //Unused   /* resaw allowed on lead edge of cant */
        int TrlReSaw;  //Unused   /* resaw allowed on trail edge of cant */
        int LineBar;   //Unused   /* L-lead,T-trail,N-none                     */
        int PcntLength;//Unused   /* 0-50 % of length from center that must
        /* be against LineBar */
        int Offset;     //Unused  /* additional wood left on line bar side or
        /* Offset from canter for direct coupled gang */
    } Gang[4];
    
} MACHINE;

typedef struct {
	int iMinStroke;
	int iMaxStroke;
} SAWS;

typedef struct {
    
    // Inputs needed by Edger available in BRD_DATA(opt_str.h)
    int default_grade_push_button;  //I
    int default_category;           //I
    int TrimSaveAvailable;	/* ? - consider cut-in-two/trim save solutions */
    int min_len_handled;      /* G - minimum length handled (MIN_LEN_BRD to MIN_LEN_BRD+6') */
    int max_len_handled;      /* G - maximum length handled (?' to 24')   */
    int num_lc_eq_notch;      // O - number of lc elements < min_w to be a notch
    int expand_chain_runners; // G - True increases len when brd ends at runner
    //     False assumes brd ends at runner
    int lc_present;           // F - True if light curtain installed.
    int use_6008;             // G - 0=PAMUX, 1=PLC5, 2=SLC500, 3=OMRON
    int first_prd;            // O - brd_prd array element to smallest product
    int first_thk;            // O - t_class array element to smallest thick
    int first_wth;            // O - w_class array element to smallest width
    int first_len;            // O - l_class array element to shortest length
    
    // Inputs for:
    // Edger int
    int num_positioners_edger_table; // T-157  number of devices to hold board on table
    int positioner_location[MAX_NUM_POSITIONERS];  // T - in lc elements from even end
    int positioner_moveable[MAX_NUM_POSITIONERS];  // T - True if moveable
    //     False if not moveable
    int distance_to_reference[MAX_NUM_POSITIONERS];// T - positioner reading (in*1000)
    int min_thk_positioner;   // T-127  min thick at positioner in thousandths of an inch
    
    int distance_lead_saw_to_ref;     //E-151 positioner reading (in*1000) to ref
    int distance_trail_saw_to_ref;    //E-152 positioner reading (in*1000) to ref
    int lower_limit_of_lead_saw_stroke;  //E-153 min position of lead saw
    int upper_limit_of_lead_saw_stroke;  //E-154 max position of lead saw
    int lower_limit_of_trail_saw_stroke; //E-155 min position of trail saw
    int upper_limit_of_trail_saw_stroke; //E-156 max position of trail saw
    
    int lower_limit_of_stroke;           //T-165 min position of holding device
    int upper_limit_of_stroke;           //T-164 max position of holding device
    
    
    int dist_ref_to_lead_side_plate;     //E-167 (in*1000)
    int dist_ref_to_trail_side_plate;    //E-168 (in*1000)
    int depth_of_top_head_cut;           //E-170 (in*1000)
    int max_thick_to_edger;   //E - Max Thick to go thru edger in thousandths
    int max_length_to_edger;  //T - Max Length that will go thru edger - lc elements
    
    SAWS iSaws[5];
    
    // minimum distance between saws
    int iMinDistBetSaws;
    
    // Lampe and Malphrus
    int flitchMBF;                       //C
    int random_width_grade_push_button;  //I
    int cant_brd_thk1;                   //I
    int cant_brd_thk2;                   //I
    int cant_kerf;                       //E-136
    int timber1_wth;                     //I
    int timber2_wth;                     //I
    
    int cost_of_splitter;                //C-160 cost of using splitter saw
    
    int positioning_reference;           //T-162 0-Center, 1-Lead, 2-Trail
    int positioner_location_per_board;   //T-163 0-Under, 1-Lead, 2-Trail
    // stroke Incr(0) or Decr(1) as pos'ner moves trail to lead
    int positioner_zero;                 //T-166
    
    int iDistPastLastPositioner; // amount of wood required past last positioner to use
    
    int iEncoderPulsesPerScan;  // L1 system, number of encoder counts per laser scan
    
    // Edger bool
    int mark_all_trims;     // G - RGM - Mark non-defect length trims [Y/N]?
    int iStripSave;			    // Strip saver option 0=Lead, 1=Trail, 2=None
    
    short int sPctWthAddPicker; // % Missing width to add a board picker to the solution
    short int sPickerLocCheck;  // Number of 1/2" increments, from the near end, to check
    
    // Begin add for hardwood edger.
    int other_grades;       // allow use of other than selected grade.
    int bark_allow_trail;		// trail -formerly large bark allowance
    int bark_allow_lead;		// lead - formerly small bark allowance
    
    // Begin add for linear edger
    int iSaveAsDefault;		  // default orientation is saved for the laser plan view
    int iNearEndOrient;		  // orientation for the near end of picture
    int iLeadEdgeOrient;		// orientation for the lead edge of picture
    
    // Begin add for miscellaneous enhancements
    int board_turner_avail;        // 1 = board turner is available, 0 = not installed
    int StripSaveBoardFixed;       // deduct for being strip save board
    int StripSaveBoardPrcntLength; // length percent required for strip save board
    int bDisplaySawLines;
    int bRemoveSawnRegion;
    int bManualTrimSaw;
    int bSmoothLaserProfiles;
    int bUseFixedSaws;			// are "piggy-back" saws used
    int iFixedSawLead;			// "piggy-back" saw spacing on Lead side
    int iFixedSawTrail;			// "piggy-back" saw spacing on Trail side
    short int wNEPinsAllowed;		// Number of NE pins allowed for positioning board (default is 2).
    short int sFlipBoardPercentWidth;  // Pct Face width require for wane; 0=disabled 100=requres full face
    short int wFlipBrdPctLengthWane;   // Pct Length wane required for flipping; 0=flip all boards with wane down; 100=only flip board with wane on full length.
    int iMaxSkewAngle;				// max skew angle of linear edger
    int iEdgeCrushThickness;			// thickness at which edge will crush to on feed table.
    int iMaxWidthToFlip;				// max width of board to allow board turner to flip	
    int iMinWidthBeforeSplit;			// minimum raw width required before we consider a split solution.
    int iCrossCutEval;				// Evaluate for CrossCut first then Rip (requires CrossCut Saws to be installed).
    int iCrossCutPercentDeduct;		// percentage of $ value to deduct for cross cut
    int iCrossCutPercentVolumeIncrease; // percentage increase in volume required for cross cut.
    int iAlignSkewToSawnEdges;		// force skew angle to follow square sawn edge.
    int iMaxCrossCutWidth;			// disable cross cut if width too wide to cut.
    int iCrossCutCost;				// penality $ value to deduct from CrossCut solutions.
    int spares[NUM_EDGER_SET_SPARES];
    MACHINE MachineSpec;
    
} EDG_DATA;   // stored in disk file named EdgerSet.DAT

/*******************************************************************************
    New stuff for canter added by JDW on  Thu  10-21-1993
*******************************************************************************/

//#define MAX_ANGLES 100   /* maximum size for angle arrays */
//#define MAX_FITS   2400  /* max number of solutions for one optimization pass */
//#define MAX_BRDS    30   /* Why this many ? (Legacy from Canter ???) */
//
//// SLM: 4.0.2.6
//// The last 2 boards do not contain any edge board details
//#define BEST_PASS_SOLUTION_SLOT		0
//#define BEST_RESAW_SOLUTION_SLOT	1
//
//// slm: Used since V4.0.0.6 of Solutione Engine
////==========================================================================================
//#define PRIMARY_ANGLES		9	// Used by current Edger Optimization Engine
//#define SECONDARY_ANGLES	4	// Used by future Edger Optimization Engine to Refine Solution
//								// Results for Secondary angles are stored in array after primary
//#define TOTAL_ANGLES		(PRIMARY_ANGLES + SECONDARY_ANGLES)
//
//								// TODO: change later
//								// after Secondary angle solution logic has been implemented
////#define LAST_USED_ANGLE		(TOTAL_ANGLES-1)
//#define LAST_USED_ANGLE		(PRIMARY_ANGLES-1)
//#define MID_ANGLE_CONVERT	(LAST_USED_ANGLE / 2)
////==========================================================================================


/******************** Optimization Data - Fit Information *****************/
/*
  This structure is designed to have all necessary data to
  re-create the image and solution for later debugging.
*/

//typedef struct
//  { /* !!! index = 0 is not used, so that negative multiboard indices
//              are unique.  Otherwise 0 == -0 */
//    int NbrBoards;
//    int Index1;              /* index to SortedList if (NbrBoards = 1)
//                                or index to BoardFits if (NbrBoards > 1)
//                                or -1 if end of fit list */
//
//    int Index2;              /* index to BoardFits if (NbrBoards > 1) */
//
//    int iGrade;				 /* the actual grade of the product. */
//    int NEPin;               /* near end pin number */
//    int FEPin;               /* far end pin number */
//    int NearBegin;
//    int FarBegin;
//    int FinalBegin;          /* Beginning of combo from lumber line -
//                                comes from WidthFit[].Line[][].FinalBegin */
//    int TrlMostLdEdge;       /* Location of lead edge with combo against
//                                trailing limit - calculated from
//                                (WidthFit[].Line[][].TrlLWaneLimit - thk) */
//    int LdMostTrlEdge;       /* same (only different) */
//
//    int LdFace;              /* lead edge face limit */
//    int TrlFace;             /* trail edge face limit */
//
//    int lead_edge;           /* lead edge for board */
//    int trail_edge;          /* trail edge for board */
//
//    int Width;               /* includes Kerf for multiboards */
//    long int Value[MAX_NUM_LEN]; /* cents.  Includes deducts */
//
//// slm: 3.13.2.9 - Renamed + Used for multibits operation
////  int PassFlag;            /* 0 if not pass, NZ if pass */
//    int Flags;				 // Bit 0 Set = Pass
//							 // Bit 1 Set = Not Positionable: Chip Depth
//							 // slm: 4.0.0.10
//							 // Bit 2 Set = Not Positionable: Servo Limit NE
//							 // Bit 3 Set = Not Positionable: Servo Limit FE
//							 // Bit 4 Set = Not Positionable: Side Wall Crash Lead
//							 // Bit 5 Set = Not Positionable: Side Wall Crash Trail
//							 // Bit 6 Set = Not Positionable: Saw Shift Violation
//							 // Bit 7 Set = Not Positionable: Limited By Width
//							 // Bit 8 Set = Not Positionable: Invalid Pin Selection
//    
//	int TClass;              /* thickness class to find thickness flags */
//    int TCombined;           /* true if this is a combo that uses more
//                                than one thickness - optimizer only combines
//                                two different thicknesses i.e. stack and jacket
//                                boards can be different thicknesses */
//    int LClass1;             /* Length class to find min lens */
//    int LClass2;             /* Length class to find min lens */
//    int LCombined;           /* true if this is a combo */
//    int NbrBoardsStack;      /* number of boards in the solution stack */
//
//    int RandomWidth1;        // 0 if not a Random Width Solution
//    int RandomWidth2;        // 0 if not a Random Width Solution
//    int RandomWidth3;        // 0 if not a Random Width Solution
//    int RandomWidth4;        // 0 if not a Random Width Solution
//
//    int iPushOrPull;         // For Hardwood.  0 if neither, 1 if push, 2 if pull
//    int iPullWidth;          // Quantify pull width
//    int iPushWidth;          // Quantify push width
//    int iPushWidthValue;     // How much is it worth?
//
//	int iCrashValue;		 // Value is used at this time as Diagnostic, since PLC does not use it yet
//	CUTTING_SOL Cuttings;
//	bool bTrashCase;
//} BOARDFITS;

//typedef struct {
//    int LdEdgeLimit;    /* 0 indicates null entry for [2] fits */
//    int TrlEdgeLimit;   /* limit based on vne */
//    int NearBegin;
//    int FarBegin;
//    int FinalBegin;     /* chosen because of lwane */
//    int LdLWaneLimit;   /* limit based on lwane evaluation */
//    int TrlLWaneLimit;
//} PRODUCTFIT;
//
///* for every product , there is a set of these - 
//   they correspond to the scanner "slices" */
//typedef struct {
//  PRODUCTFIT ProductFit[MAX_NUM_PRODUCTS][MAX_NUM_LEN][2];
//
//  int NumBF;  /* number of board fits in board_fits */
//  int IndexLst[MAX_BRDS];
//
//  BOARDFITS BoardFits[MAX_FITS];
//}FITS;
//
////===============================================================================================
//// slm: 4.0.2.6 - expose a portion of the solution record for new use
////===============================================================================================
//typedef struct {
//    int ThickCode;							// thickness code
//    int WidthCode;							// Width code
//    int LenCode;							// length code
//    int Product;							// product code
//    int Nbr;								// number of these boards (center Stack)
//    int Begin;								// start of board in LC Units
//	CUTTING_SOL Cuttings;
//} ONE_DESCRIPTION;
//
//typedef struct
//{
//	int iCategory;							// Used by Optwin only
//	int nCandidatesPass;					// How many where of this type 
//	int nCandidatesResaw;					// How many where of this type 
//	int Array[MAX_NUM_PRODUCTS];			// 0= Possible Edge, 1= Resaw, 2= Pass
//	ONE_DESCRIPTION Pass;					// More "Convenient" of the Pass Type	
//	ONE_DESCRIPTION Resaw;					// More "Convenient"of the Resaw Type
//} PRELIMINARY_SOLUTION;
////===============================================================================================
//
//
//typedef struct { 
//  int Status;              /* bits
//							0 Not positionable,
//							1 early stop, time limited
//							2 added board(s) to reach chip limit,
//							3 manual override
//							//---------------------------------------------------
//							4 bad laser data
//							5 cant too wide
//							6 canter size limited solution
//							7 infeed table limited solution
//							//---------------------------------------------------
//							8 Lampe & Malphrus, pass flitch
//							9 off center due to chip or throat limit
//							10 cant stack requires reedging  , hardwood use--push
//							11 shifted to get legal linebar  , hardwood use--pull
//							//---------------------------------------------------
//							12 bad light curtain data
//							13 cant too high
//							14 invalid pins, only one selected.
//							15 both legal linebar and chipdepth not possible
//
//							 ------------------------------------------------------
//							 Defined in V4.0.0.6 of Edger Engine
//							 ------------------------------------------------------
//								Bit 16..18 How solution was obtained
//								  000= Live/Loaded - Default state
//							 16 LSB 0 \ 
//							 17     1  > 1..7 = See Simulation Mode in Control Panel
//							 18 MSB 2 /         See Simulate.h (Operating Mode)
//
//							    Bit 19..20 - Simulation Extension mode
//							 19 1= LS= Limited Solution Range via Simulation control Panel
//							 20 1= FS= Forced Solution via Simulation Control Panel
//
//							    Bit 21..22 - Set only while Simulating in Sweep mode
//								  00 = Not Sweeping - Normal State
//								  01 = Sweep/Start
//								  11 = I= Intermediate Direction while in Sweep mode
//								  10 = Sweep/End
//							 21 1= SS= This solution uses the Most Extreme Direction (Sweep/Start)
//							 22 1= SE= This solution uses the Most Extreme Direction (Sweep/End)
//							 
//							    Bit 23 and 24 if on, are on one or the other
//							 23 1= OS= Override - Simulated via control Panel 
//							 24 1= OO= Override - Original Overrides with one in Simulation Control Panel 
//
//							 25
//							 26 
//							 27 
//							//---------------------------------------------------
//							 28 1= EO= Engine limited Solution because of Override control
//							 29 1= EL= Engine limited Solution because of Other Factors (Appearance, customer Id, etc)
//							 30 1= MO= Manual Override controls found in original data to solve for
//							 31 Reserved - do not use
//                           */
//  long int Value;          /* solution Value:  lumber Value less deducts */
//  int TrlMostLdEdge;       /* location of lead edge with solution in
//                              the trailmost position */
//  int LdMostTrlEdge;       /* same for other side */
//  int LineBar;             /* LineBar limit */
//  int OffsetPositionable;  /* Offset that makes board positionable */
//  int Width;               /* Width of solution, total stack height */
//  int Angle;               /* Angle scanned-image rotated for solution */
//  int CantHeight;          /* cant thickness */
//  int Gang;                /* destination Gang */
//  ONE_DESCRIPTION Stack[MAX_BRDS];       /* Up to 9 sideboards with center Stack */
//  int LdFace;              /* lead edge face limit */
//  int TrlFace;             /* trail edge face limit */
//
//  char NEPin;              /* near end pin number */
//  char FEPin;              /* far end pin number */
//  int NEPos;               /* servo position of near end pin */
//  int FEPos;               /* servo position of far end pin */
//  int LdSawCut[2];         /* location of lead edge cut */
//  int TrlSawCut[2];        /* location of trail edge cut */
//  double line_m;
//  int line_b;
//
//} SOLUTION;
//
//typedef struct {
//    float LdAngle;            /* taper for lead vne in Width counts/profile */
//    float TrlAngle;           /* taper for trail vne */
//    int Angles[MAX_ANGLES];   /* Sequence of tapers optimized */
//    int CantWidth;            /* maximum Width of cant */
//    int CantLength;           /* maximum length of cant */
////    char BoardWidthClass[MAX_NUM_PRODUCTS];
////                              /* bit 0 1/0 = 1 == this Product needs re edge  */
////                              /* bit 1 2/0 = 2 == has min wth for min lth     */
////    char BoardThickClass[MAX_NUM_PRODUCTS]; /* bit 0 1/0 = 1 == this Product needs resaw */
////                                            /* bit 1 2/0 = 2 == has min thk for min lth  */
////    int pass_beg[MAX_NUM_PRODUCTS];  /* beg w/ no resaw/reman */
////    int pass_len[MAX_NUM_PRODUCTS];  /* length w/ no resaw/reman */
//    char BoardWidthClass[MAX_NUM_PRODUCTS][MAX_NUM_LEN];
//                              /* bit 0 1/0 = 1 == this Product needs re edge  */
//                              /* bit 1 2/0 = 2 == has min wth for min lth     */
//    char BoardThickClass[MAX_NUM_PRODUCTS];
//							  /* bit 0 1/0 = 1 == this Product needs resaw */
//                              /* bit 1 2/0 = 2 == has min thk for min lth  */
//    int pass_beg[MAX_NUM_PRODUCTS][MAX_NUM_LEN];  /* beg w/ no resaw/reman */
//    int pass_len[MAX_NUM_PRODUCTS][MAX_NUM_LEN];  /* length w/ no resaw/reman */
//} PREEVAL;
//
///*********** Cant: Scan data and Solution *******************/
//typedef struct {
//
//  PROFILE_DATA Profile;    /* laser data */
//
//  int CantStatus;          /* bits: (lsb)0 operator inputs,
//                               1 light curtain data, 2 laser #1 data,
//                               3 laser #2 data, 4 scan data filtered,
//                               5 optimized, 6 made, 7 tallied */
//
//  int FirstProfile;        /* first profile on board */
//  int LastProfile;         /* last profile on board */
//  int BeginProfile;        /* profile after ne_cutoff excluded */
//  int EndProfile;          /* profile after fe_cutoff excluded */
//
//  struct{
//    char grade;              /* grade number: > 0  highest grade
//                                             < 0  only legal grade */
//    char GangThk[4];        /* thickness code (1-16) for gangs, 0=n/a */
//  } Oper;
//
//  PREEVAL PreEval;
//  SOLUTION Solution;		// slm: 3.13.2.10 - Created typedef for various uses
//  
//} CANT;
//
//
//#define NUM_THKS_MIN_WANE 17  // 16 divisions; 17 points
//typedef struct {
//  int thks[NUM_THKS_MIN_WANE];
//  float divisor;
//} MINWANE;
//
//// Bit 0 of Global Reason must be ON, else ignore all structure data display
//typedef struct
//{
//	int iGlobalReason;					// Why the forced solution failed on this angle
//	int iReasonByBoard[NUM_SOL_SPEC];	// Used to identify failure at board level (if possible)
//} SOL_PF_MAP;
//
//// Container for a ProductFit from the original list
//typedef struct
//{
//	int			OriginalIndex;	// Index to backtrack source of this data from original list
//	PRODUCTFIT	Limit[2];		// Product limit: Lead + Trail
//} PRODUCTFIT_DIAG;
//
//// Container for a BoardFit from the original list
//typedef struct
//{
//	int			OriginalIndex;	// Index to backtrack source of this data from original list
//	BOARDFITS	Limit;
//} BOARDFIT_DIAG;
//
//// Solution justification + details about solution for the angles attempted
//// At this time WhyNot structure is not filled in
//// Probably it will be completed in Version 5 of Engine
//#define BOARDFITS_DIAG_SIZE		7
//typedef struct {			// For this angle...
//	SOL_PF_MAP		WhyNot;			// Data valid only when in FORCE mode (Certain Boards and no other)
//	SOLUTION		Solution;		// Solution Description for this direction
//										// Room for products saw generated + 1 Strip save
//	PRODUCTFIT_DIAG ProductFit[NUM_SOL_SPEC+1];
//	BOARDFIT_DIAG	BoardFits[BOARDFITS_DIAG_SIZE];	// Subset of the whole Fit List used by BestSolution
//	int				nFitsUsed;		// Utilized of MAX_FITS
//	int				iWidth[NUM_SOL_SPEC+1];			// Solution Target Width (Dimension / RW)
//	int				iBeginLC[2][NUM_SOL_SPEC+1];	// Solution Product Begin [0]=Near, [1]=Far
//	int				BoardFitIndex[3];	// Where Double, triple, etc... being in Fit List
//	int				iCrashValue;		// Value is used at this time as Diagnostic to justify reason for shifting solution
//	int				Spare[15];			// Room to grow
//} SOLUTION_NEW;
//
//
//// calculated on every board being solved
//typedef struct {
//	int	iBestDirection;			// Signed value: MIN_SWEEP_ANGLE to MAX_SWEEP_ANGLE
//	int	iBestDirectionIndex;	// Translated Directions into Solution[]: 0 to MAX_ANGLES-1
//	int	iBoardId[NUM_SOL_SPEC];	// Normally other than -1 when valid (FORCE)
//								// This section was extracted from PREEVAL
//    float LdAngle;				//   taper for lead vne in Width counts/profile
//    float TrlAngle;				//   taper for trail vne
//    int Angles[MAX_ANGLES];		//   Sequence of tapers optimized
//    int CantWidth;				//   maximum Width of cant
//    int CantLength;				//   maximum length of cant
//								// End
//								// Used to compare/validate solution at the Office
//	SOLUTION_NEW	SolutionSummary[TOTAL_ANGLES];	// of this LAST_USED_ANGLE+1 are shown
//	int	iCaseID;				// slm: 06-20-08: This is the MT engine that produced the Best Solution
//	int	Spare[29];				// Room to grow
//} DETAILS_BY_DIRECTION;

#endif