
/** Provides definitions for MotorStageLib  **/

#ifndef __TMCL_TMCLDEFS_H_
#define __TMCL_TMCLDEFS_H_ 1

#include "stdafx.h"

typedef unsigned __int8 uint8_t;
typedef unsigned __int32 uint32_t;

/** TMCLMisc Misc defines **/
#define TMCL_VERSION					3.10			/**< Version of TMCL standard */
#define TMCL_MODULE_ADDRESS				1

/** MotorAxis Defines 
#define Xaxis							1
#define Yaxis							2
**/

/** Datagram sizes for different busses **/
#define TMCL_DGRAM_SIZE_CAN				7				/**< Datagram size for CAN bus (in bytes) */
#define TMCL_DGRAM_SIZE_IIC				8				/**< Datagram size for RS232/RS485 (in bytes) */
#define TMCL_DGRAM_SIZE_RSXXX			9				/**< Datagram size for IIC interface (in bytes) */
#define TMCL_MAX_DGRAM_SIZE	TMCL_DGRAM_SIZE_RSXXX /**< Maximum Datagram size */

/** Internal defines **/
#define TMCL_MAX_PAR_NO					211				/* Highest parameter number */

/** These are the status codes returned by the module. **/
#define TMCL_STATUS_SUCCESS              100			/**< Successfully executed, no error */
#define TMCL_STATUS_LOADED_EEPROM        101			/**< Command loaded into TCML program EEPROM */
#define TMCL_STATUS_WRONG_CHECKSUM         1			/**< Wrong checksum */
#define TMCL_STATUS_INVALID_COMMAND        2			/**< Invalid command */
#define TMCL_STATUS_WRONG_TYPE             3			/**< Wrong type */
#define TMCL_STATUS_INVALID_VALUE          4			/**< Invalid value */
#define TMCL_STATUS_EEPROM_LOCKED          5			/**< Configuration EEPROM locked */
#define TMCL_STATUS_COMMAND_NA             6			/**< Command not available */

/** Commands for controlling the motion of the module. **/
#define TMCL_ROR		  1	/**< Rotate right 	*/
#define TMCL_ROL		  2	/**< Rotate left  	*/
#define TMCL_MST		  3	/**< Motor stop   	*/
#define TMCL_MVP		  4	/**< Move to position 	*/
#define TMCL_RFS		 13	/**< Reference search	*/

/** Commands for setting module parameters. **/
#define TMCL_SAP		  5	/**< Set Axis Parameter	*/
#define TMCL_GAP		  6	/**< Get Axis Parameter	*/
#define TMCL_STAP		  7	/**< Store Axis Parameter	*/
#define TMCL_RSAP		  8	/**< Restore Axis Parameter	*/
#define TMCL_SGP		  9	/**< Set Global Parameter	*/
#define TMCL_GGP		 10	/**< Get Global Parameter	*/
#define TMCL_STGP		 11	/**< Store Global Parameter	*/
#define TMCL_RSGP		 12	/**< Restore Global Parameter*/

#define TMCL_SIO		 14	/**< Set Output	*/
#define TMCL_GIO		 15	/**< Get Input/Output	*/
#define TMCL_CALC		 19	/**< Calculate	*/
#define TMCL_COMP		 20	/**< Compare	*/
#define TMCL_JC		 	 21	/**< Jump Conditional	*/
#define TMCL_JA		 	 22	/**< Jump Always	*/
#define TMCL_CSUB		 23	/**< Call Subroutine	*/
#define TMCL_RSUB	 	 24	/**< Return from Subroutine	*/
#define TMCL_WAIT	 	 27	/**< Wait for an event to occurr	*/
#define TMCL_STOP	 	 28	/**< Stop TMCL program execution	*/
#define TMCL_SAC	 	 29	/**< SPI Bus Access	*/
#define TMCL_SCO	 	 30	/**< Set Coordinate	*/
#define TMCL_GCO	 	 31	/**< Get Coordinate	*/
#define TMCL_CCO	 	 32	/**< Capture Coordinate	*/
#define TMCL_CALCX	 	 33	/**< Calculate using the X register	*/
#define TMCL_AAP		 34	/**< Accumulator to Axis Parameter	*/
#define TMCL_AGP	 	 35	/**< Accumulator to Global Parameter	*/
#define TMCL_CLE		 36	/**< Clear Error Flag	*/
#define TMCL_UF0		 64	/**< User definable command 0	*/
#define TMCL_UF1		 65	/**< User definable command 1	*/
#define TMCL_UF2		 66	/**< User definable command 2	*/
#define TMCL_UF3		 67	/**< User definable command 3	*/
#define TMCL_UF4		 68	/**< User definable command 4	*/
#define TMCL_UF5		 69	/**< User definable command 5	*/
#define TMCL_UF6		 70	/**< User definable command 6	*/
#define TMCL_UF7		 71	/**< User definable command 7	*/

