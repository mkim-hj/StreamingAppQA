//
//  AUTAPILegacyResponse.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 7/29/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

/// An abstract response that all legacy API responses conform to.
@interface AUTAPILegacyResponse : MTLModel <MTLJSONSerializing>

/// Whether the request was considered by the server to be successful.
@property (readonly, nonatomic) BOOL success;

/// The HTTP status code from the server.
@property (readonly, nonatomic) NSInteger code;

@end

NS_ASSUME_NONNULL_END
