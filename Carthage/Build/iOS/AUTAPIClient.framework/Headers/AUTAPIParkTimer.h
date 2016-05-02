//
//  AUTAPIParkTimer.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 25/09/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIParkTimer : MTLModel <MTLJSONSerializing>

- (instancetype)init NS_UNAVAILABLE;

@property (readonly, nonatomic, copy) NSDate *date;

@property (readonly, nonatomic, assign) NSTimeInterval duration;

@property (readonly, nonatomic, copy) NSDate *fireDate;

NS_ASSUME_NONNULL_END

@end
