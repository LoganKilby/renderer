/*==============================================================================
 Copyright (c) LMI Technologies & K2 Software Corp. 2001. All rights reserved.
--------------------------------------------------------------------------------
 PROGRAM: NBLIB.DLL

 MODULE : NBLIB.H
 PURPOSE: NBLIB.DLL API declarations and prototypes

--------------------------------------------------------------------------------
 Date       Mod#    Description
--------------------------------------------------------------------------------
 Jan-04-02  000 BS  Formating changes
------------------------------------------------------------------------------*/
#ifndef LMITECHNOLOGIES_NBLIB_H
#define LMITECHNOLOGIES_NBLIB_H

/*---------------------------------CONSTANTS----------------------------------*/

/* All known errors definitions.  Additional constants may be added to this list
   over time.   */
#define NB_NOERROR                      0
#define NB_ERROR_SOCKET                 1
#define NB_ERROR_NOHOST                 2
#define NB_ERROR_WRONG_SOCKET_VERSION   3
#define NB_ERROR_INVALID_FPGA_FILE      4
#define NB_ERROR_READING_FPGA_FILE      5
#define NB_ERROR_NO_MEMORY              6
#define NB_ERROR_UNSUPPORTED_FPGA_FILE  7
#define NB_ERROR_OPEN_FPGA_FILE         8
#define NB_ERROR_NOT_CONNECTED          9
#define NB_ERROR_MISSING_DIAGS          10
#define NB_ERROR_INVALID_PACKET         11
#define NB_ERROR_INVALID_CMD            12
#define NB_ERROR_CMD_TIMEOUT            13
#define NB_ERROR_INVALID_HEAD           14
#define NB_ERROR_INVALID_SPOT           15
#define NB_ERROR_INVALID_CMD_REPLY      16
#define NB_ERROR_HEAD_NOT_PRESENT       17
#define NB_ERROR_NO_BUFFER              18
#define NB_ERROR_API_TIMEOUT            19
#define NB_ERROR_NO_BEGINNING           20
#define NB_ERROR_NO_END                 21
#define NB_ERROR_NO_BEGINNING_NO_END    22
#define NB_ERROR_PACKETS_MISSING        23
#define NB_ERROR_DATA_MISSING           24
#define NB_ERROR_INCORRECT_ARGUMENT     25
#define NB_ERROR_INVALID_TABLE          26
#define NB_ERROR_INVALID_SERIAL_REPLY   27
#define NB_ERROR_CONNECTION_LOST        28
#define NB_ERROR_CONNECTION_CLOSED      29
#define NB_ALREADY_CONNECTED            30
#define NB_TCP_COMMAND_FAILED           31
#define NB_ERROR_UDP_THREAD             32
#define NB_ERROR_BUFFERSIZE             33  // allocated scanlines < user.maxwidth parameter

/* Additional library/application wide definitions      */
#define NB_CAMERAS              2       /* number of cameras per bx sensor    */
#define NB_MAXSPOTS             0x17    /* maximum number of lasers per sensor*/
#define NB_LIGHTCURTAIN_SIZE	14
#define NB_LIGHTCURTAIN_CELLS	216
#define NB_MAXHEADS             24      /* maximum number of supported sensors*/
#define LMI_OUTRANGE            0x8000  /* out of range value                 */
#define MAX_PHOTOCELL_BUFFERS   1024    /* maximum number of photocell buffers*/

/* Asynchronous event types      */
#define ASYNC_EVENT_CONNECTION_LOST 0x00000001
#define ASYNC_EVENT_HEADS_CHANGED   0x00000002
#define ASYNC_EVENT_BOARD_READY     0x00000004
#define ASYNC_EVENT_LOG_PC_INFO     0x00000008

#define NBMAXLOG        16
#define MINVERSIONSZ    16

/* Photocell states */
#define PC_LIGHT        0
#define PC_DARK         1

#define LMI_LASERS_OFF	0x6B
#define LMI_LASERS_ON	0x6A

#define LMI_ENC_FWD		0
#define LMI_ENC_REV		1
/*-----------------------------------TYPES------------------------------------*/

/* The following values can be set through user parameters to select various quadrature modes */

typedef enum
{
    ENCODER_MODE_ONE_COUNT,     /* Sample once per set of quadrature signals */
    ENCODER_MODE_TWO_COUNTS,    /* Sample twise per set of quadrature signals */
    ENCODER_MODE_FOUR_COUNTS,   /* Sample four times per set of quadrature signals */
} ENCODER_MODE;

/* This structure contains diagnostic data pertaining to a single laser spot. */

typedef struct BXSPOTDATA
{
    unsigned short range;                   /* calculated range to target     */
    unsigned char  txPower;                 /* transmitted laser power        */
    unsigned short location[NB_CAMERAS];    /* (subpix) spot location on CCD  */
    unsigned short rxPower[NB_CAMERAS];     /* (sumpix) spot intensity on CCD */
    unsigned char  width[NB_CAMERAS];       /* (numpix) spot width on CCD     */
    unsigned char  spots[NB_CAMERAS];       /* spots present on each CCD      */
} BXSPOTDATA;

