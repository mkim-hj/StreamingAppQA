//
//  AUTClient+AUTAPITripDetails.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 15/01/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTAPITripDetails)

/// Returns a signal which sends an AUTAPITripDetails instance corresponding to
/// the given `tripID` and `vehicleID` and completes. Or errors if a network or
/// deserialization error occurs.
- (RACSignal *)fetchTripWithID:(NSString *)tripID vehicleID:(NSString *)vehicleID;

@end

NS_ASSUME_NONNULL_END
