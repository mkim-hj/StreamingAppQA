//
//  AUTLicensePlusCoachInvitation.h
//  AUTAPIClient
//
//  Created by James Lawton on 7/8/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTLicensePlusCoachInvitation : MTLModel <MTLJSONSerializing>

@property (nonatomic, copy, readonly) NSString *identifier;

@property (nonatomic, copy, readonly) NSString *email;
@property (nonatomic, copy, readonly) NSDate *lastInvitedAt;

@property (nonatomic, strong, readonly) NSURL *reminderURL;

@end

NS_ASSUME_NONNULL_END
