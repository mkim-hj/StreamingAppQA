/*****************************************************************************
 **
 **  Automatic Labs - CONFIDENTIAL
 **
 **  Unpublished Copyright (c) 2009-2016 AUTOMATIC LABS, All Rights Reserved.
 **
 **  NOTICE:
 **
 **  All information contained herein is, and remains the property of AUTOMATIC LABS.
 **  The intellectual and technical concepts contained herein are proprietary to AUTOMATIC LABS
 **  and may be covered by U.S. and Foreign Patents, patents in process, and are protected by trade
 **  secret or copyright law.
 **
 **  Dissemination of this information or reproduction of this material is strictly forbidden unless
 **  prior written permission is obtained from AUTOMATIC LABS.  Access to the source code contained
 **  herein is hereby forbidden to anyone except current AUTOMATIC LABS employees, managers or
 **  contractors who have executed Confidentiality and Non-disclosure agreements explicitly covering
 **  such access.
 **
 **  The copyright notice above does not evidence any actual or intended publication or disclosure of
 **  this source code, which includes information that is confidential and/or proprietary, and is a
 **  trade secret, of AUTOMATIC LABS. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC PERFORMANCE,
 **  OR PUBLIC DISPLAY OF OR THROUGH USE OF THIS SOURCE CODE WITHOUT THE EXPRESS WRITTEN CONSENT OF
 **  AUTOMATIC LABS IS STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND INTERNATIONAL TREATIES.
 **  THE RECEIPT OR POSSESSION OF THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY
 **  ANY RIGHTS TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL
 **  ANYTHING THAT IT MAY DESCRIBE, IN WHOLE OR IN PART.
 **
 ******************************************************************************/

#import "ALAuthorizationJob.h"
#import "ALELM327Session.h"
#import "NSData+AL.h"
#import "NSError+AL.h"
#import "NSString+AL.h"

#ifndef AL_LOG_AUTHORIZATIONJOB
#if 0
#define AL_LOG_AUTHORIZATIONJOB AL_LOG
#else
#define AL_LOG_AUTHORIZATIONJOB(...)
#endif
#endif

static NSMutableDictionary *sJobsByAdapterID;

#pragma mark -

@interface _ALAuthorizationJobObserver : NSObject

+ (instancetype)observer:(ALAuthorizationJobOnWaitVerdict)inOnVerdict onThread:(NSThread *)inHomeThread;

- (void)gotVerdict:(NSError *)inError withStatus:(ALAuthStatus)inStatus;

@end

#pragma mark -

@interface ALAuthorizationJob ()

@property (nonatomic, setter=_setStatus:) ALAuthStatus status;
@property (nonatomic, setter=_setError:) NSError *error;

@end

#pragma mark -

@implementation ALAuthorizationJob {
  @private
    NSString *_acctAccessReqCode;
    NSURLSession *_urlSession;
    NSURLSessionTask *_dataTask;
    ALAuthorizationJobOnStatus _onStatus;
    ALAuthorizationJobOnAuthorized _onAuthorized;
    NSMutableArray *_waiters;
}

#pragma mark - Lifecycle

- (void)dealloc {
    [_urlSession invalidateAndCancel];
    _urlSession = nil;
    if (_adapterID) {
        [sJobsByAdapterID removeObjectForKey:_adapterID];
        if ([sJobsByAdapterID count] == 0) {
            sJobsByAdapterID = nil;
        }
    }
    if (_onStatus) {
        [self _setError:[NSError ALError:kALError_Aborted userInfo:nil]];
    }
}

+ (instancetype)jobForURL:(NSURL *)inAccountAuthRedirectURL {
    return [[self alloc] initWithURL:inAccountAuthRedirectURL];
}

+ (instancetype)jobForAdapter:(NSString *)inAdapterID {
    return [[self alloc] initWithAdapterID:inAdapterID];
}

