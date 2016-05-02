//
//  AUTLicensePlusPerson.h
//  AUTAPIClient
//
//  Created by James Lawton on 6/16/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTLicensePlusPerson : MTLModel <MTLJSONSerializing>

@property (nonatomic, copy, readonly) NSString *identifier;
@property (nonatomic, strong, readonly) NSURL *URL;

@property (nonatomic, copy, readonly) NSString *email;
@property (nonatomic, copy, readonly) NSString *firstName;
@property (nonatomic, copy, readonly) NSString *lastName;
@property (nonatomic, copy, readonly) NSString *phoneNumber;

@end

NS_ASSUME_NONNULL_END
