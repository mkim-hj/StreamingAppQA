//
//  AUTClient+AUTAPIFuelFillUps.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 23/02/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTAPIFuelFillUps)

/// Fetches the fuel fill-up state object for the vehicle with `vehicleID`
/// rendered by the view server.
///
/// Returns a signal which will send an AUTAPIFuelFillUpState then complete,
/// or else error.
///
/// The `itemsList` property of the returned AUTAPIFuelFillUpState contains the
/// first page of the fill-ups for the vehicle. -fetchNextPageForPage: to
/// retrieve subsequent pages if available.
- (RACSignal *)fetchFillUpStateForVehicleWithID:(NSString *)vehicleID;

@end

NS_ASSUME_NONNULL_END