- (instancetype)initWithURL:(NSURL *)inAccountAuthRedirectURL {
    NSString *scheme = [inAccountAuthRedirectURL scheme];
    if (![scheme hasPrefix:@"automatic-"]) {
        // Not our scheme
        AL_LOG_AUTHORIZATIONJOB(@"IGNORING URL with scheme `%@`", scheme);
        return nil;
    }

    AL_LOG_AUTHORIZATIONJOB(@"GOT AUTHORIZATION URL %@", inAccountAuthRedirectURL);
    NSArray *splitScheme = [[scheme lowercaseString] componentsSeparatedByString:@"automatic-"];
    NSString *clientID = [splitScheme lastObject];
    if ([clientID length] == 0) {
        AL_LOG_AUTHORIZATIONJOB(@"MISSING CLIENT ID in scheme `%@`", scheme);
        return nil;
    }

    NSURLComponents *urlComponents = [[NSURLComponents alloc] initWithURL:inAccountAuthRedirectURL resolvingAgainstBaseURL:NO];
    NSArray *queryItems = [urlComponents queryItems];
    NSDictionary *queryDict = [NSDictionary dictionaryWithObjects:[queryItems valueForKey:@"value"] forKeys:[queryItems valueForKey:@"name"]];
    NSString *stateEscapedBase64 = queryDict[@"state"];
    NSString *stateBase64 = [stateEscapedBase64 stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSData *stateData = [[NSData alloc] initWithBase64EncodedString:stateBase64 options:0];
    NSDictionary *stateDict;
    @try {
        stateDict = [NSKeyedUnarchiver unarchiveObjectWithData:stateData];
    }
    @catch (NSException *inException) {
        // Invalid state archive
        AL_LOG_AUTHORIZATIONJOB(@"INVALID STATE argument `%@` raised %@", stateEscapedBase64, inException);
        return nil;
    }

    if (![stateDict isKindOfClass:[NSDictionary class]]) {
        // Invalid state object
        AL_LOG_AUTHORIZATIONJOB(@"INVALID STATE object %@", stateDict);
        return nil;
    }

    NSString *stateClass = stateDict[@"class"];
    if (![stateClass isEqual:@"ALAuthorization"]) {
        // Invalid state object
        AL_LOG_AUTHORIZATIONJOB(@"Expected state class ALAuthorization.  Got `%@`", stateClass);
        return nil;
    }

    NSString *deviceID = stateDict[@"deviceID"];
    if ([deviceID length] == 0) {
        AL_LOG_AUTHORIZATIONJOB(@"MISSING DEVICE ID in %@", stateDict);
        return nil;
    }

    NSString *acctAccessReqCode = queryDict[@"code"];
    if ([acctAccessReqCode length] == 0) {
        AL_LOG_AUTHORIZATIONJOB(@"MISSING ACCESS CODE in query params %@", queryDict);
        return nil;
    }

    if ((self = [super init])) {
        _clientID = clientID;
        _adapterID = deviceID;
        _acctAccessReqCode = acctAccessReqCode;
        _status = kALAuthStatus_None;
    }

    return self;
}

- (instancetype)initWithAdapterID:(NSString *)inAdapterID {
    if ((self = [super init])) {
        _adapterID = inAdapterID;
        _status = kALAuthStatus_None;
    }

    return self;
}

#pragma mark - Property accessors

- (void)_setError:(NSError *)inError {
    if (!inError) {
        return;
    }
    _error = inError;

    _dataTask = nil;
    _urlSession = nil;

    if (_adapterID) {
        [sJobsByAdapterID removeObjectForKey:_adapterID];
        if ([sJobsByAdapterID count] == 0) {
            sJobsByAdapterID = nil;
        }
    }
    _onStatus(_status, _error);
    [self _onStatus:nil];

    for (_ALAuthorizationJobObserver *eachObserver in _waiters) {
        [eachObserver gotVerdict:_error withStatus:_status];
    }
    _waiters = nil;
}

- (void)_setStatus:(ALAuthStatus)inStatus {
    if (_status == inStatus) {
        return;
    }

    _status = inStatus;
    _onStatus(_status, nil);
}

#pragma mark - ALAuthorizationJob methods

- (void)runWithOAuthToken:(NSString *)inToken
                tokenType:(NSString *)inTokenType
                 onStatus:(ALAuthorizationJobOnStatus)inOnStatus
             onAuthorized:(ALAuthorizationJobOnAuthorized)inOnAuthorized {
    AL_LOG_AUTHORIZATIONJOB(@"");
    [self _onStatus:inOnStatus];
    [self _onAuthorized:inOnAuthorized];

    [self _setStatus:kALAuthStatus_AccountAccessPrivileges_Acquiring];

    ALAuthorizationJob *existingJob = sJobsByAdapterID[_adapterID];
    if (existingJob) {
        AL_LOG_AUTHORIZATIONJOB(@"Already authorizing adapter [%@].  Aborting REDUNDANT AUTHORIZATION", _adapterID);
        [self _setError:[NSError ALError:kALError_Conflict userInfo:nil]];
        return;
    }
    if (!sJobsByAdapterID) {
        sJobsByAdapterID = [NSMutableDictionary new];
    }
    sJobsByAdapterID[_adapterID] = self;

    _urlSession = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration ephemeralSessionConfiguration]];
    [self _getDeviceInfoWithOAuthToken:inToken type:inTokenType];
}

