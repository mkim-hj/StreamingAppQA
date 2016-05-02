//
//  AUTAPIUser.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 23/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIObject.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIUser : AUTAPIObject

@property (readonly, nonatomic, copy) NSString *emailAddress;

@property (readonly, nonatomic, copy) NSString *firstName;

@property (readonly, nonatomic, copy) NSString *lastName;

@property (readonly, nonatomic, copy) NSString *username;

@end

NS_ASSUME_NONNULL_END
