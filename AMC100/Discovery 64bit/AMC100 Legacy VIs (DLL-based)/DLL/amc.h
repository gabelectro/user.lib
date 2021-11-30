/*****************************************************************************/
/** @mainpage AMC DLL
 *
 *  \ref amc.h "The amc.dll" is a library that allows custom programming for the AMC100 
 *  piezo controller.
 *  AMC100 devices can be searched by @ref AMC_Check, connected by @ref AMC_Connect
 *  and released by @ref AMC_Close functions.
 *  It is possible to handle multiple devices simultaneously. the @ref AMC_Connect 
 *  function gives a unique handle for the selected device.
 *
 *  Functions that allow set and get operations are named AMC_control... .
 *  A boolean parameter (Bln32 set) determines the way which is meant.
 *
 *  Documentation for dll functions can be found
 *  \ref amc.h "here".
 */
/*****************************************************************************/

/******************************************************************/
/** @file amc.h
 *  AMC100 DLL
 *
 *  Defines functions for connecting and controlling the AMC100
 */
/******************************************************************/



#ifndef __AMC_H__
#define __AMC_H__


/** Definitions for the windows DLL interface                                        */
#ifdef unix
#define NCB_API
#else
#ifdef  DLL_EXPORT
#define NCB_API __declspec(dllexport) _stdcall  /**< For internal use of this header */
#else
#define NCB_API __declspec(dllimport) _stdcall  /**< For external use of this header */
#endif
#endif




