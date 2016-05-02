//
//  AUTLicensePlusProgram.h
//  AUTAPIClient
//
//  Created by James Lawton on 6/16/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>
#import <AUTAPIClient/AUTAPIPage.h>

NS_ASSUME_NONNULL_BEGIN

/// Represents a License+ program in which a user might enroll or be enrolled.
@interface AUTLicensePlusProgram : MTLModel <MTLJSONSerializing>

@property (nonatomic, strong, readonly) NSURL *URL;
@property (nonatomic, strong, readonly) NSURL *badgesURL;
@property (nonatomic, strong, readonly) NSURL *enrollmentURL;

@property (nonatomic, assign, readonly) NSTimeInterval duration;

@property (nonatomic, copy, readonly) NSString *localizedBrandingFooter;

@end

AUTPageSubclassInterface(AUTLicensePlusProgram)

NS_ASSUME_NONNULL_END
