//
//  AFHTTPRequestOperationManager+AUTHTTPRequest_Legacy.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 05/09/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import AFNetworking;

NS_ASSUME_NONNULL_BEGIN

@class AUTHTTPRequest;

@interface AFHTTPSessionManager (AUTHTTPRequest_Legacy)

- (NSURLRequest *)aut_convertLegacyRequest:(AUTHTTPRequest *)request error:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
