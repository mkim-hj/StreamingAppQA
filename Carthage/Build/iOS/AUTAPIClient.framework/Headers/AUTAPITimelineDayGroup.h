//
//  AUTAPITimelineDayGroup.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 12/4/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIPage.h>

@import Mantle;

@class AUTAPITimelineDayGroupItem;

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPITimelineDayGroup : MTLModel <MTLJSONSerializing>

- (instancetype)init NS_UNAVAILABLE;

@property (readonly, nonatomic, copy) NSDate *date;

@property (readonly, nonatomic, copy) NSArray <AUTAPITimelineDayGroupItem *> *items;

@end

AUTPageSubclassInterface(AUTAPITimelineDayGroup)

NS_ASSUME_NONNULL_END
