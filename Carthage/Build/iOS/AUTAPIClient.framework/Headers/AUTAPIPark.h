//
//  AUTAPIPark.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 25/09/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIObject.h>
#import <AUTAPIClient/AUTAPIDeletable.h>
#import <AUTAPIClient/AUTAPIParkPhoto.h>

@class AUTAPIAddress;
@class AUTAPILocation;
@class AUTAPIParkTimer;

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIPark : AUTAPIObject

- (instancetype)init NS_UNAVAILABLE;

@property (readonly, nonatomic, copy) NSDate *date;

@property (readonly, nonatomic, strong, nullable) AUTAPILocation *location;

@property (readonly, nonatomic, copy, nullable) NSString *hardwareDeviceID;

@property (readonly, nonatomic, strong, nullable) AUTAPIAddress *address;

@property (readonly, nonatomic, strong, nullable) AUTAPIDeletable<AUTAPIParkPhoto *> *photo;

@property (readonly, nonatomic, strong, nullable) AUTAPIParkTimer *timer;

@end

NS_ASSUME_NONNULL_END