/** Commands for controlling the TMCL module.Not to be used in stand-alone mode **/
#define TMCL_CTL_STOP		128	/**< Stop application */
#define TMCL_CTL_RUN		129	/**< Run application */
#define TMCL_CTL_STEP		130	/**< Only exectute next command of application */
#define TMCL_CTL_RST		131	/**< Reset application */
#define TMCL_CTL_DLM_START	132 /**< Start download mode */
#define TMCL_CTL_DLM_QUIT	133 /**< Stop download mode */
#define TMCL_CTL_READMEM	134 /**< Read TMCL memory */
#define TMCL_CTL_STATUS		135 /**< Get application status */
#define TMCL_CTL_FW_VER		136 /**< Get firmware version */
#define TMCL_CTL_FACTORY	137 /**< Restore factory settings */
/* 138 is reserved */
#define TMCL_CTL_ASCII		139 /**< Enter ASCII mode */

/** Operation type codes for TMCL commands **/
#define TMCL_MVP_ABS		0	/**< Moving to absolute position */
#define TMCL_MVP_REL		1	/**< Moving to relative position */
#define TMCL_MVP_COORD		2	/**< Moving to coordinate */
#define TMCL_RFS_START		0	/**< Starting reference search */
#define TMCL_RFS_STOP		1	/**< Stopping reference search */
#define TMCL_RFS_STATUS		2	/**< Checking status of reference search */

/** Axis parameters to be used with TMCL_SAP, TMCL_GAP, TMCL_AAP, TMCL_STAP and TMCL_RSAP for TMCM-3xx/11x/109/61x modules.
 * @todo These are not complete. **/
//  Parameters that can be read and written

/** Basic parameters */
#define TMCL_AP_TARGET_POS	 	  0	/**< Target (next) postition */
#define TMCL_AP_CURR_POS	 	  1	/**< Current position */
#define TMCL_AP_TARGET_SPEED 	  2	/**< Desired speed in velocity mode */
#define TMCL_AP_MAX_POS_SPEED	  4	/**< Maximum positioning speed */
#define TMCL_AP_MAX_ACCEL		  5	/**< Maximum acceleration */
#define TMCL_AP_ABS_CURRENT	 	  6	/**< Maximum absolute current */
#define TMCL_AP_STBY_CURRENT 	  7	/**< Maximum standby current */
#define TMCL_AP_DISABLE_LIMIT_R	 12 /**< Disable the right limit switch */
#define TMCL_AP_DISABLE_LIMIT_L	 13 /**< Disable the left limit switch */
#define TMCL_AP_SR_PRESC		 14 /**< @note Currently not used */

/** Extended Parameters */
#define TMCL_AP_MICROSTEPS			140	/**< Microstep mode (@see TMCLMicrosteps) */
#define TMCL_AP_MAX_CURR_REST		143 /**< Maximal current at rest (Normally use #TMCL_AP_ABS_CURRENT and #TMCL_AP_STBY_CURRENT) */
#define TMCL_AP_MAX_CURR_LOW_ACCEL	144	/**< Maximal current at low acceleration (Normally use #TMCL_AP_ABS_CURRENT and #TMCL_AP_STBY_CURRENT) */
#define TMCL_AP_MAX_CURR_HIGH_ACCEL	145	/**< Maximal current at high acceleration (Normally use #TMCL_AP_ABS_CURRENT and #TMCL_AP_STBY_CURRENT) */
#define TMCL_AP_RFS_MODE			193	/**< Reference search mode */
#define TMCL_AP_RFS_SPEED			194 /**< Reference search speed mode */
#define TMCL_AP_RFS_SW_SPEED		195 /**< Reference search speed at switch position */
/** 196 and 197 reserved */

//	Read-Only Parameters
/** Basic parameters */
#define TMCL_AP_CURR_SPEED	 	  3	/**< Current speed */
#define TMCL_AP_POS_REACHED 	  8	/**< Target position reached */
#define TMCL_AP_LIMIT_L		 	  9	/**< Right limit switch status */
#define TMCL_AP_LIMIT_R 		 10	/**< Left limit switch status */

/** Get microsteps from microstep mode e.g. for mode 4 (16 microsteps): microsteps[4] = 16 **/
static const int TMCLMicrosteps[] = {0, 1, 4, 8, 16, 32, 64};

typedef struct MotorStatusStruct {
  uint8_t			address;
  uint8_t    		number;				/**< Bank/channel of device */
  int				num_refswitches; 	/**< Number of reference switches on axis */
  uint8_t			switch_stat_a;
  uint8_t			switch_stat_b;
  int				ref_position;
  int				position;

  int				vel_max_posMode;	//Maximum Velocity in Position Mode
  int				vel_min_posMode;		//Minimum velocity in position Mode
  int				accel_max_posMode;	//Maximum Accel in position Mode
  int				vel_max_velMode;	//Maximum velocity in velocity mode

  int				ref_vel;
} Motor;

/** Structure for holding the reply of a module. **/
typedef struct ReplyStruct {
  uint8_t	reply_address;		/**< Reply address */
  uint8_t	module_address;		/**< Module address */
  uint8_t	status;    			/**< @ref StatusCodes "Status Code" **/
  uint8_t	command;   			/**< Command */
  int		value;         		/**< Value */
  uint8_t	checksum;  			/**< Checksum */
} ConnReply;

/** Structure containing a TMCL command and related data.  **/
typedef struct CommandStruct {
  uint8_t 	command;   /* Command */
  uint8_t 	type;  	   /*  Type */
  int		value; 	   /* Value */
} ConnCommand;

uint8_t checksum(uint8_t *commands, int length);

#endif 
