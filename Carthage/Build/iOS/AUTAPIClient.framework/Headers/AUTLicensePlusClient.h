//
//  AUTLicensePlusClient.h
//  AUTAPIClient
//
//  Created by James Lawton on 7/16/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

/// An API client to communicate with the License+ servers. These servers are
/// independent of the core API servers. The only dependency is that authentication
/// with the License+ servers requires a grant token from the core API.
@interface AUTLicensePlusClient : NSObject

- (instancetype)init NS_UNAVAILABLE;

/// Initialise the License+ client.
///
/// @param serverEnvironment Generally, this should match the server environment
///                          of the core API from which a grant token is issued.
/// @param clientID          The identifier of the app using this client.
///
/// @return Initialised, but unauthenticated, License+ client.
- (instancetype)initWithServer:(AUTServerEnvironment)serverEnvironment clientID:(NSString *)clientID NS_DESIGNATED_INITIALIZER;

/// The server set are we communicating with
@property (nonatomic, readonly) AUTServerEnvironment serverEnvironment;

@property (nonatomic, copy, readonly) NSString *clientID;

/// Authenticated session
@property (nonatomic, copy, nullable) NSString *sessionID;

/// A custom user agent string
@property (nonatomic, copy) NSString *customUserAgent;

@end

NS_ASSUME_NONNULL_END
