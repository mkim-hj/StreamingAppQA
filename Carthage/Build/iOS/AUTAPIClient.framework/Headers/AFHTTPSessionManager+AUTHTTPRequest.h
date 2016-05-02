//
//  AFHTTPRequestOperationManager+AUTHTTPRequest.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 27/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AFNetworking/AFNetworking.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTHTTPRequest;

@interface AFHTTPSessionManager (AUTHTTPRequest)

- (NSURLRequest *)aut_convertRequest:(AUTHTTPRequest *)request error:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