- (void)runWithSecret:(NSString *)inClientSecret
             onStatus:(ALAuthorizationJobOnStatus)inOnStatus
         onAuthorized:(ALAuthorizationJobOnAuthorized)inOnAuthorized {
    AL_LOG_AUTHORIZATIONJOB(@"");
    [self _onStatus:inOnStatus];
    [self _onAuthorized:inOnAuthorized];

    [self _setStatus:kALAuthStatus_AccountAccessPrivileges_Acquiring];

    ALAuthorizationJob *existingJob = sJobsByAdapterID[_adapterID];
    if (existingJob) {
        AL_LOG_AUTHORIZATIONJOB(@"Already authorizing adapter [%@].  Aborting REDUNDANT AUTHORIZATION", _adapterID);
        [self _setError:[NSError ALError:kALError_Conflict userInfo:nil]];
        return;
    }
    if (!sJobsByAdapterID) {
        sJobsByAdapterID = [NSMutableDictionary new];
    }
    sJobsByAdapterID[_adapterID] = self;

    // See "Example CURL for Access Token" at https://developer.automatic.com/my-apps/
    NSURL *acctAccessReqEndpoint = [NSURL URLWithString:@"https://accounts.automatic.com/oauth/access_token"];
    NSMutableURLRequest *acctAccessReq = [NSMutableURLRequest requestWithURL:acctAccessReqEndpoint];
    [acctAccessReq setHTTPMethod:@"POST"];

    NSString *acctAccessReqFormData = [NSString stringWithFormat:
                                                    @"client_id=%@&"
                                                    @"client_secret=%@&"
                                                    @"code=%@&"
                                                    @"grant_type=authorization_code",
                                                    _clientID,
                                                    inClientSecret,
                                                    _acctAccessReqCode];
    [acctAccessReq setHTTPBody:[acctAccessReqFormData dataUsingEncoding:NSUTF8StringEncoding]];

    AL_LOG_AUTHORIZATIONJOB(@"SENDING %@{%@}{%@}", acctAccessReq, [acctAccessReq allHTTPHeaderFields], [NSString stringWithUTF8Data:[acctAccessReq HTTPBody]]);
    _urlSession = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration ephemeralSessionConfiguration]];
    _dataTask = [_urlSession dataTaskWithRequest:acctAccessReq completionHandler:^(NSData *inData, NSURLResponse *inResponse, NSError *inError) {
        // NSURLSessionTask calls back in its own thread.  Resume in the main thread:
        dispatch_async(dispatch_get_main_queue(), ^{
            [self gotResponse:inResponse withData:inData toAccountAccessRequest:acctAccessReq error:inError];
        });
    }];
    [_dataTask resume];
}

+ (BOOL)waitIfAuthorizingAdapter:(ALAutomaticAdapter *)inAdapter onVerdict:(ALAuthorizationJobOnWaitVerdict)inOnVerdict {
    NSThread *homeThread = [NSThread currentThread];
    BOOL __block jobPending = NO;
    void (^onWait)(void) = ^{
        jobPending = [self _waitForAuthorizationForAdapter:inAdapter onThread:homeThread onVerdict:inOnVerdict];
    };
    if ([NSThread isMainThread]) {
        onWait();
    } else {
        dispatch_sync(dispatch_get_main_queue(), onWait);
    }

    return jobPending;
}

#pragma mark -

- (BOOL)_checkError:(NSError *)inError {
    if (!inError) {
        return YES;
    }

    [self _setError:inError];

    return NO;
}

