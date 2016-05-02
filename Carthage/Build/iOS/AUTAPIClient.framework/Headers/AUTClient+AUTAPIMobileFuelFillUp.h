//
//  AUTClient+AUTAPIMobileFuelFillUp.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 3/1/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTAPIMobileFuelFillUp;

@interface AUTClient (AUTAPIMobileFuelFillUp)

/// Returns a signal which sends a `AUTAPIMobileFuelFillUp` or `nil` and completes,
/// or errors.
- (RACSignal *)fetchFillUpWithObjectID:(NSString *)objectID forVehicleID:(NSString *)vehicleID;

/// Upload a newly-created fill-up.
///
/// Returns the `AUTAPIMobileFuelFillUp` from the server and completes, or errors.
- (RACSignal *)createFillUp:(AUTAPIMobileFuelFillUp *)fillUp forVehicleID:(NSString *)vehicleID;

/// Modify an existing fill-up.
///
/// Returns the updated `AUTAPIMobileFuelFillUp` from the server and completes, or errors.
- (RACSignal *)updateFillUp:(AUTAPIMobileFuelFillUp *)fillUp forVehicleID:(NSString *)vehicleID;

@end

NS_ASSUME_NONNULL_END
