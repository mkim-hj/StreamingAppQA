/*****************************************************************************
 **
 **  Automatic Labs - CONFIDENTIAL
 **
 **  Unpublished Copyright (c) 2009-2016 AUTOMATIC LABS, All Rights Reserved.
 **
 **  NOTICE:
 **
 **  All information contained herein is, and remains the property of AUTOMATIC LABS.
 **  The intellectual and technical concepts contained herein are proprietary to AUTOMATIC LABS
 **  and may be covered by U.S. and Foreign Patents, patents in process, and are protected by trade
 **  secret or copyright law.
 **
 **  Dissemination of this information or reproduction of this material is strictly forbidden unless
 **  prior written permission is obtained from AUTOMATIC LABS.  Access to the source code contained
 **  herein is hereby forbidden to anyone except current AUTOMATIC LABS employees, managers or
 **  contractors who have executed Confidentiality and Non-disclosure agreements explicitly covering
 **  such access.
 **
 **  The copyright notice above does not evidence any actual or intended publication or disclosure of
 **  this source code, which includes information that is confidential and/or proprietary, and is a
 **  trade secret, of AUTOMATIC LABS. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC PERFORMANCE,
 **  OR PUBLIC DISPLAY OF OR THROUGH USE OF THIS SOURCE CODE WITHOUT THE EXPRESS WRITTEN CONSENT OF
 **  AUTOMATIC LABS IS STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND INTERNATIONAL TREATIES.
 **  THE RECEIPT OR POSSESSION OF THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY
 **  ANY RIGHTS TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL
 **  ANYTHING THAT IT MAY DESCRIBE, IN WHOLE OR IN PART.
 **
 ******************************************************************************/


#import <Foundation/Foundation.h>

/*

References:

    http://www.elmelectronics.com/ELM327/AT_Commands.pdf
    https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_01

    SAE J1979DA Rev JUN2014

*/

#pragma mark - ELM327Protocol.json Keys

/*

ELM327Protocol.json Format

"command-name" : {
    "group" : "ELM|OBD|etc.",
    "avaliable" : "1.0", // version number
    "description" : "description of this command",
    "command" : "<command-string>",
    "supported" : true | false,
    "streaming" : true | false,
    "argument-type" : "null|string|number|array|data|packed",
    "argument-format" : "<printf-format-string>",
    "response-type" : "null|string|number|array|data|packed",
    "response-size" : 4, // number of bytes in the response
    "response-format" : "<scanf-format-string>",
    "response-unit" : "<unit-name>",
    "response-limits" : [0,1], // always signed doubles: [min, max], used to scale views
    "response-bits" : ["bit-1","bit-2","bit-3"],
    "response-packing" : [] // unpacking instructions for the response TBD
}

Groups:

    ELM - messages which relate to the ELM327 connection
    OBD - messages which relate to the OBD-II bus
    AL - messages specific to the Automatic adapter

Argument and Response Types:

    null - implicit argument type
    string - string of chars (ASCII?)
    number - integer or floating point value
    array - array of numbers
    data - bit array, encoded as HEX ASCII chars
    packed - packed bytes, encoded as HEX ASCII chars

*/

static NSString * const ALVehicleCommandGroupKey = @"group";
static NSString * const ALVehicleCommandAvailableKey = @"available";
static NSString * const ALVehicleCommandDescriptionKey = @"description";
static NSString * const ALVehicleCommandStringKey = @"command";
static NSString * const ALVehicleCommandArgumentTypeKey = @"argument-type";
static NSString * const ALVehicleCommandArgumentFormatKey = @"argument-format";
static NSString * const ALVehicleCommandResponseTypeKey = @"response-type";
static NSString * const ALVehicleCommandResponseSizeKey = @"response-size";
static NSString * const ALVehicleCommandResponseFormatKey = @"response-format";
static NSString * const ALVehicleCommandResponseArgumentsKey = @"response-arguments";
static NSString * const ALVehicleCommandResponseBitsKey = @"response-bits";
static NSString * const ALVehicleCommandResponseUnitsKey = @"response-units";
static NSString * const ALVehicleCommandResponseLimitsKey = @"response-limits";
static NSString * const ALVehicleCommandResponsePackingKey = @"response-packing";

static NSString * const ALVehicleCommandResponseTypeString = @"string";
static NSString * const ALVehicleCommandResponseTypeNumber = @"number";
static NSString * const ALVehicleCommandResponseTypeArray = @"array";
static NSString * const ALVehicleCommandResponseTypeBits = @"bits";
static NSString * const ALVehicleCommandResponseTypePacked = @"packed";

#pragma mark - General ELM Commands

static NSString * const ALELMRepeatCommand = @"elm-repeat-command";
static NSString * const ALELMDeviceDescriptionCommand = @"elm-device-description";
static NSString * const ALELMDeviceIdentifierCommand = @"elm-device-identifier";
static NSString * const ALELMSetDeviceIdentifierCommand = @"elm-set-device-identifier";
static NSString * const ALELMSetBaudRateDivisorCommand = @"elm-set-baud-rate-divisor";
static NSString * const ALELMSetDefaultsCommand = @"elm-set-defaults";
static NSString * const ALELMEchoOnCommand = @"elm-echo-on";
static NSString * const ALELMEchoOffCommand = @"elm-echo-off";
static NSString * const ALELMForgetEvents = @"elm-forget-events";
static NSString * const ALELMPrintIdentifier = @"elm-print-id";
static NSString * const ALELMLinefeedsOff = @"elm-linefeeds-off";
static NSString * const ALELMLinefeedsOn = @"elm-linefeeds-on";
static NSString * const ALELMLowPowerMode = @"elm-low-power-mode";
static NSString * const ALELMMemoryOff = @"elm-memory-off";
static NSString * const ALELMMemoryOn = @"elm-memory-on";
static NSString * const ALELMReadData = @"elm-read-data";
static NSString * const ALELMStoreData = @"elm-store-data";
static NSString * const ALELMWarmStart = @"elm-warm-start";
static NSString * const ALELResetAll = @"elm-reset-all";

#pragma mark - OBD-II Commands

static NSString * const ALOBDSupportedPids = @"obd-supported-pids";
static NSString * const ALOBDMonitorStatus = @"obd-monitor-status";
static NSString * const ALOBDFreezeDTC = @"obd-freeze-dtc";
static NSString * const ALOBDIntakeManifoldPressure = @"obd-intake-manifold-pressure";
static NSString * const ALOBDEngineRPM = @"obd-engine-rpm";
static NSString * const ALOBDVehicleSpeed = @"obd-vehicle-speed";
static NSString * const ALOBDEngineCoolantTemperature = @"obd-engine-coolant-temperature";

@interface ALVehicleCommands : NSObject

/*! @abstract returns the path to the ALVehicleComands.json file in this bundle */
+ (NSString *)ALVehicleCommandsPath;

/*! @abstract returns the compact JSON contents of ALVehicleCommands.json */
+ (NSString *)ALVehicleCommandsJSON;

/*! @abstract returns a dictionary of command info from ALVehicleCommands.json */
+ (NSDictionary *)ALVehicleCommandsInfo;

/*! @abstract maps the ELM command string to the command name, which is the key to the info dictionary  */
+ (NSDictionary *)ALVehicleCommandsMap;

@end
