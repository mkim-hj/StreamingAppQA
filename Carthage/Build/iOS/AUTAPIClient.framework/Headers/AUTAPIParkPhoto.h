//
//  AUTAPIParkPhoto.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 25/09/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIObject.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIParkPhoto : MTLModel <MTLJSONSerializing>

- (instancetype)init NS_UNAVAILABLE;

@property (readonly, nonatomic, copy) NSURL *URL;

@property (readonly, nonatomic, copy) NSDate *date;

@end

NS_ASSUME_NONNULL_END