/* This structure contains diagnostic data pertaining to a single sensor.     */
typedef struct BXHEADDATA
{
    unsigned int  numSpots;                 /* number of in-range spots seen  */
    unsigned int  model;                    /* model number                   */
    unsigned int  serialNumber;             /* serial number                  */
    unsigned char busAddress;               /* serial bus address             */
    unsigned char FPGAVersion;              /* version of the sensor FPGA code*/
    unsigned char DSPVersion;               /* version of the sensor DSP code */
    double        vdc;                      /* sensor voltage, (volts)        */
    double        temperature;              /* sensor temperature (deg. C)    */
    unsigned char dynamicThreshold0;        /* ambient light threshold value  */
    unsigned char dynamicThreshold1;        /* ambient light threshold value  */
} BXHEADDATA;


/* This structure contains both sensor and spot data for a single sensor.     */
typedef struct BXHEADANDSPOTDATA
{
    int isValid;                    /* non-zero if head is present            */
    BXHEADDATA head;                /* sensor data                            */
    BXSPOTDATA spot[NB_MAXSPOTS];   /* spot data for each laser spot          */
}
BXHEADANDSPOTDATA;

typedef struct BXSENSORPLACEMENT
{
    int isValid;
    int isTop;
} BXSENSORPLACEMENT;

/* Single-byte alignment is used for the following types.  */
#pragma pack(push, 1)

/* This type is used as an argument to nbOpenScanner.      */
typedef struct OPENSCANNERST
{
    char *IPAddress;    /* IP address of the NetPower hub                       */
    char *nicIPAddress; /* IP address of the desired network interface card     */
    HANDLE hEvent;      /* event handle for asynchronous event notifications    */
} OPENSCANNERST;

/*  Used in function nbGetSystemDataInfo to report system capabilities */
typedef struct HEADDATAINFO
{
	unsigned char isValid;
	unsigned char numLasers;
	unsigned char hasLightCurtainData;
	unsigned int scannedObjRangesOffset;
	unsigned int scannedObjCurtainOffset;
} BXHEADDATAINFO;

/* This type is used as an argument to nbOpenScanner.      */
typedef struct SCANNERPARAMS
{
	short lead_spots;     /* min number of valid readings to validate leading edge of a board  */
	short lead_wait;      /* min number of EPs to detect board                                 */
	short trail_spots;    /* max number of spots to initiate trailing edge detection           */
	short trail_wait;     /* number of EPs to finalize a board completion                      */
	short history;        /* history prior leading edge of a board                             */
	short maxwidth;       /* maximum board (lug) width in encoder pulses (EP)                  */
	short maxreverse;     /* allow this many EPs before we need to rescan the board */
	short mode;           /* flag to indicate manual scanning mode - start and end triggered by a photocell (PC) */
	short pcindex_trg;    /* PC (index) used for manual mode */
	short escale;         /* encoder scale factor */
	short pcindex_len;    /* log scanner = 1 board scanner = 0 */
	short segments;       /* specify number of segments of maxwidth to deliver in mode 2 */
	short trail_holdscan; /**/
	short pcindex_stopscan;/* PC index used for manual stop scan, can't be same value as pcindex_trg or pcindex_len */
	short timer_freq;	  /* dynamically change sync pulse...future implementation */	
	short encoder_mode;	  /* changes quadrature division. uses an ENCODER_MODE value */	
	short reserved[10];   /* for future use...costs nothing                          */
} SCANNERPARAMS;

/* This type contains information related to photocell inputs.  */
typedef struct PHOTOCELLST
{
    unsigned int encoderCount;  /* encoder count at time of photocell reading */
    unsigned short photocells;  /* bitmap of photocell input values           */
} PHOTOCELLST;

/* This type describes a single scan line, which contains data from each sensor
   in the system  */
typedef struct ONELINEST
{
    unsigned packetid;                                  /* internal use       */
    unsigned encoder;                                   /* encoder value      */
    unsigned short photocells;                          /* photocell bitmap   */
    unsigned short scanline[NB_MAXHEADS*NB_MAXSPOTS];   /* range readings     */
} ONELINEST;

typedef struct ONELINE2ST
{
    unsigned packetid;                                  /* internal use       */
    unsigned encoder;                                   /* encoder value      */
    unsigned short photocells;                          /* photocell bitmap   */
    unsigned short scanline[NB_MAXHEADS*NB_MAXSPOTS];   /* range readings     */
	unsigned short lightcurtain[NB_MAXHEADS * NB_LIGHTCURTAIN_SIZE];
} ONELINE2ST;

/* This types describes an entire scanned object, consisting of several scan
   lines.  This type is used as an argument to the function
   nbGetScannedObject.      */
typedef struct SCANNEDOBJECTST
{
    int allocatedlines;     // input
    int validlines;         // sum of received lines
    int firstscanline;      // index
    int lastscanline;       // index
    int firstphotoline;     // index
    int lastphotoline;      // index
    unsigned  reserved1;    // internal use
    unsigned  reserved2;    // internal use
    ONELINEST scanline[];   // output
} SCANNEDOBJECTST;

