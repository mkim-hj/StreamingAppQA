//
//  AUTLicensePlusBadgeState.h
//  AUTAPIClient
//
//  Created by James Lawton on 6/16/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>
#import <AUTAPIClient/AUTAPIPage.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTLicensePlusBadgeState : MTLModel <MTLJSONSerializing>

@property (readonly, copy, nonatomic) NSString *identifier;

@property (readonly, nonatomic, assign) double progress; // [0,1]
@property (readonly, nonatomic, assign, getter=isCompleted) BOOL completed;
@property (readonly, nonatomic, strong) NSURL *userURL;

@end

AUTPageSubclassInterface(AUTLicensePlusBadgeState)

NS_ASSUME_NONNULL_END
