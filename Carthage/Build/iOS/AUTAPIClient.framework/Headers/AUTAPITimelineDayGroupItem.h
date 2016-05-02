//
//  AUTAPITimelineDayGroupItem.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 12/4/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPITimelineDayGroupItem : MTLModel <MTLJSONSerializing>

- (instancetype)init NS_UNAVAILABLE;

@property (readonly, nonatomic, copy) NSString *title;

@property (readonly, nonatomic, copy) NSString *viewPath;

@property (readonly, nonatomic, copy) NSDate *date;

@end

NS_ASSUME_NONNULL_END