#ifdef __cplusplus
extern "C" {
#endif

typedef int Bln32;                              /**< Boolean compatible to older C      */
typedef int Int32;                              /**< Basic type                         */

/** Return values of functions */
#define NCB_Ok                   0              /**< No error                              */
#define NCB_Error              (-1)             /**< Unspecified error                     */
#define NCB_NotConnected        -2              /**< No active connection to device        */
#define NCB_DriverError         -3              /**< Error in comunication with driver     */
#define NCB_NetworkError        -4              /**< Network error when connecting to AMC  */
#define BAD_IP_ADDRESS          -5
#define CONNECTION_TIMEOUT      -6
#define NO_DEVICE_FOUND_ERR     -7
#define NCB_InvalidParam        -9              /**< Parameter out of range                */
#define NCB_FeatureNotAvailable 10              /**< Feature only available in pro version */


/** @brief  Actor types                                                              */
typedef enum {
  AMC_actorLinear = 0,                       /**< Actor is of linear type            */
  AMC_actorGonio  = 1,                       /**< Actor is of goniometer type        */
  AMC_actorRot    = 2                        /**< Actor is of rotator type           */
} AMC_actorType;


/** @brief  RealTime Output Mode                                                               */
typedef enum {
	AMC_RTOUT_AQUADB_LVTTL = 0,
	AMC_RTOUT_AQUADB_LVDS = 1
} AMC_RTOUT_MODE;

/** @brief  RealTime Input Mode                                                               */
typedef enum {
	AMC_RTIN_AQUADB_LVTTL = 0,
	AMC_RTIN_AQUADB_LVDS = 1,
	AMC_RTIN_STEPPER_LVTTL = 8,
	AMC_RTIN_STEPPER_LVDS = 9,
	AMC_RTIN_TRIGGER_LVTTL = 10,
	AMC_RTIN_TRIGGER_LVDS = 11,
	AMC_RTIN_OFF = 15,
} AMC_RTIN_MODE;

/** @brief  RealTime Input Feedback Loop Mode                                                               */
typedef enum {
	AMC_RTIN_LOOP_MODE_OPEN_LOOP = 0,
	AMC_RTIN_LOOP_MODE_CLOSED_LOOP = 1,
} AMC_RTIN_LOOP_MODE;


/** @brief Connect device
 *
 *  Initializes and connects the selected AMC device.
 *  The function has to be called before any function operating the device,
 *  but after @ref AMC_Check.
 *
 *  @param  deviceAddress:Address of the device to connect, 
 *                        must be smaller than the number of found devices.
 *  @param  deviceHandle: Output: Device handle that is used to address the
 *                        device in further communication.
 *  @return Result of function
 */
Int32 NCB_API AMC_Connect( const char *deviceAddress, Int32* deviceHandle );



/** @brief Close connection
 *
 *  Closes the connection to the device.
 *
 *  @param  deviceHandle: Handle of device
 *
 *  @return  Result:  NCB_Ok (exclusive result value)
 */
Int32 NCB_API AMC_Close( Int32 deviceHandle );


/** @brief get lock status
*
*  get information weither the device is locked or not and if access is authorized
*
*  @param  locked: indicates if the device is locked
*  @param  authorized: indicates if access is granted for this dll
*
*  @return   Result of function
*/
Int32 NCB_API AMC_getLockStatus(Int32 deviceHandle, Bln32* locked, Bln32* authorized);


/** @brief lock device with password
*
*  lock the device so that it will only be possible to call functions by knowing the password
*
*  @param  password: null-terminated string
*
*  @return   Result of function
*/
Int32 NCB_API AMC_lock(Int32 deviceHandle, char* password);

/** @brief request access to a locked device
*
*  @param  password: null-terminated string
*
*  @return   Result of function
*/
Int32 NCB_API AMC_grantAccess(Int32 deviceHandle, char* password);


/** @brief get error text from error code
*
*  @param  lang : number indicating error text lang
*  @param  errcode : error code to translate
*  @param  error : output buffer where the msg will be stored (min 50 bytes)
*
*  @return   Result of function
*/
Int32 NCB_API AMC_errorNumberToString(Int32 deviceHandle, int lang, int errcode, char* error);

/** @brief unlock the device
*
*  lock the device so that it will be available to all clients
*
*  @return   Result of function
*/
Int32 NCB_API AMC_unlock(Int32 deviceHandle);


/** @brief Control output stage
 *
 *  Controls the output relais of the selected axis.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  enable        Switches the output relais
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlOutput( Int32 deviceHandle, 
                                 Int32 axis,
                                 Bln32* enable,
                                 Bln32 set );

/** @brief Control amplitude
 *
 *  Controls the amplitude of the actuator signal.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  amplitude     Amplitude in mV
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlAmplitude( Int32 deviceHandle, 
                                    Int32 axis,
                                    Int32* amplitude,
                                    Bln32 set );

/** @brief Control frequency
 *
 *  Controls the frequency of the actuator signal.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  frequency     Frequency in mHz
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlFrequency( Int32 deviceHandle,
                                    Int32 axis,
                                    Int32* frequency,
                                    Bln32 set );


/** @brief Control actor selection
 *
 *  Selects the actor to be used on selected axis from actor presets
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  actor         Actor selection [0..255]
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlActorSelection( Int32 deviceHandle,
                                         Int32 axis,
                                         Int32* actor,
                                         Bln32 set );

/** @brief Get actor name
 *
 *  Get the name of actual selected actor
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  name          Name of the actor coded as NULL-terminated c-string
 *  @param  size          size of the buffer
 *  @return               Result of function
 */
Int32 NCB_API AMC_getActorName( Int32 deviceHandle, 
                                Int32 axis,
                                char* name,
                                Int32 size);



/** @brief Get actor type
 *
 *  Get the type of actual selected actor
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  type          Type of the actor. See @ref AMC_actorType
 *  @return               Result of function
 */
Int32 NCB_API AMC_getActorType( Int32 deviceHandle, 
                                Int32 axis,
                                AMC_actorType* type );


/** @brief Reset position
 *
 *  Resets the actual position to zero and marks the reference position as invalid.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @return               Result of function
 */
Int32 NCB_API AMC_setReset( Int32 deviceHandle,
                            Int32 axis );


/** @brief Control actor approach
 *
 *  Controls the approach of the actor to the target position
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  enable        Enables/ disables the approach
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlMove( Int32 deviceHandle,
                               Int32 axis,
                               Bln32* enable,
                               Bln32 set );


/** @brief Single step
 *
 *  Triggers N steps in desired direction.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  backward      Selects the desired direction. False triggers a forward step,
 *                        true a backward step.
 *  @param  n             Number of steps
 *  @return               Result of function
 */
Int32 NCB_API AMC_setNSteps( Int32 deviceHandle,
                            Int32 axis,
                            Bln32 backward,
                            Int32 n);

/** @brief get number of steps setted
 *
 *  Triggers a single step in desired direction.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  n             Number of steps
 *  @return               Result of function
 */
Int32 NCB_API AMC_getNSteps( Int32 deviceHandle,
                            Int32 axis,
                            Int32* n);


/** @brief Control continous movement forward
 *
 *  Controls continous movement in forward direction. If enabled a potential present movement
 *  in the opposite direction is stopped. The parameter "false" stops all movement of the axis regardless
 *  its direction.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  enable        Enables/ disables the movement
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlContinousFwd( Int32 deviceHandle,
                                       Int32 axis,
                                       Bln32* enable,
                                       Bln32 set );



/** @brief Control continous movement backward
 *
 *  Controls continous movement in backward direction. If enabled a potential present movement
 *  in the opposite direction is stopped. The parameter "false" stops all movement of the axis regardless
 *  its direction.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  enable        Enables/ disables the movement
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlContinousBkwd( Int32 deviceHandle,
                                        Int32 axis,
                                        Bln32* enable,
                                        Bln32 set );

/** @brief Control target position
 *
 *  Controls the target position for the approach function, see @ref AMC_controlMove .
 *  For linear type actors the position is defined in nm for goniometer an rotator type
 *  actors it is µ°.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  target        Target position in nm or µ° depending on actor type.
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlTargetPosition( Int32 deviceHandle,
                                         Int32 axis,
                                         Int32* target,
                                         Bln32 set );



/** @brief Reference status
 *
 *  Retrieves the status of the reference position. It may be valid or invalid.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  valid         Status of the reference position.
 *  @return               Result of function
 */
Int32 NCB_API AMC_getStatusReference( Int32 deviceHandle,
                                      Int32 axis,
                                      Bln32* valid );



/** @brief Moving status
 *
 *  Retrieves the status of the output stage. Moving means the actor is actively driven by the output stage
 *  either for approaching or continous/single stepping and the output is active. Pending means the output
 *  stage is driving but the output is deactivted i.e. by EOT or AMC_controlOutput.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  moving        Moving status, 0: Idle; 1: Moving; 2: Pending
 *  @return               Result of function
 */
Int32 NCB_API AMC_getStatusMoving( Int32 deviceHandle,
                                   Int32 axis,
                                   Int32* moving );


/** @brief Connected status
 *
 *  Retrieves the connected status. Indicates whether an actor is eletrically connected
 *  to the controller.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  connected     status
 *  @return               Result of function
 */
Int32 NCB_API AMC_getStatusConnected( Int32 deviceHandle,
                                      Int32 axis,
                                      Bln32* connected );




/** @brief Reference position
 *
 *  Retrieves the reference position. See @ref AMC_getStatusReference for 
 *  determining the validity.
 *  For linear type actors the position is defined in nm for goniometer an rotator type
 *  actors it is µ°.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  reference     Reference position in nm or µ° depending on actor type.
 *  @return               Result of function
 */
Int32 NCB_API AMC_getReferencePosition( Int32 deviceHandle,
                                        Int32 axis,
                                        Int32* reference );



/** @brief Actor position
 *
 *  Retrieves the current actor position.
 *  For linear type actors the position is defined in nm for goniometer an rotator type
 *  actors it is µ°.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  position      Actor position in nm or µ° depending on actor type.
 *  @return               Result of function
 */
Int32 NCB_API AMC_getPosition( Int32 deviceHandle,
                               Int32 axis,
                               Int32* position );


/** @brief Reference Auto Update
 *
 *  When set, every time the reference marking is hit the reference position
 *  will be updated. When this function is disabled, the reference marking will
 *  be considered only the first time and after then ignored.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  enable        enable/disable auto update
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return            Result of function
 */
Int32 NCB_API AMC_controlReferenceAutoUpdate( Int32 deviceHandle,
                                              Int32 axis,
                                              Bln32* enable,
                                              Bln32 set);


/** @brief Auto Reset
 *
 *  Resets the position for every time the reference position is detected.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  enable        enables/ disable functionality
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlAutoReset( Int32 deviceHandle,
                                    Int32 axis,
                                    Bln32* enable,
                                    Bln32 set);


/** @brief Target Range
 *
 *  Defines the range around the target position in which the flag target status
 *  become active.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  range         range in nm or µ° depending on actor type.
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlTargetRange( Int32 deviceHandle,
                                      Int32 axis,
                                      Int32* range,
                                      Bln32 set);

/** @brief Target status
 *
 *  Retrieves the target status. Indicates whether the actual position is within
 *  the target range.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  target        actual position is within target range
 *  @return               Result of function
 */
Int32 NCB_API AMC_getStatusTargetRange( Int32 deviceHandle,
                                        Int32 axis,
                                        Bln32* target );


/** @brief Firmware version
 *
 *  Retrieves the version of actual firmware
 *
 *  @param  deviceHandle  Handle of device
 *  @param  version       version number
 *  @param  size          size of the buffer
 *  @return               Result of function
 */
Int32 NCB_API AMC_getFirmwareVersion( Int32 deviceHandle,
                                      char* version,
                                      Int32 size);

/** @brief Fpga version
*
*  Retrieves the version of the fpga
*
*  @param  deviceHandle  Handle of device
*  @param  version       version number
*  @param  size           size of the buffer
*  @return               Result of function
*/
Int32 NCB_API AMC_getFpgaVersion(Int32 deviceHandle,
                                 char* version,
                                 Int32 size);

/** @brief Reboot system
*
*  reboot AMC device
*
*  @param  deviceHandle  Handle of device
*  @return               Result of function
*/
Int32 NCB_API AMC_rebootSystem(Int32 deviceHandle);


/** @brief Reset system
*
*  Device will reset to factory settings on next reboot
*
*  @param  deviceHandle  Handle of device
*  @return               Result of function
*/
Int32 NCB_API AMC_factoryReset(Int32 deviceHandle);

/** @brief mac address
*
*  Retrieves the mac address of the device
*
*  @param  deviceHandle  Handle of device
*  @param  mac           mac addr
*  @param  size          size of the buffer
*  @return               Result of function
*/
Int32 NCB_API AMC_getMacAddress(Int32 deviceHandle, char* mac, Int32 size);

/** @brief ip address
*
*  Retrieves the ip address of the device
*
*  @param  deviceHandle  Handle of device
*  @param  ip            ip addr
*  @param  size          size of the buffer
*  @return               Result of function
*/
Int32 NCB_API AMC_getIpAddress(Int32 deviceHandle, char* ip, Int32 size);

/** @brief device type
*
*  Retrieves the type of AMC based on eeprom configuration
*
*  @param  deviceHandle  Handle of device
*  @param  type          type
*  @param  size          size of the buffer
*  @return               Result of function
*/
Int32 NCB_API AMC_getDeviceType(Int32 deviceHandle, char* type, Int32 size);

/** @brief serial number
*
*  Retrieves the serial number of the device
*
*  @param  deviceHandle  Handle of device
*  @param  sn            output buffer
*  @param  size          size of the buffer
*  @return               Result of function
*/
Int32 NCB_API AMC_getSerialNumber(Int32 deviceHandle, char* sn, Int32 size);

/** @brief retrieve friendly name
*
*  Retrieves the friendly name given to this device
*
*  @param  deviceHandle  Handle of device
*  @param  name          output buffer
*  @param  size          size of the buffer
*  @return               Result of function
*/
Int32 NCB_API AMC_getDeviceName(Int32 deviceHandle, char* name, Int32 size);

/** @brief set friendly name
*
*  Set a friendly name to this device
*
*  @param  deviceHandle  Handle of device
*  @param  name          new friendly name as null-terminated string
*  @return               Result of function
*/
Int32 NCB_API AMC_setDeviceName(Int32 deviceHandle, const char* name);

/** @brief Device id
 *
 *  Sets and retrieve the device identifier. A set will save the
 *  id persistent in flash of the device.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  id            iud of the device.
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlDeviceId( Int32 deviceHandle,
                                   Int32* id,
                                   Bln32 set);


/** @brief EOT status forward
 *
 *  Retrieves the status of the end of travel (EOT) detection in forward direction.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  EotDetected   status
 *  @return               Result of function
 */
Int32 NCB_API AMC_getStatusEotFwd( Int32 deviceHandle,
                                   Int32 axis,
                                   Bln32* EotDetected);

/** @brief EOT status backward
 *
 *  Retrieves the status of the end of travel (EOT) detection in backward direction.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  EotDetected   status
 *  @return               Result of function
 */
Int32 NCB_API AMC_getStatusEotBkwd( Int32 deviceHandle,
                                    Int32 axis,
                                    Bln32* EotDetected);

/** @brief Output deactivate on EOT
 *
 *  Defines the behavior of the output on EOT. If enabled, the output of the axis will be deactivated on positive EOT detection.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  enable        true:  Output will be deactiveted on EOT.
                          false: Output stays active on EOT
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlEotOutputDeactive( Int32 deviceHandle,
                                            Int32 axis,
                                            Bln32* enable,
                                            Bln32 set);




/** @brief Fixed output voltage
 *
 *  Controls the DC level on the output.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  voltage       Output voltage in µV
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlFixOutputVoltage( Int32 deviceHandle,
                                           Int32 axis,
                                           Int32* voltage,
                                           Bln32 set);




/** @brief AQuadB input resolution
 *
 *  Controls the AQuadB input resolution for setpoint parameter.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  resolution    resolution in nm
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlAQuadBInResolution( Int32 deviceHandle,
                                             Int32 axis,
                                             Int32* resolution,
                                             Bln32 set);


/** @brief AQuadB output enable
 *
 *  Controls the AQuadB output for position indication.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  enable        true:  AQuadB output enabled
 *                        false: AQuadB output disabled
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlAQuadBOut( Int32 deviceHandle,
                                    Int32 axis,
                                    Bln32* enable,
                                    Bln32 set);

/** @brief AQuadB output resolution
 *
 *  Controls the AQuadB output resolution for position indication.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  resolution    resolution in nm
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlAQuadBOutResolution( Int32 deviceHandle,
                                              Int32 axis,
                                              Int32* resolution,
                                              Bln32 set);

/** @brief AQuadB output clock
 *
 *  Controls the clock for AQuadB output.
 *
 *  @param  deviceHandle  Handle of device
 *  @param  axis          Number of the axis to be configured
 *  @param  clock         Clock in multiples of 20ns. Minimum 2 (40ns), maximum 65535 (1,310700ms)
 *  @param  set           1: Send the supplied values to the controller
 *                        0: Ignore input; only retreive the results
 *  @return               Result of function
 */
Int32 NCB_API AMC_controlAQuadBOutClock( Int32 deviceHandle,
                                         Int32 axis,
                                         Int32* clock,
                                         Bln32 set);


/** @brief 
*
*  
*
*  @param  deviceHandle  Handle of device
*  @param  axis          Number of the axis to be configured
*  @param  actorname       
*/
Int32 NCB_API AMC_setActorParametersByName(Int32 deviceHandle,
                                           Int32 axis,
                                           const char* actorname);


/** @brief
*
*
*
*  @param  deviceHandle  Handle of device
*  @param  axis          Number of the axis to be configured
*  @param  actorname
*/
Int32 NCB_API AMC_setActorParameters(Int32 deviceHandle,
                                     Int32 axis,
                                     const char* actorName,
                                     Int32 actorType,
                                     Int32 fmax,
                                     Int32 amax,
                                     Bln32 sensor_dir,
                                     Bln32 actor_dir,
                                     Int32 pitchOfGrading,
                                     Int32 sensivity,
                                     Int32 stepsize
);

Int32 NCB_API AMC_setActorParametersByParamNameBoolean(Int32 deviceHandle,
                                                       Int32 axis,
                                                       char* paramName,
                                                       Bln32 paramValue);


Int32 NCB_API AMC_setActorParametersByParamName(Int32 deviceHandle,
                                                Int32 axis,
                                                char* paramName,
                                                Int32 paramValue);

/** @brief get actor parameters
*
*  @param  deviceHandle   Handle of device
*  @param  axis           Number of the axis to be configured
*  @param  paramName      name of the parameter to read
*  @param  paramValue     value of the parameter
*  @param  paramValueSize size of the buffer
*  @return                Result of function
*/
Int32 NCB_API AMC_getActorParametersByParamName(Int32 deviceHandle,
                                                Int32 axis,
                                                char* paramName,
                                                char* paramValue,
                                                Int32 paramValueSize);

/** @brief get list of all positioners
*
*  @param  deviceHandle   Handle of device
*  @param  list     value of the parameter
*  @param  listSize size of the buffer
*  @return                Result of function
*/
Int32 NCB_API AMC_getPositionersList(Int32 deviceHandle,
                                     char* list,
                                     Int32 listSize);
/** @brief
*
*
*
*  @param  deviceHandle  Handle of device
*  @param  axis          Number of the axis to be configured
*  @param  actorname
*/
Int32 NCB_API AMC_getActorParameters(Int32 deviceHandle,
                                     Int32 axis,
                                     char* actorname,
                                     size_t actorname_size,
                                     Int32* actorType,
                                     Int32* fmax,
                                     Int32* amax,
                                     Bln32* sensor_dir,
                                     Bln32* actor_dir,
                                     Int32* pitchOfGrading,
                                     Int32* sensivity,
                                     Int32* stepsize
);

/** @brief Set or get Realtime output mode
*
*  @param  deviceHandle  Handle of device
*  @param  mode          0 = AquadB (LVTTL), 1 = AquadB (LVDS)
*  @param  set           true: Send the supplied values to the controller
*                        false: Ignore input; only retreive the results
*  @return               Result of function
*/
Int32 NCB_API AMC_controlRtOutSignalMode(Int32 deviceHandle,
                                         Int32 *mode,
                                         Bln32 set);

/** @brief Set or get realtime input mode
*
*  @param  deviceHandle  Handle of device
*  @param  axis          Number of the axis to be configured
*  @param  mode          Realtime Input mode 0 = Aquadb (LVTTL),    1 = AquadB (LVDS),
*                                            8 = Stepper (LVTTL),   9 = Stepper(LVDS),
*                                            10 = Trigger (LVTTL), 11 = Trigger (LVDS),
*                                            15 = disable
*  @param  set           1: Send the supplied values to the controller
*                        0: Ignore input; only retreive the results
*  @return               Result of function
*/
Int32 NCB_API AMC_controlRealtimeInputMode(Int32 deviceHandle,
                                           Int32 axis,
                                           Int32* mode,
                                           Bln32 set);

/** @brief Set realtime input feedback loop mode
*
*  @param  deviceHandle  Handle of device
*  @param  axis          Number of the axis to be configured
*  @param  mode          Realtime Input mode 0 = open-loop, 1 = closed-loop
*  @param  set           1: Send the supplied values to the controller
*                        0: Ignore input; only retreive the results
*  @return               Result of function
*/
Int32 NCB_API AMC_controlRealtimeInputLoopMode(Int32 deviceHandle,
                                               Int32 axis,
                                               Int32* mode,
                                               Bln32 set);

/** @brief Set or get realtime input change per pulse in closed loop mode
*
*  @param  deviceHandle  Handle of device
*  @param  axis          Number of the axis to be configured
*  @param  change        Change per pule in nm
*  @param  set           1: Send the supplied values to the controller
*                        0: Ignore input; only retreive the results
*  @return               Result of function
*/
Int32 NCB_API AMC_controlRealtimeInputChangePerPulse(Int32 deviceHandle,
                                                     Int32 axis,
                                                     Int32* change,
                                                     Bln32 set);

/** @brief Set or get realtime input steps per pulse in open loop mode
*
*  @param  deviceHandle  Handle of device
*  @param  axis          Number of the axis to be configured
*  @param  steps         Number of steps per pulse
*  @param  set           1: Send the supplied values to the controller
*                        0: Ignore input; only retreive the results
*  @return               Result of function
*/
Int32 NCB_API AMC_controlRealtimeInputStepsPerPulse(Int32 deviceHandle,
                                                    Int32 axis,
                                                    Int32* steps,
                                                    Bln32 set);

/** @brief Enable or disable realtime input move in closed loop mode
*
*  @param  deviceHandle  Handle of device
*  @param  axis          Number of the axis to be configured
*  @param  enable        true = enable move, false = disable move
*  @param  set           1: Send the supplied values to the controller
*                        0: Ignore input; only retreive the results
*  @return               Result of function
*/
Int32 NCB_API AMC_controlRealtimeInputMove(Int32 deviceHandle,
                                           Int32 axis,
                                           Bln32* enable,
                                           Bln32 set);

#ifdef __cplusplus
}
#endif

#endif 


