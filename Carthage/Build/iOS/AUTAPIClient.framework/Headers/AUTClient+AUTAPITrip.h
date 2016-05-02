//
//  AUTClient+AUTAPITrip.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 25/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTAPITripPage;
@class AUTAPIVehicle;
@class RACSignal;
@class AUTAPILegacyVehicle;

@interface AUTClient (AUTAPITrip)

/// Fetches the trips for the currently authenticated user.
///
/// @return A signal that sends the a `AUTAPITripPage` containing the currently
///         authenticated user's trips and then completes, or sends an error
///         otherwise.
- (RACSignal *)fetchTripsForCurrentUser;

/// Fetches the trips for the specified vehicle for the currently authenticated
/// user.
///
/// @return A signal that sends the `AUTAPITripPage` containing the currently
///         authenticated user's trips for the specified vehicle and then
///         completes, or sends an error otherwise.
- (RACSignal *)fetchTripsForVehicle:(AUTAPIVehicle *)vehicle;

/// Fetches the trips for the specified vehicle ID for the currently
/// authenticated user.
///
/// @return A signal that sends the `AUTAPITripPage` containing the currently
///         authenticated user's trips for the specified vehicle ID and then
///         completes, or sends an error otherwise.
- (RACSignal *)fetchTripsForVehicleID:(NSString *)vehicleID;

@end

NS_ASSUME_NONNULL_END