- (void)_getDeviceInfoWithOAuthToken:(NSString *)inOAuthToken type:(NSString *)inTokenType {
    if ([inOAuthToken length] == 0) {
        AL_LOG_AUTHORIZATIONJOB(@"NO OAUTH TOKEN");
        [self _setError:[NSError ALError:kALError_Unauthorized userInfo:nil]];
        return;
    }

    if ([inTokenType length] == 0) {
        AL_LOG_AUTHORIZATIONJOB(@"NO OAUTH TOKEN TYPE");
        [self _setError:[NSError ALError:kALError_Unauthorized userInfo:nil]];
        return;
    }

    [self _setStatus:kALAuthStatus_AdapterAccessPrivileges_Acquiring];

    // curl -XGET -d- https://api.automatic.com/device/ -H'Authorization: bearer 9db294e578cef59db9ea8b204df029695c33b57f' -H'User-Agent: {"os": {"name": "iOS"}}'
    NSURL *deviceEndpoint = [NSURL URLWithString:[NSString stringWithFormat:@"https://api.automatic.com/device/%@/", _adapterID]];
    NSMutableURLRequest *deviceReq = [NSMutableURLRequest requestWithURL:deviceEndpoint];
    NSString *apiAuthorization = [NSString stringWithFormat:@"%@ %@", inTokenType, inOAuthToken];
    [deviceReq addValue:apiAuthorization forHTTPHeaderField:@"Authorization"];
    [deviceReq addValue:@"{\"os\": {\"name\": \"iOS\"}}" forHTTPHeaderField:@"User-Agent"];

    AL_LOG_AUTHORIZATIONJOB(@"SENDING %@{%@}{%@}", deviceReq, [deviceReq allHTTPHeaderFields], [NSString stringWithUTF8Data:[deviceReq HTTPBody]]);
    _dataTask = [_urlSession dataTaskWithRequest:deviceReq completionHandler:^(NSData *inData, NSURLResponse *inResponse, NSError *inError) {
        // NSURLSessionTask calls back in its own thread.  Resume in the main thread:
        dispatch_async(dispatch_get_main_queue(), ^{
            [self gotResponse:inResponse withData:inData toInfoRequest:deviceReq error:inError];
        });
    }];
    [_dataTask resume];
    [_urlSession finishTasksAndInvalidate];
}

- (void)_gotAuthorizationVerdict:(NSError *)inError
                      forAdapter:(ALAutomaticAdapter *)inAdapter
                  usingSecretKey:(NSData *)inSecretKey {
    AL_LOG_AUTHORIZATIONJOB(@"%@", inError);
    if (![self _checkError:inError]) {
        return;
    }

    ALAuthorization *authorization = [ALAuthorization authorizationForAdapter:inAdapter];
    NSError *error = [authorization saveSecret:inSecretKey];
    if (![self _checkError:error]) {
        AL_LOG_AUTHORIZATIONJOB(@"setAutomaticAdapterKey failed: %@", error);
        return;
    }

    AL_LOG_AUTHORIZATIONJOB(@"kALAuthStatus_AdapterAccessPrivileges_Installed");
    [self _setStatus:kALAuthStatus_AdapterAccessPrivileges_Installed];

    [sJobsByAdapterID removeObjectForKey:_adapterID];
    if ([sJobsByAdapterID count] == 0) {
        sJobsByAdapterID = nil;
    }
    _onAuthorized(inAdapter, _clientID, _onStatus);
    [self _onStatus:nil];
    [self _onAuthorized:nil];
    for (_ALAuthorizationJobObserver *eachObserver in _waiters) {
        [eachObserver gotVerdict:nil withStatus:_status];
    }
    _waiters = nil;
}

