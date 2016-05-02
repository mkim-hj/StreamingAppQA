//
//  AUTClient+AUTAPILegacyVehicle.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 02/04/15.
//  Copyright (c) 2015 Automatic. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

@class AUTAPILegacyVehicle;

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTAPILegacyVehicle)

/// Returns a signal that will associate a vehicle vin with the given
/// adapter ID, or else error.
///
/// @param vin Vehicle VIN
/// @param deviceID Hardware device ID as reported by firmware
/// @param supportedPids NSArray of pids as reported by firmware
- (RACSignal *)registerLegacyVehicleWithVIN:(NSString *)vin deviceID:(NSString *)deviceID supportedPids:(nullable NSArray *)pids;

/// Returns a signal that will return vehicle information for
/// a previously registered vehicle.
///
/// @param vin Vehicle VIN
- (RACSignal *)fetchLegacyVehicleWithVIN:(NSString *)vin;

/// Returns a signal that will unregister a given vehicle
/// previously associated with user.
///
/// @param vin Vehicle VIN
- (RACSignal *)unregisterLegacyVehicleWithVIN:(NSString *)vin;

/// Returns a signal that sends the AUTAPIMobileVehicle corresponding to the specified
/// AUTAPILegacyVehicle as its next value, or completes with no next if one was
/// not found.
- (RACSignal *)vehicleForLegacyVehicle:(AUTAPILegacyVehicle *)legacyVehicle;

@end

NS_ASSUME_NONNULL_END