/* SCANNEDOBJECT2ST is similar to the SCANNEDOBJECTST and can be retrieved with
   nbGetScannedObject2 call. The structure also contains light curtain data if
   the sensors support it
*/
typedef struct SCANNEDOBJECT2ST
{
    int allocatedlines;     // input
    int validlines;         // sum of received lines
    int firstscanline;      // index
    int lastscanline;       // index
    int firstphotoline;     // index
    int lastphotoline;      // index
    int boardIndex;         // valid board count at the time this board was received
    unsigned  reserved1;    // internal use
    unsigned  reserved2;    // internal use
    ONELINE2ST scanline[];   // output
} SCANNEDOBJECT2ST;


/* This type describes an asynchronous event and is used as an argument to the
   nbGetLastAsyncEvent function.    */
typedef struct ASYNCEVENTST
{
    int code;       /* indicates the type of asynchronous event that occurred */
    int data;       /* data specific to the asynchronous event type           */
} ASYNCEVENTST;

typedef struct PCSTATEST
{
    short int     last_state;           // keeps the last state status (ON/OFF)
    unsigned int  lead_encoder;         // encoder at the light2dark transition
    unsigned char lead_pcmask;          // photocell mask associated with light2dark transition
    unsigned int  trail_encoder;        // encoder at the dark2light transition
    unsigned char trail_pcmask;         // photocell mask associated with dark2light transition
} PCSTATEST;
/* Return to previous alignment strategy.       */
#pragma pack(pop)

