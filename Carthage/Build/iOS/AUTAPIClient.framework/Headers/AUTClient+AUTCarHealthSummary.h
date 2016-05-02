//
//  AUTClient+AUTCarHealthSummary.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 19/02/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTCarHealthSummary)

/// Fetches the items on the car health summary page as rendered by the
/// view server for the vehicle with `vehicleID`.
///
/// Returns a signal which will send an AUTAPICarHealthSummaryItemPage then
/// complete, or else error.
///
/// The returned AUTAPICarHealthSummaryItemPage can be passed to
/// -fetchNextPageForPage: to retrieve subsequent pages if available.
- (RACSignal *)fetchCarHealthSummaryForVehicleWithID:(NSString *)vehicleID;

@end

NS_ASSUME_NONNULL_END
