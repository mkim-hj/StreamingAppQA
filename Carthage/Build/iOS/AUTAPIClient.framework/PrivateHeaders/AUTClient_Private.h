//
//  AUTClient_Private.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 5/7/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import AFNetworking;

#import "AFHTTPRequestSerializer+OAuth2.h"
#import "AFOAuth2SessionManager.h"

#import "AFHTTPSessionManager+AUTReactiveCocoaAdditions.h"
#import "AFHTTPSessionManager+AUTHTTPRequest.h"

#import "AUTClient.h"

NS_ASSUME_NONNULL_BEGIN

/// Returns the subdomain for the provided service.
extern NSString *AUTSubdomainForService(AUTAPIService service);

/// Returns the top-level domain for the provided environment.
extern NSString *AUTTopLevelDomainForServerEnvironment(AUTServerEnvironment environment);

/// Returns the URL host for the provided service and environment.
///
/// e.g. "api.automatic.com"
extern NSString *AUTURLHostForServiceInEnvironment(AUTAPIService, AUTServerEnvironment);

/// Returns the base URL for the provided service and environment.
///
/// e.g. https://api.automatic.com
extern NSURL *AUTBaseURLForServiceInEnvironment(AUTAPIService, AUTServerEnvironment);

extern NSString * const AUTClientHTTPHeaderUserAgent;

@interface AUTClient ()

/// Stores session managers keyed by an NSNumber-wrapped AUTAPIService.
@property (readonly, nonatomic, strong) NSDictionary<NSNumber *, __kindof AFHTTPSessionManager *> *sessionManagersByService;

@property (readonly, nonatomic, strong) AFOAuth2SessionManager *OAuth2Manager;

@property (readonly, nonatomic, strong) AFJSONRequestSerializer *requestSerializer;

// A request that will refresh the current auth token, using the refresh token.
//
// Multiple subscribers to this signal will share a single token refresh request,
// if they subscribe during the refresh attempt.
@property (readonly, nonatomic, strong) RACSignal *currentTokenRefreshRequest;

// The queue on which all operations related to auth token happen.
@property (readonly, nonatomic, strong) RACScheduler *tokenRefreshScheduler;

/// Returns the session manager used to perform the provided request, if there
/// is one.
- (nullable __kindof AFHTTPSessionManager *)sessionManagerForURL:(NSURL *)url;

@end

NS_ASSUME_NONNULL_END
