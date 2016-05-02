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

#import "ALVehicleSession.h"
#import "ALVehicleAttribute.h"
#import "ALAutomaticAdapter.h"
#import "ALELM327Session.h"
#import "ALVehicleCommands.h"

#pragma mark - Private

@interface ALVehicleSession ()
@property (nonatomic, strong) NSTimer *pollingTimer;
@property (nonatomic, strong) NSMutableSet *currentlyStreaming;
@property (nonatomic, strong) NSMutableDictionary *attributeCacheStorage;
@end

#pragma mark -

@implementation ALVehicleSession

+ (instancetype)vehicleWithELMSession:(ALELM327Session *)elmSession updateInterval:(NSTimeInterval)interval delegate:(id)delegate {
    return [[ALVehicleSession alloc] initWithELMSession:elmSession updateInterval:interval delegate:delegate];
}

- (instancetype) initWithELMSession:(ALELM327Session *)elmSession updateInterval:(NSTimeInterval)interval delegate:(id) delegate {
    if (self = [super init]) {
        self.currentlyStreaming = [NSMutableSet new];
        self.attributeCacheStorage = [NSMutableDictionary new];
        self.elmSession = elmSession;
        self.updateInterval = interval;
        self.delegate = delegate;

        if (elmSession.isConnected && [delegate respondsToSelector:@selector(vehicle:connectedViaAdapter:)]) {
            [delegate vehicle:self connectedViaAdapter:elmSession.adapter];
        }
    }

    return self;
}

- (void)setUpdateInterval:(NSTimeInterval)updateInterval {
    _updateInterval = updateInterval;

    if (self.pollingTimer.isValid) {
        [self.pollingTimer invalidate];
    }

    if (self.currentlyStreamingAttributes.count > 0) {
        self.pollingTimer = [NSTimer
            scheduledTimerWithTimeInterval:self.updateInterval
            target:self
            selector:@selector(pollingTimer:)
            userInfo:nil
            repeats:YES];
        [self.pollingTimer fire];
    }
}

- (NSArray *)supportedAttributes {
    return [self.staticAttributes arrayByAddingObjectsFromArray:self.streamingAttributes];
}

- (NSArray *)staticAttributes {
	return @[
        ALVehicleDeviceDescription,
        ALVehicleDeviceProtocol,
        ALVehicleAdapterIdentifier,
        ALVehicleAdapterFirmwareVersion,
        ALVehicleIdentificationNumber,
        ALVehicleSupportedCommands];
}

- (NSArray *)streamingAttributes {
	return @[
        ALVehicleStatus,
        ALVehicleFuelSystemStatus,
        ALVehicleEngineLoad,
        ALVehicleCoolantTemp,
        ALVehicleFuelPressure,
        ALVehicleManifoldAbsolutePressure,
        ALVehicleEngineRPM,
        ALVehicleSpeed,
        ALVehicleTimingAdvance,
        ALVehicleIntakeAirTemperature,
        ALVehicleMassAirFlow,
        ALVehicleThrottlePosition,
        ALVehicleEngineRuntime];
//        TODO outside of the Mock
//        ALVehicleOdometer,
//        ALVehicleEfficiency,
//        ALVehicleEfficiencyHistory];
}

- (NSArray *)currentlyStreamingAttributes {
    return self.currentlyStreaming.allObjects;
}

- (NSDictionary *)attributeCache {
    return [NSDictionary dictionaryWithDictionary:self.attributeCacheStorage];
}

- (void)readAttribute:(NSString *)attributeName {
    __block NSDictionary* commandInfo = [ALVehicleCommands ALVehicleCommandsInfo][attributeName];
    NSString* command = nil;

    if (commandInfo) {
        command = commandInfo[ALVehicleCommandStringKey];
    }

    if (command) {
        [self.elmSession sendLine:command onResponse:^(NSString* command, NSString* response, NSError* error) {
            if (error == nil) {
                ALVehicleAttribute* attribute = self.attributeCacheStorage[attributeName];

                // create the attribute if it's not already in cache storage
                if (!attribute) {
                    attribute = [ALVehicleAttribute attributeWithName:attributeName infoDictionary:commandInfo];
                    self.attributeCacheStorage[attributeName] = attribute;

                    if ([self.delegate respondsToSelector:@selector(vehicle:createdAttribute:)]) {
                        [self.delegate vehicle:self createdAttribute:attribute];
                    }
                }

                NSString* responseType = commandInfo[ALVehicleCommandResponseTypeKey];

                if ([responseType isEqualToString:@"number"]) {
                    NSArray *responseElements = [response componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
                    NSScanner *responseScanner = [NSScanner scannerWithString:responseElements.lastObject];
                    unsigned scanned = 0;
                    [responseScanner scanHexInt:&scanned];
                    [attribute recordValue:@(scanned)];
                }
                else { // if ([responseType isEqualToString:@"string"]) { @"array", @"packed"
                    [attribute recordValue:response];
                }

                // notify the delegate, if they're interested
                if ([self.delegate respondsToSelector:@selector(vehicle:updatedAttribute:)]) {
                    [self.delegate vehicle:self updatedAttribute:attribute];
                }
            }
            else {
                NSLog(@"Error: %@ reading attribute: %@", error, attributeName);
            }
        }];
    }
    else {
        //NSLog(@"<%@ %p WARNING! No command for property: %@>", self.class, self, attributeName);
        [[NSException
            exceptionWithName:NSInvalidArgumentException
            reason:[NSString stringWithFormat:@"attribute not found: %@", attributeName]
            userInfo:nil] raise];
    }
}

- (void) startStreamingAttribute:(NSString *)attributeName {
    [self.currentlyStreaming addObject:attributeName];

    if (self.currentlyStreaming.count > 0 && !self.pollingTimer.isValid) { // start the timer
        self.pollingTimer = [NSTimer
            scheduledTimerWithTimeInterval:self.updateInterval
            target:self
            selector:@selector(pollingTimer:)
            userInfo:nil
            repeats:YES];
    }
}

- (void) stopStreamingAttribute:(NSString *)attributeName {
    [self.currentlyStreaming removeObject:attributeName];

    if (self.currentlyStreaming.count == 0 && self.pollingTimer.isValid) {
        [self.pollingTimer invalidate];
        self.pollingTimer = nil;
    }
}

#pragma mark - NSTimers

- (void) pollingTimer:(NSTimer*) timer {
    for (NSString *attributeName in self.currentlyStreaming) {
        [self readAttribute:attributeName];
    }
}

@end