/*----------------------------------FUNCTIONS---------------------------------*/
/* The functions exported from this library use the C linkage convention.  */
#ifdef __cplusplus
extern "C" {
#endif
    
    /*=============================================================================
     The functions exported from this library use the stdcall calling convention.
     If your compiler does not define __stdcall, consult the compiler documentation
     for an appropriate substitution and modify the line below.
     ----------------------------------------------------------------------------*/
#define LMIAPI __stdcall
    void getUdpPktNum(DWORD *pR, DWORD *pC);
    
    /*=============================================================================
     PURPOSE: This function initializes communication with the host.
     INPUT  : OPENSCANNERST *opp: Pointer to a structure of type OPENSCANNERST.
                                  This structure contains two parameters
                                  required to initialize the NBLIB for board
                                  scanning:
    
                  opst.IPAddress: ASCII string, specifying the netpower hub's
                                  internet address, such as "192.168.0.10". If
                                  this field is set to NULL, NBLIB will
                                  attempt to detect the host. Specifying the
                                  host address is typically only neccessary when
                                  more then a single netpower hub is connected.
    
               opst.nicIPAddress: ASCII string, specifying the internet address
                                  of the network interface card (NIC) through which
                                  communication with the netpower hub will take
                                  place.  If this field is set to NULL, NBLIB will
                                  use the system's default NIC. Specifying this
                                  address is typically only useful when the
                                  client (PC) contains more than one NIC.
    
                     opst.hEvent: Contains a handle to an event. If this
                                  handle is valid, the library will signal
                                  all asynchronuous events using this handle.
                                  It is the caller's responsibility to create
                                  manual resettable event by a call such as:
                                  opst.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
                                  If this field is set to NULL, the library
                                  will not issue any asynchronuous event
                                  notifications.
    
          SCANNERPARAMS *user_sp: These parameters control the operation of the
                                  board detection state machine within the
                                  netpower hub.
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: This function must be called before making any other calls that use
              NBLIB communication. A TRUE return value from this function indicates
              that the library is successfully communicating with the netpower hub.
    
              For more informations on asynchronuous events see description of
              nbGetLastAsyncEvent(ASYNCEVENTST *apt)
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbOpenScanner(OPENSCANNERST *opp, SCANNERPARAMS *user_sp);
    
    /*=============================================================================
     PURPOSE: This function returns the information about data capabilities of the system.
                  
     INPUT  : BXHEADDATAINFO * buffer : preallocated buffer(should have 
                                        size NBMAXHEADS*sizeof(BXHEADDATAINFO))
                          int maxHeads: specifies maximum number of heads the provided
                                        buffer can store
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: 
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetSystemDataInfo(BXHEADDATAINFO * buffer,int maxHeads);
    
    /*=============================================================================
     PURPOSE: This function terminates communication with the netpower hub and
              performs any necessary cleanup.
     INPUT  : none
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: If nbOpenScanner returned TRUE when called to initialize the host
              communication, nbCloseScanner should be called to perform any
              necessary cleanup. If this function is not called, resource leaks
              may result.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbCloseScanner();
    
    /*=============================================================================
     PURPOSE: This function is called to determine whether the library is
              currently using the indicated laser in acquiring board scan data.
     INPUT  : int head: The sensor index of the head whose status should be returned.
              int spot: The spot index of the laser whose status should be returned.
              BOOL *enable: Returns the status of the specified spot. A value
                            of TRUE indicates that the spot is currently used in
                            acquiring board scan data. A value of FALSE indicates
                            that the spot will be always Out of Range.
    
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
    
     COMMENT: All spots for any head that is not available will be implicitly
              disabled. If any lasers are known to return spurious readings, they
              can be disabled with a call to nbSetLaserStatus. Disabling a spot
              has no effect on the actual sensor but indicates to the library
              that the data from the spot should not be used.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetLaserStatus (int head, int spot, int *enable);
    
    /*=============================================================================
     PURPOSE: This function is called to specify whether the library should use
              the indicated laser in acquiring board scan data.
     INPUT  :    int head: The sensor index of the head whose status should be set.
                 int spot: The spot index of the laser whose status should be set.
              BOOL enable: The status of the specified spot. A value
                           of TRUE indicates that the spot should be used in
                           acquiring board scan data. A value of FALSE indicates
                           that the spot will be always Out of Range.
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: When the library is initialized, all lasers for all heads are
              set to be enabled. If any lasers are known to return spurious
              readings, they can be disabled with a call to this function.
              Disabling a spot has no effect on the actual sensor but indicates
              to the library that the data from the spot should not be used when
              acquiring board scans.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbSetLaserStatus(int head, int spot, BOOL enable);
    
    /*=============================================================================
     PURPOSE: This function specifies ,where each corresponding word will be ANDed
              with the curtain returned from the specified sensor.
     INPUT  :    int head: The sensor index of the head whose status should be set.
                 unsigned short * curtainMask: The buffer which specifies the mask for 
                                               sensor's curtain buffer.
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: curtainMask must point to a buffer NB_LIGHTCURTAIN_SIZE words in size. 
              When the library is initialized all of the curtain bits are enabled. 
              The bit order is expected to be exactly the same as the one in the curtain
              buffer returned by the sensor.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbSetCurtainStatus(int head,unsigned short * curtainMask);
    
    /*=============================================================================
     PURPOSE: This function returns the mask buffer currently used for a given sensor.
     INPUT  :    int head: The sensor index of the head whose status should be set.
                 unsigned short * curtainMask: The buffer which receives the mask for 
                                               sensor's curtain buffer.
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: curtainMask must point to a buffer NB_LIGHTCURTAIN_SIZE words in size. 
    
     -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetCurtainStatus(int head,unsigned short * curtainMask);
    
    /*=============================================================================
     PURPOSE: This function is called to retrieve diagnostic data for a particular
              laser.
     INPUT  :             int head: The logical sensor index of the desired sensor.
                          int spot: The index of the laser spot for which
                                    diagnostic data are desired.
              BXSPOTDATA *headData: Pointer to a structure of type BXSPOTDATA that
                                    is to be filled in with diagnostic data read
                                    from the sensor.
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: This function may require a long time (on the order of one second)
              to complete.  Do not use this function in time-critical code
              that requires rapid, deterministic completion.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetLaserInfo(int head,int spot,BXSPOTDATA* spotData);
    
    /*=============================================================================
     PURPOSE: This function is called to retrieve diagnostic data for a particular
              sensor.
     INPUT  :             int head: The logical sensor index of the desired sensor.
              BXHEADDATA *headData: Pointer to a structure of type BXHEADDATA that
                                    is to be filled in with diagnostic data read
                                    from the sensor.
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: This function may require a long time (on the order of one second)
              to complete.  Do not use this function in time-critical code
              that requires rapid, deterministic completion.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetHeadInfo(int head, BXHEADDATA *headData);
    
    
    /*=============================================================================
     PURPOSE: This function fills a user buffer with the current photocell
              information and current encoder count.
     INPUT  : PHOTOCELLST *photocellsinfo: user buffer to be filled with the
                                           current photocells value and encoder
                                           count.
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: When the netpower hub is first powered-on, the encoder count is
              initialized to zero. The encoder count is then incremented with each
              received encoder pulse until a counter overflow occurs. The encoder
              count is then zeroed and begins counting up again.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetPhotocells(PHOTOCELLST *photocellsinfo);
    
    /*=============================================================================
     PURPOSE: This function fills a user buffer with the current photocell
              information and current encoder count
     INPUT  : PHOTOCELLST *photocellsbuffer: user buffer to be filled with the
                                             most recent photocells values and
                                             encoder counts.
    
                             int buffersize: number of elements in photocellsbuffer.
                                             [1..MAX_PHOTOCELL_BUFFERS]
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: At each encoder tick the value of photocells is recorded by the
              host in a ring buffer. The most recently recorded data may be
              recovered using this API. The netpower hub buffer can hold at least
              MAX_PHOTOCELL_BUFFERS readings. An attempt to read more data than
              the buffer contains will lead to an error.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetPhotocellsHistory(PHOTOCELLST *photocellsbuffer,int buffersize);
    
    /*=============================================================================
     PURPOSE: This function fills a user buffer with the current encoder count.
     INPUT  : unsigned int *encoderCount: user buffer to be filled with the current
                                          encoder count.
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: When the netpower hub is first powered-on, the encoder count is
              initialized to zero. The encoder count is then incremented with each
              received encoder pulse until a counter overflow occurs. The encoder
              count is then zeroed and begins counting up again.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetEncoderInfo(unsigned int *encoderCount);
    
    /*=============================================================================
     PURPOSE: This function sets the current encoder count to a specific value.
     INPUT  : unsigned int encoderCount: user value to replace the current encoder
                                         count.
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: When the host is first powered-on, the encoder count is initialized
              to zero. The encoder count is then incremented with each received
              encoder pulse until a counter overflow occurs.
              The encoder count is then zeroed and begins counting up again.
              This function explicitly sets the current encoder count to a
              specific value.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbSetEncoderInfo(unsigned int encoderCount);
    
    /*=============================================================================
     PURPOSE: Functions nbGetScannedObject and nbGetScannedObject2 gather scan 
              information into a user provided buffer. The function detects the 
              leading and trailing edge of a board being scanned and recalculates 
              the encoder/scans positions using the offsets table.
    
    
     INPUT  : SCANNEDOBJECTST *buffer: pointer to an user allocated structure
                                       containing the scan buffer(Used in nbGetScannedObject)
    
              SCANNEDOBJECTST *buffer: pointer to an user allocated structure
                                       containing the scan buffer.Also includes the light curtain
                                       data if the system supports it.(Used in nbGetScannedObject2)
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
    
     COMMENT: This function will attempt to gather scan and photocells information
              for each encoder tick and store it in the user buffer, line by line.
              The user must allocate the buffer to hold enough scan lines using a
              call such as:
    
              buffer = (SCANNEDOBJECTST*)malloc(sizeof(SCANNEDOBJECTST) +
                       maxscanlines*sizeof(ONELINEST));
    
              If the buffer is successfully allocated, the field allocatedlines
              must be initialized:
    
              if (buffer != NULL)
                  buffer->allocatedlines = maxscanlines;
              else
                 // ....process error
    
              After the buffer has been successfully allocated and initialized,
              we can call the API:
    
              if (nbGetScannedObject(buffer))
                 {
                 // ...process scanlines and photocells
                 }
              else
                 {
                 // ...process error
                 error = nbGetLastError(NULL);
                 }
    
              Upon succesfull completion of this function, the SCANNOBJECTST fields
              are filled as follows:
    
              buffer.firstscanline:  contains the index of the buffer line with the
                                     first board scan information.
              buffer.lastscanline:   contains the index of the buffer line with the
                                     last board scan information.
              buffer.firstphotoline: contains the index of the buffer line with the
                                     first valid photocell information.
              buffer.lastphotoline:  contains the index of the buffer line with the
                                     last valid photocell information.
    
              All additional fields are for internal use only and should be ignored
              by the user. Each valid scanline contains the encoder position,
              photocell values and the actual scan data. The scan data is adjusted
              using the internal offsets table. (See nbSetOffsets for more
              information).
    
              This function can generate the following errors, as well as others:
    
              NB_ERROR_API_TIMEOUT:         no board ready at present
              NB_ERROR_NO_BEGINNING:        function failed to detect leading edge
              NB_ERROR_NO_END:              function failed to detect trailing edge
                                            before the user buffer was filled.
              NB_ERROR_NO_BEGINNING_NO_END: function failed to detect both the
                                            leading and the trailing edge.
              NB_ERROR_PACKETS_MISSING:     Some UDP packets sent by host were lost.
              NB_ERROR_DATA_MISSING:        Scan data missing, typically caused by
                                            encoder being too fast.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetScannedObject(SCANNEDOBJECTST *buffer);
    BOOL LMIAPI nbGetScannedObject2(SCANNEDOBJECT2ST *buffer);
    
    /*=============================================================================
     PURPOSE: This function fills a user buffer with ranges as read by the host.
     INPUT  : int head: logical head number
              int numspots: number of range readings requested
              unsigned short *ranges: user buffer to be filled with the ranges.
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: The buffer must be large enough to hold all requested readings.
              A possible calling sequence would be
    
              buffer = (short*)malloc(sizeof(short)*NB_MAXSPOTS);
    
              if (nbGetRanges(head,NB_MAXSPOTS,buffer))
                  {
                  //  Process ranges
                  }
              else
                  {
                  //  Process error
                  }
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetRanges(int head, int numSpots, unsigned short *ranges);
    
    /*=============================================================================
     PURPOSE: This function fills a user buffer with ranges and light curtain data
              as read by the host. 
     INPUT  : int head: logical head number
              unsigned short *ranges: user buffer to be filled with the ranges.
                                      Must contain space for NBMAXSPOTS ranges.
              unsigned short *curtain: user buffer to be filled with the curtain data.
                                      Must be sized NB_LIGHTCURTAIN_SIZE*sizeof(unsigned short).
                                      
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: The range buffer must contain space for NBMAXSPOTS ranges. All available sensor
              ranges will be returned. The curtain buffer is fixed and must be at least 
              the size sizeof(short)*NB_LIGHTCURTAIN_SIZE.
              A possible calling sequence would be
    
              ranges = (short*)malloc(sizeof(short)*NB_MAXSPOTS);
              curtain = (short*)malloc(sizeof(short)*NB_LIGHTCURTAIN_SIZE);
    
              if (nbGetRangesAndCurtain(head,buffer,curtain))
                  {
                  //  Process data
                  }
              else
                  {
                  //  Process error
                  }
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetRangesAndCurtain(int head,unsigned short *ranges, unsigned short *curtain);
    
    
    /*=============================================================================
     PURPOSE: This function fills a user buffer with light curtain data
              as read by the host. 
     INPUT  : int head: logical head number
              unsigned short *curtain: user buffer to be filled with the curtain data.
                                      Must be sized NB_LIGHTCURTAIN_SIZE*sizeof(unsigned short).
                                      
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: The curtain buffer is fixed and must be at least the size sizeof(short)*NB_LIGHTCURTAIN_SIZE.
              A possible calling sequence would be
    
              curtain = (short*)malloc(sizeof(short)*NB_LIGHTCURTAIN_SIZE);
    
              if (nbGetCurtain(head,curtain))
                  {
                  //  Process data
                  }
              else
                  {
                  //  Process error
                  }
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetCurtain(int head,unsigned short *curtain);
    
    /*=============================================================================
     PURPOSE: The function is called to acquire range readings from every sensor in
              in the system.
     INPUT  :              int numHeads: the number of sensors in the system
                                         (0..NB_MAXHEADS)
                           int numSpots: the number of spots for each sensor
                                         (0..NB_MAXSPOTS)
              unsigned int *isAvailable: reports whether the sensor is present
                 unsigned short *ranges: reports the current range readings
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: The isAvailable buffer must contain at least numHeads elements.  The
              ranges buffer must contain at least numHeads*numSpots elements.
              e.g.
    
              isAvailable = (int*)malloc(sizeof(int)*numHeads);
              ranges = (unsigned short*)malloc(sizeof(unsigned short)*numHeads*numSpots);
    
              if (nbGetAllRanges(numHeads, numSpots, isAvailable, ranges))
                  {
                  for (i = 0; i < numHeads; ++i)
                      {
                      if (isAvailble[i])    //was this sensor present in the system?
                          {
                          for (j = 0; j < numSpots; ++j)
                              {
                                  currentRange = ranges[i][j];
                                  //do something with the range data
                              }
                          }
                      }
                  }
              else
    
                  //process error
                  }
    
    
              Note that the ranges for any sensor that is not available will be set
              to LMI_OUTRANGE.
    
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetAllRanges(int numHeads, int numSpots, int *isAvailable, unsigned short *ranges);
    
    /*=============================================================================
     PURPOSE: Returns the version number of this software library.
     INPUT  : none
     RETURN : int version : the current version of this library.
     COMMENT: Version is of format (major*100 + minor), i.e. value 123 should be
              interpreted as version 1.23.
    -----------------------------------------------------------------------------*/
    int LMIAPI nbGetVersion();
    
    /*=============================================================================
     PURPOSE: Read NPH firmware code version.
     INPUT  : char *version destination string
              int buffersz  size of the destination string (min MINVERSIONSZ)
     RETURN : TRUE : success
              FALSE: error
     COMMENT:
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetFwVersion(char *version);
    
    /*=============================================================================
     PURPOSE: This function fills a user buffer with the current FPGA version.
    
     INPUT  : unsigned int *FPGAversion: version returned by the netpower hub.
    
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetFPGAVersion(unsigned int *FPGAversion);
    
    /*=============================================================================
     PURPOSE: This function returns the last asynchronuous event information.
    
     INPUT  : ASYNCEVENTST *apt: Pointer to a structure of type ASYNCEVENTST that
                                 will be filled with valid information.
    
     RETURN : int : the code of the last asynchronuous event.
    
     COMMENT: The user is optionally notified of some asynchronuous events (see
              nbOpenScanner). If an event is signalled, user can retrieve relevant
              information using this function. User will typically start a separate
              thread that waits for an event to be signalled:
    
              hEventThread = StartReadingThread((LPTHREAD_START_ROUTINE)EventWaitThread);
    
              The thread itself is an ifinite loop waiting for and processing all
              events:
    
              void EventWaitThread()
                  {
                  ASYNCEVENTST aest;
    
                  while (1)
                      {
                      WaitForSingleObject(ghEvent,INFINITE);
    
                      nbGetLastAsyncEvent(&aest);
    
                      if (aest.code == ASYNC_EVENT_CONNECTION_LOST)
                          {
                          // process connection lost...
                          }
                      else if (aest.code == ASYNC_EVENT_HEADS_CHANGED)
                          {
                          HeadsMask = aest.data; //bitfield of active heads
                          // process heads change. Some head died
                          // (or came back to life!)
    
                          }
    
                      // allow signalling again
                      ResetEvent(ghEvent);
                      }
                  }
    
              The code above assumes the variable ghEvent contains the same event
              handle that was passed to nbOpenScanner();
    -----------------------------------------------------------------------------*/
    int LMIAPI nbGetLastAsyncEvent(ASYNCEVENTST *apt);
    
    /*=============================================================================
     PURPOSE: This function is called to specify the range offsets table used
              by nbGetScannedObject. The table must have one entry for each spot
              for all sensors. The offsets table should contain data that has been
              calculated to compensate for sensor misalignments.
    
     INPUT  : int *offsets: Pointer to the offsets table.
              int bytesize: size of the offsets table in bytes.
                            Should be sizeof(int)*NB_MAXHEADS*NB_MAXSPOTS
                            The table must be of full size even if not all sensors
                            are installed.
    
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
    
     COMMENT: When the library is initialized, the offsets are all set to zero.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbSetOffsets(int *offsets,int bytesize);
    
    /*=============================================================================
     PURPOSE: This function is called to obtain the current range offsets table used
              by nbGetScannedObject.
    
     INPUT  : int *offsets: Pointer to an user buffer to copy the table into.
              int bytesize: size of the user buffer in bytes.
                            Must be at least sizeof(int)*NB_MAXHEADS*NB_MAXSPOTS
    
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetOffsets(int *offsets,int bytesize);
    
    /*=============================================================================
     PURPOSE: This function is called to specify the light curtain offsets table used
              by nbGetScannedObject. The table must have one entry for each light 
              curtain cell(bit) for all sensors. 
    
     INPUT  : int *curtainOffsets: Pointer to the offsets table.
              int bytesize: size of the offsets table in bytes.
                            Should be sizeof(int)*NB_MAXHEADS*NB_LIGHTCURTAIN_CELLS
                            The table must be of full size even if not all sensors
                            are installed.
    
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
    
     COMMENT: When the library is initialized, the offsets are all set to zero.
              Note that the offsets for the unused light curtain bits in the scanned object
              buffer should not be specified. For each sensor the number of offset values 
              should be exactly NB_LIGHTCURTAIN_CELLS
              
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbSetCurtainOffsets(int *curtainOffsets,int bytesize);
    
    
    /*=============================================================================
     PURPOSE: This function is called to obtain the current light curtain offsets table used
              by nbGetScannedObject.
    
     INPUT  : int *curtainOffsets: Pointer to an user buffer to copy the table into.
              int bytesize: size of the user buffer in bytes.
                            Must be at least sizeof(int)*NB_MAXHEADS*NB_LIGHTCURTAIN_CELLS
    
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetCurtainOffsets(int *curtainOffsets,int bytesize);
    
    /*=============================================================================
     PURPOSE: This function is called to remap the sensor indices from connector
              based (physical) to logical numbers used in a client application.
     INPUT  :   int *table: Pointer to the sensor table.
                            The mapping is performed as:
                            logical = table[physical]
                            The table must contain unique values for all sensors.
              int bytesize: size of the sensor table in bytes.
                            Must be at least sizeof(int)*NB_MAXHEADS
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: The default sensor map maps connector indices to logical indices
              as follows:
    
              int SensorMap[NB_MAXHEADS] =
                 {
                 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,
                 12,13,14,15,16,17,18,19,20,21,22,23,
                 };
    
              The SensorMap is used internally by all APIs dealing with sensor
              data.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbSetSensorMap(int *table,int bytesize);
    
    /*=============================================================================
     PURPOSE: This function is called to get a copy of the current sensor map.
     INPUT  :   int *table: Pointer to the user buffer to receive the copy of the
                            sensor table.
              int bytesize: size of the user buffer in bytes.
                            Must be at least sizeof(int)*NB_MAXHEADS
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: This function is a complement of the function nbSetSensorMap.
              See nbSetSensorMap for more details on the sensor table.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetSensorMap(int *table,int bytesize);
    
    
    /*=============================================================================
     PURPOSE: This function may be called to retrieve an error code after
              a function in the nblib library returns unsuccessfully.
     INPUT  : const char **description: if not NULL, the library will return
                                        a pointer to an ASCII string describing
                                        the last error.
     RETURN : int: numeric code of the last error.
     COMMENT: If a function in the NBLIB library returns unsuccessfully, call this
              function immediately to determine the cause of the error. If any
              calls are made to the NBLIB library between the unsuccessful call and
              the call to this function, the error information will most likely be
              lost.
              The return value of nbGetLastError is undefined when the most recent
              call to the NBLIB library returned successfully.
    -----------------------------------------------------------------------------*/
    int LMIAPI nbGetLastError(const char** description);
    
    
    /*=============================================================================
     PURPOSE: This function can be used to retrieve the number of lasers present in
              given sensor. 
     INPUT  : int head: logical index of the head in question
              int *numLasers: output value for the number of lasers in the specified head
     RETURN : int: numeric code of the last error.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetValidLasers(int head,int *numlasers);
    
    BOOL LMIAPI nbGetLog( unsigned int *nblog, int size);
    void LMIAPI nbSetLog(void);
    
    /*=============================================================================
     PURPOSE: This function is called to soft-reset all connected sensors.
     INPUT  : int address   logical address of a sensor to reset.
                            0 = general broadcast - all the sensors accept
     RETURN : TRUE : success
              FALSE: error
     COMMENT:
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbSensorReset(int addr);
    
    /*=============================================================================
     PURPOSE: This function is called to upload new SCANNERPARAMS to the NPH unit
     INPUT  : SCANNERPARAMS *user_sp
     RETURN : TRUE : success
              FALSE: error
     COMMENT: Use caution when calling this function. This operation may be result
              in temporarily unpredictable behaviour such as loss of currently
              scanned board dat. User should use this call ONLY if no board is being
              scanned.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbUploadUserParameters(SCANNERPARAMS *user_sp);
    
    /*=============================================================================
     PURPOSE: This function is called to acquire both the sensor and the spot
              diagnostic information from all sensors in the system.
     INPUT:   BXHEADANDSPOTDATA *systemData:
                array in which data is returned.
                !! Allocate fixed size: NB_MAXHEADS x BXHEADANDSPOTDATA as it will
                fill in the information for each NPH66 input (connector).
              int *numHeads:
                stores number of detected sensors in the system
              int numSpots:
                the number of laser spots for sensors in the system
                            (0..NB_MAXSPOTS) e.g. 8 for a B8
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: This function may require a long time (on the order of one second)
              to complete.  Do not use this function in time-critical code
              that requires rapid, deterministic completion.
              Also will not support mixed systems with different sensor models at
              this time.
    ---------------------------------------------------------------------------*/
    BOOL LMIAPI nbGetSystemData(BXHEADANDSPOTDATA *systemData, int *numHeads, int numSpots);
    
    /*=============================================================================
     PURPOSE: This function is called to find retrieve photocell info logs on the
              last target. PCSTATEST structure holds photocell transition states
              recorded during the last log scan.
     INPUT  : PCSTATEST *nbpcstates
     RETURN : TRUE : success
              FALSE: error
     COMMENT: This call can be made anytime after board ready event.
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbReadLogLength(PCSTATEST *nbpcstates, int source);
    
    /*=============================================================================
     PURPOSE: This function is called to hard reset the NPH unit.
     INPUT  : void
     RETURN : void
     COMMENT:
    -----------------------------------------------------------------------------*/
    void LMIAPI nbResetNPH(void);
    
    /*=============================================================================
     PURPOSE: Software trigger one time shot.
     INPUT  : int direction - indicates expected encoder direction
              LMI_ENC_FWD - forward (incrementing encoder value)
              LMI_ENC_REV - reverse (decrementing encoder value)
     RETURN : success/failure
     COMMENT:
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbStartScan(int direction);
    
    /*=============================================================================
     PURPOSE: Software termination of a scan segment.
     INPUT  : void
     RETURN : void
     COMMENT:
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbStopScan(void);
    
    /*=============================================================================
     PURPOSE: Set all lasers ON/OFF for the whole system
     INPUT  : cmd	LMI_LASERS_ON | LMI_LASERS_OFF
     RETURN : void
     COMMENT: 
    -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbSetAllLasers(int cmd);
    
    
    /*=============================================================================
     COMMENT: This function is provided to for compatibility with older versions of
              NPH systems. The number of lasers present in a sensor(defines the amount of
              data sent to the host) is now determined on startup. The function 
              will always return TRUE and will have no effect on the system.
              nbGetValidLasers can still be used to determine number of lasers present.          
      -----------------------------------------------------------------------------*/
    BOOL LMIAPI nbSetValidLasers(int head,int numlasers);
    
    
    /*=============================================================================
     PURPOSE: The function is called to acquire curtain data from every sensor in
              in the system.
     INPUT  :              int numHeads: the number of sensors in the system
                                         (0..NB_MAXHEADS)
              unsigned int *isAvailable: reports whether the sensor is present
              unsigned short *curtain: reports the current curtain data
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
     COMMENT: The isAvailable buffer must contain at least numHeads elements.  The
              curtain buffer must contain at least numHeads*NB_LIGHTCURTAIN_SIZE elements.
              e.g.
              
              curtainBits = NB_LIGHTCURTAIN_SIZE*BITS_IN_WORD;
              isAvailable = (int*)malloc(sizeof(int)*numHeads);
              curtain = (unsigned short*)malloc(sizeof(unsigned short)*numHeads*NB_LIGHTCURTAIN_SIZE);
    
              if (nbGetAllCurtain(numHeads,isAvailable, curtain))
                  {
                  for (i = 0; i < numHeads; ++i)
                      {
                      if (isAvailble[i])    //was this sensor present in the system?
                          {
                              sensorCurtain = &curtain[i* NB_LIGHTCURTAIN_SIZE];
                              for (j = 0; j < curtainBits; ++j)
                              {
                                bitValue = sensorCurtain[bit / BITS_IN_WORD] & (0x8000 >> (bit % BITS_IN_WORD))
                              }
                          }
                      }
                  }
              else
    
                  //process error
                  }
    
    
              Note that the ranges for any sensor that is not available will be set
              to LMI_OUTRANGE.
    
    -----------------------------------------------------------------------------*/
    
    BOOL LMIAPI nbGetAllCurtain(int numHeads, int *isAvailable, unsigned short *curtain);
    
    /*=============================================================================
     PURPOSE: The function returns information about the placement of each head,
              as previously configured by the user.
    
     INPUT  :   
                BXSENSORPLACEMENT * placement: output buffer for the placement 
                                               information                        
                int maxHeads: number of heads in the system (0..NB_MAXHEADS)
    
     RETURN : TRUE : success
              FALSE: error. Call nbGetLastError to retrieve more information on
                     the nature of the failure.
    
     COMMENT: The size of the placement buffer must be at least 
              sizeof(BXSENSORPLACEMENT)*maxHeads
    
    -----------------------------------------------------------------------------*/
    
    BOOL LMIAPI nbGetSystemPlacement(BXSENSORPLACEMENT * placement,int maxHeads);
    
#ifdef __cplusplus
}       /* extern "C" */
#endif


#endif      /* LMITECHNOLOGIES_NBLIB_H */