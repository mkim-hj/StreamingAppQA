//
//  AUTClient+AUTAPIVehicle.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 6/29/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

@import ReactiveCocoa;

@class AUTAPIVehiclePage;

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTAPIVehicle)

/// Fetches the vehicle with the specified ID
///
/// Returns a signal that sends an AUTAPIVehicle as its next value and completes,
/// or else errors.
- (RACSignal *)fetchVehicleWithID:(NSString *)vehicleID;

/// Fetches all of the vehicles for the current user.
///
/// Returns a signal which will send an AUTAPIVehiclePage then complete, or else
/// error.
/// The returned AUTAPIVehiclePage can be passed to -fetchNextPageForPage: to
/// retrieve subsequent page if available.
- (RACSignal *)fetchVehicles;

@end

NS_ASSUME_NONNULL_END
