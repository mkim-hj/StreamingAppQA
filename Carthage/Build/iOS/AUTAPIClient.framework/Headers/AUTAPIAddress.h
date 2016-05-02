//
//  AUTAPIAddress.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 25/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIAddress : MTLModel <MTLJSONSerializing>

@property (readonly, nonatomic, copy, nullable) NSString *city;

@property (readonly, nonatomic, copy, nullable) NSString *country;

@property (readonly, nonatomic, copy, nullable) NSString *displayName;

@property (readonly, nonatomic, copy, nullable) NSString *name;

@property (readonly, nonatomic, copy, nullable) NSString *state;

@property (readonly, nonatomic, copy, nullable) NSString *streetName;

@property (readonly, nonatomic, copy, nullable) NSString *streetNumber;

@end

NS_ASSUME_NONNULL_END
