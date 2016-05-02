//
//  AUTClient+AUTTrips.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 12/13/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTAPITripsDayGroup)

/// Fetches the first page of the current user's trips history for the given
/// vehicle ID as synthesized by the view server.
///
/// Returns a signal which will send an AUTAPITripsDayGroupPage then complete,
/// or else error.
///
/// The returned AUTAPITripsDayGroupPage can be passed to -fetchNextPageForPage:
/// to retrieve subsequent page if available.
- (RACSignal *)fetchDayTripsForVehicleWithID:(NSString *)vehicleID;

/// Invokes fetchDayTripsForVehicleWithID:, with trips since the given date.
- (RACSignal *)fetchDayTripsForVehicleWithID:(NSString *)vehicleID since:(NSDate *)since;

/// Invokes fetchDayTripsForVehicleWithID:, with trips until the given date.
- (RACSignal *)fetchDayTripsForVehicleWithID:(NSString *)vehicleID until:(NSDate *)until;

@end

NS_ASSUME_NONNULL_END
