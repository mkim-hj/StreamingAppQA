//
//  AUTClient+AUTAPIFuelFillUpDetail.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 25/02/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTAPIFuelFillUpDetail)

/// Fetches the fuel fill-up detail object for the fill up with `fillUpID` and
/// vehicle with `vehicleID` as rendered by the view server.
///
/// Returns a signal which will send an AUTAPIFuelFillUpDetail then complete,
/// or else error.
- (RACSignal *)fetchFillUpDetailForFillUpID:(NSString *)fillUpID vehicleID:(NSString *)vehicleID;

@end

NS_ASSUME_NONNULL_END
