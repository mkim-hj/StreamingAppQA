//
//  AUTClient+AUTDefaultLegacyBodyMatcher.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 7/17/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

@interface AUTClient (AUTDefaultLegacyBodyMatcher)

+ (BOOL(^)(NSData *, NSDictionary *))defaultLegacyBodyMatcher;
+ (BOOL(^)(NSData *, NSDictionary *))defaultLegacyBodyMatcherSubsetOnly:(BOOL)subsetOnly;

@end
