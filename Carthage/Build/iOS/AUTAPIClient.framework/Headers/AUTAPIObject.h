//
//  AUTAPIObject.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 23/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIObject : MTLModel <MTLJSONSerializing>

@property (readonly, nonatomic, copy) NSString *objectID;

@property (readonly, nonatomic, strong) NSURL *URL;

@end

NS_ASSUME_NONNULL_END
