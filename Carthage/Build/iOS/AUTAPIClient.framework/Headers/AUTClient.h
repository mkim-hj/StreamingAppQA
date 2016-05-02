//
//  AUTClient.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 24/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;
@import Foundation;

NS_ASSUME_NONNULL_BEGIN

@class AFOAuthCredential;
@class AUTAPIPage;
@class AUTHTTPRequest;

typedef NS_OPTIONS(NSUInteger, AUTClientAuthorizationScopes) {
    AUTClientAuthorizationScopesPublic          = 1 << 0,
    AUTClientAuthorizationScopesUserProfile     = 1 << 1,
    AUTClientAuthorizationScopesUserFollow      = 1 << 2,
    AUTClientAuthorizationScopesLocation        = 1 << 3,
    AUTClientAuthorizationScopesCurrentLocation = 1 << 4,
    AUTClientAuthorizationScopesVehicleProfile  = 1 << 5,
    AUTClientAuthorizationScopesVehicleEvents   = 1 << 6,
    AUTClientAuthorizationScopesVehicleVin      = 1 << 7,
    AUTClientAuthorizationScopesTrip            = 1 << 8,
    AUTClientAuthorizationScopesBehavior        = 1 << 9,
    AUTClientAuthorizationScopesAutomatic       = 1 << 10,
    AUTClientAuthorizationScopesAdapterBasic    = 1 << 11
};

typedef NS_ENUM(NSInteger, AUTServerEnvironment) {
    /// Points at https://*.automatic.com hosts
    AUTServerEnvironmentProduction = 0,

    /// Points at https://*.automatic.co hosts
    AUTServerEnvironmentStaging,
};

/// A service that API requests can be made against.
typedef NS_ENUM(NSInteger, AUTAPIService) {
    /// The service responsible for pure REST APIs, aka "Newton".
    AUTAPIServiceNewton,

    /// The service responsible for OAuth 2 authentication.
    AUTAPIServiceAccounts,

    /// The service responsible for aggregating entities from "Newton" to reduce
    /// the number of calls mobile clients have to make.
    AUTAPIServiceMobile,

    /// The service responsible for aggregating entities from "Newton" into
    /// semi-opaque views for presentation to users.
    AUTAPIServiceView,
    
    /// The service responsible for creating records. Will eventually merge with
    /// AUTAPIServiceMobile maybe.
    AUTAPIServiceCooper,
};

@interface AUTClient : NSObject

/// A custom User-Agent HTTP header value used for overriding the
/// default one set by AFNetworking.
@property (readwrite, nonatomic, copy) NSString *customUserAgent;

/// The OAuth2 credential used to authenticate the receiver. This credential is
/// not persisted by the client, use `+[VALValet aut_storeCredential]` for that.
///
/// This property is KVO-compliant. However, it is not guaranteed to be changed
/// from the main thread.
@property (readwrite, nonatomic, strong, nullable) AFOAuthCredential *credential;

/// The client ID used by the receiver.
@property (readonly, nonatomic, copy) NSString *clientID;

/// The client ID used by the receiver.
@property (readonly, nonatomic, copy) NSString *clientSecret;

/// The server environment used by the receiver. Defaults to
/// AUTServerEnvironmentProduction.
@property (nonatomic, readonly) AUTServerEnvironment serverEnvironment;

- (instancetype)init NS_UNAVAILABLE;

/// Initializes the receiver to point to the production servers with a specific
/// client ID and secret.
- (instancetype)initWithClientID:(NSString *)clientID clientSecret:(NSString *)clientSecret;

/// Initializes the receiver with a new client ID and secret.
///
/// @param  server       The server set to access.
/// @param  clientID     The client ID to use. This argument must not be nil.
/// @param  clientSecret The client ID to use. This argument must not be nil.
/// @return A new `AUTClient` instance.
- (instancetype)initWithServer:(AUTServerEnvironment)serverEnvironment clientID:(NSString *)clientID clientSecret:(NSString *)clientSecret NS_DESIGNATED_INITIALIZER;

/// Converts `AUTClientAuthorizationScopes` to an array of `NSString`s that are
/// understood by the server.
+ (NSArray *)serializeScopes:(AUTClientAuthorizationScopes)scopes;

/// Attempts to authenticate the receiver with the server.
///
/// @param username The username to authenticate with. This argument must not be
///                 `nil`.
/// @param password The password to authenticate with. This argument must not be
///                 `nil`.
/// @param scopes   The scopes the client should request.
///
/// @return A signal that sends the credential and completes when the user was
///         successfully authenticated with the server, or sends an error
///         otherwise.
- (RACSignal *)authenticateWithUsername:(NSString *)username password:(NSString *)password scopes:(AUTClientAuthorizationScopes)scopes;

/// Enqueues an `AUTHTTPRequest` request for a service.
///
/// @param request The `AUTHTTPRequest` to perform. This argument must not be nil.
///
/// @param service The service that the request should made against.
///
/// @return A signal that will send the results of the request, or send an error
///         otherwise.
- (RACSignal *)enqueueRequest:(AUTHTTPRequest *)request forService:(AUTAPIService)service;

/// A hot signal that sends all errors that the receiver receives when making
/// requests.
///
/// Does not error or complete.
@property (readonly, nonatomic) RACSignal *errors;

/// Returns a cold signal that sends next with a boolean value representing
/// whether this client is authenticated upon subscription, and subsequently
/// whenever the authentication status changes.
- (RACSignal *)isAuthenticated;

/// Issue a grant token to hand off to another client.
- (RACSignal *)issueGrantTokenForClientID:(NSString *)clientID scopes:(AUTClientAuthorizationScopes)scopes;

/// A hot signal that sends updates to the reachability of the domain used for
/// API requests as NSNumber<AFNetworkReachabilityStatus>.
///
/// Does not error or complete.
///
/// Filters AFNetworkReachabilityStatusUnknown, sending next only when the
/// reachability status has been determined. If the reachability status has
/// already been determined, it is sent immediately upon subscription.
@property (readonly, nonatomic) RACSignal *reachabilityStatus;

@end

NS_ASSUME_NONNULL_END
