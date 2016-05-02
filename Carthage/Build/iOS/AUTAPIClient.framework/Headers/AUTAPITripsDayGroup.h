//
//  AUTAPITripsDayGroup.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 12/13/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

#import <AUTAPIClient/AUTAPIPage.h>

@class AUTAPITripsDayGroupTrip;

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPITripsDayGroup : MTLModel <MTLJSONSerializing>

- (instancetype)init NS_UNAVAILABLE;

@property (readonly, nonatomic, copy) NSDate *date;

@property (readonly, nonatomic, copy) NSArray <AUTAPITripsDayGroupTrip *> *items;

@end

AUTPageSubclassInterface(AUTAPITripsDayGroup)

NS_ASSUME_NONNULL_END