- (void)gotResponse:(NSURLResponse *)inResponse
                  withData:(NSData *)inData
    toAccountAccessRequest:(NSURLRequest *)inRequest
                     error:(NSError *)inError {
    if (![self _checkError:inError]) {
        return;
    }
    if (![inResponse isKindOfClass:[NSHTTPURLResponse class]]) {
        [self _setError:[NSError ALError:kALError_BadResponse userInfo:@{ @"response" : inResponse }]];
        return;
    }

    NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)inResponse;
    NSInteger statusCode = [httpResponse statusCode];
    if (statusCode != 200) {
        AL_LOG_AUTHORIZATIONJOB(@"HTTP ERROR %@ to %@{%@}", inResponse, inRequest, [NSString stringWithUTF8Data:[inRequest HTTPBody]]);
        [self _setError:[NSError errorWithDomain:kALErrorDomainHTTP code:statusCode userInfo:@{ @"response" : inResponse }]];
        return;
    }

    if ([inData length] == 0) {
        [self _setError:[NSError ALError:kALError_BadResponse userInfo:@{ @"response" : inResponse }]];
        return;
    }

    // {"access_token": "9db294e578cef59db9ea8b204df029695c33b57f", "user": {"id": "18c0f74c58b1fe7e0c32", "sid": "U_8738add82c6fecff"}, "refresh_token": "82c8682207fcaf7f42d459e5f5d3f40a0e1097a8", "scope": "scope:adapter:basic scope:public", "expires_in": 31535999, "token_type": "Bearer"}
    NSError *__autoreleasing error;
    id oauth = [NSJSONSerialization JSONObjectWithData:inData options:0 error:&error];
    if (!oauth) {
        [self _setError:[NSError ALError:kALError_BadResponse userInfo:@{ @"response" : inResponse }]];
        return;
    }
    if (![oauth isKindOfClass:[NSDictionary class]]) {
        [self _setError:[NSError ALError:kALError_BadResponse userInfo:@{ @"response" : inResponse }]];
        return;
    }

    NSDictionary *oauthDict = (NSDictionary *)oauth;
    [self _getDeviceInfoWithOAuthToken:oauthDict[@"access_token"] type:oauthDict[@"token_type"]];
}

- (void)gotResponse:(NSURLResponse *)inResponse withData:(NSData *)inData toInfoRequest:(NSURLRequest *)inRequest error:(NSError *)inError {
    _dataTask = nil;
    _urlSession = nil;

    if (![self _checkError:inError]) {
        return;
    }
    if (![inResponse isKindOfClass:[NSHTTPURLResponse class]]) {
        [self _setError:[NSError ALError:kALError_BadResponse userInfo:@{ @"response" : inResponse }]];
        return;
    }

    NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)inResponse;
    NSInteger statusCode = [httpResponse statusCode];
    if (statusCode != 200) {
        AL_LOG_AUTHORIZATIONJOB(@"HTTP ERROR %@ to %@{%@}", inResponse, inRequest, [NSString stringWithUTF8Data:[inRequest HTTPBody]]);
        [self _setError:[NSError errorWithDomain:kALErrorDomainHTTP code:statusCode userInfo:@{ @"response" : inResponse }]];
        return;
    }

    if ([inData length] == 0) {
        [self _setError:[NSError ALError:kALError_BadResponse userInfo:@{ @"response" : inResponse }]];
        return;
    }

    //	{
    //	   "id":"bf662327073bc8781974ccb2",
    //	   "url":"https://api.automatic.com/device/bf662327073bc8781974ccb2/",
    //	   "version":2,
    //	   "direct_access_token":"authticket 1 bf662327:073bc878:1974ccb2 1 bd28d964c234e9df04eb 192:a4ffaa3a:8b3d5583:5ba32bbe:3653dcb4:37e54f5b:ba57db1d 1428660713 aa1e786cdfa07720cdf730fde180ac67d7fc6c9f2a2342559efdc9591db304ebedac97c9a700523588482f8a93860e1ac5b616a50e3ec2aa493af7dc5059915ab8cba1f8f99a035eb19663d04b3127882968d201cba277cea22585abc093f479783e1d2dd1914d37afabbe3248a0dd127495a67f9615a68bfa7c2436df4f4604",
    //	   "app_encryption_key":"0a28d228fa8f6090fdc3f69bb9e43d9c"
    //	}
    NSError *__autoreleasing error;
    id deviceDesc = [NSJSONSerialization JSONObjectWithData:inData options:0 error:&error];
    if (!deviceDesc) {
        [self _setError:[NSError ALError:kALError_BadResponse userInfo:@{ @"response" : inResponse }]];
        return;
    }
    if (![deviceDesc isKindOfClass:[NSDictionary class]]) {
        [self _setError:[NSError ALError:kALError_BadResponse userInfo:@{ @"response" : inResponse }]];
        return;
    }

    NSDictionary *deviceDict = (NSDictionary *)deviceDesc;
    AL_LOG_AUTHORIZATIONJOB(@"%@", deviceDict);

    NSString *secretKeyStr = deviceDict[@"app_encryption_key"];
    if ([secretKeyStr length] == 0) {
        [self _setError:[NSError ALError:kALError_BadResponse userInfo:deviceDict]];
        return;
    }

    [self _setStatus:kALAuthStatus_AdapterAccessPrivileges_Installing];

    NSString *authorization = deviceDict[@"direct_access_token"];
    ALAutomaticAdapter *adapter = [ALAutomaticAdapter connectedAdapterWithID:_adapterID];
    NSData *secretKey = [NSData dataWithHex:secretKeyStr];
    [ALELM327Session installAuthorization:authorization onAdapter:adapter onVerdict:^(NSError *inError) {
        [self _gotAuthorizationVerdict:inError forAdapter:adapter usingSecretKey:secretKey];
    }];
}

