//
//  AUTClient+AUTAPIMobileVehicle.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 29/09/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTAPIVehicle;
@class AUTAPIMobileVehicle;
@class AUTAPIMobileVehiclePage;

@interface AUTClient (AUTAPIMobileVehicle)

/// Fetches the vehicle and its associated objects as synthesized from the
/// mobile aggregation API for the specified ID.
///
/// Returns a signal that sends an AUTAPIMobileVehicle as its next value and
/// completes, or else errors.
- (RACSignal *)fetchMobileVehicleWithID:(NSString *)vehicleID;

/// Fetches all of the current user's vehicles and their associated objects as
/// synthesized from the mobile aggregation API.
///
/// Returns a signal which will send an AUTAPIMobileVehiclePage then complete,
/// or else error.
///
/// The returned AUTAPIMobileVehiclePage can be passed to -fetchNextPageForPage:
/// to retrieve subsequent page if available.
- (RACSignal *)fetchMobileVehicles;

/// Updates the given vehicle and its associated objects as specified in the
/// given model object.
///
/// TODO: untested because the server return object is not implemented yet.
- (RACSignal *)updateMobileVehicle:(AUTAPIMobileVehicle *)vehicle;

@end

NS_ASSUME_NONNULL_END
