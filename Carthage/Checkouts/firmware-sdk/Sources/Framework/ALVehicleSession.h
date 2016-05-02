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

NS_ASSUME_NONNULL_BEGIN

#pragma mark Vehicle Attributes

#pragma mark ELM and ODB Static Attributes

static NSString * const ALVehicleDeviceDescription = @"elm-device-description";
static NSString * const ALVehicleDeviceIdentifier = @"elm-device-identifier";
static NSString * const ALVehicleDeviceProtocol = @"elm-describe-protocol-number";
static NSString * const ALVehicleSupportedCommands = @"obd-supported-pids";
static NSString * const ALVehicleIdentificationNumber = @"obd-vehicle-identification-number";

#pragma mark OBD Streaming Attributes

static NSString * const ALVehicleStatus = @"obd-monitor-status";
static NSString * const ALVehicleFuelSystemStatus = @"obd-fuel-system-status";
static NSString * const ALVehicleEngineLoad = @"obd-engine-load";
static NSString * const ALVehicleCoolantTemp = @"obd-coolant-temp";
static NSString * const ALVehicleFuelPressure = @"obd-fuel-pressure";
static NSString * const ALVehicleManifoldAbsolutePressure = @"obd-intake-manifold-absolute-pressure";
static NSString * const ALVehicleEngineRPM = @"obd-engine-rpm";
static NSString * const ALVehicleSpeed = @"obd-vehicle-speed";
static NSString * const ALVehicleTimingAdvance = @"obd-timing-advance";
static NSString * const ALVehicleIntakeAirTemperature = @"obd-intake-air-temperature";
static NSString * const ALVehicleMassAirFlow = @"obd-mass-air-flow";
static NSString * const ALVehicleThrottlePosition = @"obd-throttle-position";
static NSString * const ALVehicleEngineRuntime = @"obd-engine-runtime";

#pragma mark Automatic Static Attributes
static NSString * const ALVehicleAdapterIdentifier = @"automatic-adapter-identifier";
static NSString * const ALVehicleAdapterFirmwareVersion = @"automatic-firmware-version";

#pragma mark Automatic Streaming Attributes
static NSString * const ALVehicleOdometer = @"automatic-trip-odometer";
static NSString * const ALVehicleEfficiency = @"automatic-efficiency";
static NSString * const ALVehicleEfficiencyHistory = @"automatic-efficiency-history";

#pragma mark -

@class ALAutomaticAdapter;
@class ALVehicleAttribute;
@protocol ALVehicleDelegate;

#pragma mark -

@class ALELM327Session;

/*! @abstract simple interface to query vehicle attributes using the ALEML327Protocol information
    via an autorized or mock ALELM327Session */
@interface ALVehicleSession : NSObject

/*! @abstract delegate which the session will report lifecycle state and data reads to */
@property (nonatomic, weak) id<ALVehicleDelegate> delegate;

/*! @abstract underlying, authorized or mock ALELM327Session to issue commands to */
@property (nonatomic, strong) ALELM327Session *elmSession;

/*! @abstract interval at which to report updated values for the list of streaming properties */
@property (nonatomic, assign) NSTimeInterval updateInterval;

/*! @abstract array of proprety names which are supported for the current session */
@property (nonatomic, readonly) NSArray<NSString *>*supportedAttributes;

/*! @abstract array of proprety names which have static values and should not be streamed */
@property (nonatomic, readonly) NSArray<NSString *>*staticAttributes;

/*! @abstract array of proprety names which have dynamic values and which may be streamed */
@property (nonatomic, readonly) NSArray<NSString *>*streamingAttributes;

/*! @abstract array of proprety names which are currently being streamed from the adapter */
@property (nonatomic, readonly) NSArray<NSString *>*currentlyStreamingAttributes;

/*! @abstract cache of ALVehicleAttributes which can be accessed by clients */
@property (nonatomic, readonly) NSDictionary<NSString *, ALVehicleAttribute *>*attributeCache;

#pragma mark -

/*! @param elmSession - 
    @param interval -
    @return an initllized ALVehicleSession with the underlying ALELM327Session and the pollingInterval provided */
+ (instancetype)vehicleWithELMSession:(ALELM327Session *)elmSession updateInterval:(NSTimeInterval) interval delegate:(id) delegate;

#pragma mark - Reading and Streaming Attributes

/*! @param attributeName - name of the attribute to read 
    @throws InvalidArgumentException if the attributeName is not recognized
    @abstract reqeuest read of an attribute from the ALELM327Session, looks up the attribute info using
    ALELM327Protocol, if the command is found it is sent to the vehicle via the ALELM327Session and the
    results are returned via the ALVehicleDelegate protocol methods */
- (void)readAttribute:(NSString *)attributeName;

/*! @param attributeName - name of the attribute to start streaming
    @throws InvalidArgumentException if the attributeName is not recognized
    @abstract reqeuest streaming of an attribute from the ALELM327Session at the sessions pollingInterval */
- (void)startStreamingAttribute:(NSString *)attributeName;

/*! @param attributeName - name of the attribute to stop streaming
    @throws InvalidArgumentException if the attributeName is not recognized
    @abstract stop streaming of an attribute from the ALELM327Session */
- (void)stopStreamingAttribute:(NSString *)attributeName;

@end

#pragma mark -

/*! @protocol ALVehicleDelegate - receive notifications when a vehicle is connected,
    supported attributes are determined, attributes are updated, and the adapter is
	disconnected */
@protocol ALVehicleDelegate <NSObject>
@optional

/*! @param vehicle - the vehicle session which was connected
    @param adapter - the underying ALAutomaticAdapter which the session is connected via 
    @abstract if the delegate implements this method it will be called when the session has
    connected to the adapter provided, typically when initilizing the object */
- (void)vehicle:(ALVehicleSession *)vehicle connectedViaAdapter:(ALAutomaticAdapter *)adapter;

/*! @param vehicle - the vehicle session which craeted the attribute
    @param newAttribute - newly created attribute for this session
    @abstract if the delegate implements this method it will be called after the client has sent a
    readAttribute: or startStreamingAttribute: message for the first time, after this call the 
    attribute will be avaliable in the attributeCache of the session */
- (void)vehicle:(ALVehicleSession *)vehicle createdAttribute:(ALVehicleAttribute *)newAttribute;

/*! @param vehicle - the vehicle session which has an updated attribute
    @param updateAttribute - updated attribute
    @abstract if the delegate implements this method it will be called after the client has sent once 
    after a readAttribute: message or every pollingInterval after a startStreamingAttribute: message */
- (void)vehicle:(ALVehicleSession *)vehicle updatedAttribute:(ALVehicleAttribute *)updateAttribute;

/*! @param vehicle - the vehicle session which was disconnected
    @param adapter - recently disconncted adapter
    @abstract if the delegate implements this method it will be called after the client has sent a
    readAttribute: or startStreamingAttribute: message for the first time */
- (void)vehicle:(ALVehicleSession *)vehicle disconnectedViaAdapter:(ALAutomaticAdapter *)adapter;

/*! @param vehicle - the vehicle session which experianced a connection error
    @param error - which caused the connection to fail
    @abstract the session should be considered inavlid after this message to the delegate */
- (void)vehicle:(ALVehicleSession *)vehicle connectionError:(NSError *)error;

@end

NS_ASSUME_NONNULL_END
