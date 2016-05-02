//
//  AUTClient+AUTAPIMobileFuelStation.h
//  AUTClient
//
//  Created by Westin Newell on 2/25/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import CoreLocation;

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTAPIMobileFuelStation)

/// Fetches the nearest fuel station to a given location from the mobile API.
///
/// Returns a signal that sends an AUTAPIMobileFuelStation as its next value and
/// completes, or else errors.
///
/// If no stations were returned in the results, errors in the
/// AUTClientErrorDomain domain with the AUTClientErrorNoNearestStation code.
- (RACSignal *)fetchNearestMobileFuelStationToCoordinate:(CLLocationCoordinate2D)coordinate;

/// Fetches the nearest fuel stations to a given location from the mobile API.
///
/// Returns a signal that sends an AUTAPIMobileFuelStationPage as its next value and
/// completes, or else errors.
- (RACSignal *)fetchNearestMobileFuelStationsToCoordinate:(CLLocationCoordinate2D)coordinate;

/// Fetches an array of fuel stations within the given map rectangle.
///
/// Returns a signal that sends an array of AUTAPIMobileFuelStation as its next
/// value and completes, or else errors.
- (RACSignal *)fetchFuelStationsInRectangleWithNortheast:(CLLocationCoordinate2D)northeast southwest:(CLLocationCoordinate2D)southwest;

@end

NS_ASSUME_NONNULL_END