- (void)_onAuthorized:(ALAuthorizationJobOnAuthorized)inOnAuthorized {
    if (inOnAuthorized) {
        _onAuthorized = [inOnAuthorized copy];
    } else {
        [self _onAuthorized:^(ALAutomaticAdapter *inAdapter, NSString *inClientID, ALAuthorizationJobOnStatus inStatusCallback){

        }];
    }
}

- (void)_onStatus:(ALAuthorizationJobOnStatus)inOnStatus {
    if (inOnStatus) {
        _onStatus = [inOnStatus copy];
    } else {
        [self _onStatus:^(ALAuthStatus inAuthStatus, NSError *inError){

        }];
    }
}

+ (BOOL)_waitForAuthorizationForAdapter:(ALAutomaticAdapter *)inAdapter
                               onThread:(NSThread *)inHomeThread
                              onVerdict:(ALAuthorizationJobOnWaitVerdict)inOnVerdict {
    NSAssert([NSThread isMainThread], @"must be called on main thread");
    NSString *adapterID = [[inAdapter accessory] serialNumber];
    ALAuthorizationJob *job = sJobsByAdapterID[adapterID];
    if (!job) {
        AL_LOG_AUTHORIZATIONJOB(@"Authorization for adapter [%@] NOT PENDING in %@", adapterID, sJobsByAdapterID);
        return NO;
    }

    AL_LOG_AUTHORIZATIONJOB(@"AUTHORIZATION IN PROGRESS for adapter [%@]", adapterID);
    [job _waitForVerdict:inOnVerdict onThread:inHomeThread];
    return YES;
}

- (void)_waitForVerdict:(ALAuthorizationJobOnWaitVerdict)inOnVerdict
               onThread:(NSThread *)inHomeThread {
    NSAssert([NSThread isMainThread], @"must be called on main thread");
    _ALAuthorizationJobObserver *newWaiter = [_ALAuthorizationJobObserver observer:inOnVerdict onThread:inHomeThread];
    if (!_waiters) {
        _waiters = [NSMutableArray new];
    }
    [_waiters addObject:newWaiter];
}

@end

#pragma mark -

@implementation _ALAuthorizationJobObserver {
  @private
    ALAuthorizationJobOnWaitVerdict _onVerdict;
    NSThread *_homeThread;
    ALAuthStatus _status;
}

+ (instancetype)observer:(ALAuthorizationJobOnWaitVerdict)inOnVerdict
                onThread:(NSThread *)inHomeThread {
    return [[self alloc] initWithObserver:inOnVerdict onThread:inHomeThread];
}

- (instancetype)initWithObserver:(ALAuthorizationJobOnWaitVerdict)inOnVerdict
                        onThread:(NSThread *)inHomeThread {
    if ((self = [super init])) {
        _onVerdict = [inOnVerdict copy];
        _homeThread = inHomeThread;
    }

    return self;
}

- (void)gotVerdict:(NSError *)inError withStatus:(ALAuthStatus)inStatus {
    _status = inStatus;
    [self performSelector:@selector(_onDispatch:) onThread:_homeThread withObject:inError waitUntilDone:NO];
}

- (void)_onDispatch:(NSError *)inVerdict {
    if (_onVerdict) {
        _onVerdict(_status, inVerdict);
        _onVerdict = nil;
    }
}

@end
