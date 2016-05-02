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

#import "ALAuthorization.h"
#import "ALAutomaticAdapter.h"
#import "ALDuplexStream.h"
#import "ALELM327Session.h"
#import "ALSocket.h"
#import "ALSocket_EAAccessory.h"
#import "EAAccessory+AL.h"
#import "NSError+AL.h"
#import "NSMutableString+AL.h"

#ifndef AL_LOG_ELM327SESSION
#if 0
#define AL_LOG_ELM327SESSION AL_LOG
#else
#define AL_LOG_ELM327SESSION(...)
#endif
#endif

@interface ALELM327Session ()

@property (nonatomic) NSError *error;

@end

#pragma mark -

static NSTimeInterval const _kConnectTimeoutSecs = 3.0;
static NSRegularExpression *_endOfResponse;
static NSString * const _kEOL = @"\r\n";
static NSNull *_endOfScript;

@implementation ALELM327Session {
  @private
    ALELM327SessionOnOpened _onOpened;
    ALELM327SessionOnClosed _onClosed;
    NSString *_sending;
    NSInteger _sent;
    NSString *_receiving;
    NSMutableArray *_script;
    NSMutableArray *_onResponses;
    ALELM327SessionOnNoMoreCommands _onNoMoreCommands;
    NSUInteger _timeoutID;
}

+ (void)initialize {
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        _endOfResponse = [NSRegularExpression regularExpressionWithPattern:@"(\\r|\\r?\\n)>" options:0 error:nil];
        _endOfScript = [NSNull null];
    });
}

#pragma mark - Lifecycle

- (void)dealloc {
    AL_LOG_ELM327SESSION(@"");
    [self _cancelTimeout:_timeoutID];
}

+ (void)installAuthorization:(NSString *)inAuthorization
                   onAdapter:(ALAutomaticAdapter *)inAdapter
                   onVerdict:(ALELM327SessionOnAuthorizationVerdict)inOnVerdict {
    ALELM327Session *__block elmSession = [self sessionWithAdapter:inAdapter];
    ALELM327SessionOnAuthorizationVerdict onVerdict = [inOnVerdict copy];
    [elmSession open:^(NSString *inGreeting) {
        AL_LOG_ELM327SESSION(@"'%@'", inGreeting);
        [elmSession installAuthorization:inAuthorization onVerdict:^(NSError *inError) {
            if (!elmSession) {
                // Session already completed
                return;
            }

            elmSession = nil;

            if (onVerdict) {
                // Defer callback until the next iteration of the run loop to give the session
                // a chance to close
                dispatch_async(dispatch_get_main_queue(), ^{
                    onVerdict(inError);
                });
            }
        }];
    }
        onClosed:^(NSError *inError, BOOL inWasOpen) {
            AL_LOG_ELM327SESSION(@"CLOSED on %@", inError);
            if (!elmSession) {
                // Session already completed
                return;
            }

            NSAssert(inError, @"Authorization session closed without authorization and without error");
            elmSession = nil;

            if (onVerdict) {
                // Defer callback until the next iteration of the run loop to give the session
                // a chance to close
                dispatch_async(dispatch_get_main_queue(), ^{
                    onVerdict(inError);
                });
            }
        }];
}

+ (void)openAuthorizedSessionWithAdapter:(ALAutomaticAdapter *)inAdapter
                      forAutomaticClient:(NSString *)inClientID
                 allowingUserInteraction:(BOOL)inInteractionAllowed
                                onStatus:(ALELM327SessionOnAuthorizationStatus)inOnStatus
                            onAuthorized:(ALELM327SessionOnAuthorized)inOnAuthorized
                                onClosed:(ALELM327SessionOnClosed)inOnClosed {
    // See https://phabricator.automatic.co/file/data/o7ioaavkphtunjj44ern/PHID-FILE-3hd2ur37so5x3thrkw6b/T9884-Auth_Flow-v3.pdf
    // for a diagram of authorization and authentication flow.

    ALELM327SessionOnAuthorizationStatus onStatus = [inOnStatus copy];
    ALELM327SessionOnAuthorized onAuthorized = [inOnAuthorized copy];
    ALELM327SessionOnClosed onClosed = [inOnClosed copy];
    __block ALELM327Session *elmSession = [ALELM327Session sessionWithAdapter:inAdapter];
    [elmSession openAuthorizedSessionForAutomaticClient:inClientID allowingUserInteraction:inInteractionAllowed onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
        BOOL stopped = inError  ||  (inAuthStatus == kALAuthStatus_UserInteractionRequired  &&  !inInteractionAllowed);
        
        /// @todo The final callback if an error occurs during authentication should probably
        ///       be to the inOnClosed block (with inWasOpen = NO), rather than to the status
        ///       callback, so that the caller can handle all session-ending conditions in
        ///       one place.
        
        if (stopped) {
            // This is the final callback.  Release the session:
            elmSession = nil;
        }
        
        if (onStatus) {
            if (stopped) {
                // Releasing the session doesn't close the underlying iAP session synchronously.  It's unclear why,
                // but empirically it behaves like it closes on autorelease.  Defer the final callback until the
                // next iteration of the run loop to give the session a chance to close before invoking the callback:
                dispatch_async(dispatch_get_main_queue(), ^{
                    onStatus(inAuthStatus, inError);
                });
            } else {
                onStatus(inAuthStatus, inError);
            }
        }
    }
    onAuthorized:^(ALELM327Session *inSession, NSString *inGreeting) {
        elmSession = nil;
        onAuthorized(inSession, inGreeting);
    }
    onClosed:^(NSError *inError, BOOL inWasOpen) {
        // This is the final callback.  Release the session:
        elmSession = nil;
        
        // Releasing the session doesn't close the underlying iAP session synchronously.  It's unclear why,
        // but empirically it behaves like it closes on autorelease.  Defer the final callback until the
        // next iteration of the run loop to give the session a chance to close before invoking the callback:
        if (onClosed) {
            dispatch_async(dispatch_get_main_queue(), ^{
                onClosed(inError, YES);
            });
        }
    }];
}

+ (instancetype)sessionWithAdapter:(ALAutomaticAdapter *)inAdapter {
    return [[self alloc] initWithAdapter:inAdapter];
}

+ (instancetype)sessionWithSocket:(ALSocket *)inSocket {
    return [[self alloc] initWithSocket:inSocket];
}

- (instancetype)initWithAdapter:(ALAutomaticAdapter *)inAdapter {
    ALSocket *socket = [ALSocket socketWithProtocol:[ALAutomaticAdapter protocolID] atAccessory:[inAdapter accessory]];
    if ((self = [self initWithSocket:socket])) {
        _adapter = inAdapter;
    }

    return self;
}

- (instancetype)initWithSocket:(ALSocket *)inSocket {
    if ((self = [super init])) {
        _socket = inSocket;
    }

    return self;
}

#pragma mark - Property accessors

- (void)setError:(NSError *)inError {
    AL_LOG_ELM327SESSION(@">> ERROR << %@", inError);
    _error = inError;
    if (_error) {
        [self _abort:_error];
    }
}

- (BOOL)isConnected {
    return self.adapter.accessory.isConnected;
}

#pragma mark - ALELM327Session methods

- (void)authenticateClient:(NSString *)inAutomaticClientID
         withAuthorization:(ALAuthorization *)inAuthorization
                 onVerdict:(ALELM327SessionOnAuthenticationVerdict)inOnVerdict {
    NSAssert(inOnVerdict, @"nil ALELM327SessionOnAuthenticationVerdict");

    if (![inAuthorization exists]) {
        inOnVerdict([NSError ALError:kALError_Unauthorized userInfo:nil], YES);
        return;
    }

    ALELM327Session *__weak weakSelf = self;
    NSString *loginCmd = [@"ALLG" stringByAppendingString:inAutomaticClientID];
    ALELM327SessionOnAuthenticationVerdict onVerdict = [inOnVerdict copy];
    NSUInteger timeoutID = [self _startTimeout:_kConnectTimeoutSecs];
    [weakSelf sendLine:loginCmd onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        AL_LOG_ELM327SESSION(@"%@ -> %@", inCommand, inResponse);
        if (!inError) {
            NSString *proof = [inAuthorization proofWithChallenge:inResponse];
            [weakSelf sendLine:proof onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
                AL_LOG_ELM327SESSION(@"%@ -> %@", inCommand, inResponse);
                [weakSelf _cancelTimeout:timeoutID];
                NSDictionary *refusals = @{
                    @"?" : @(kALError_Unauthorized),
                    @"UNSYNCED" : @(kALError_Unsynced),
                };
                NSNumber *refusal = inError ? nil : refusals[inResponse];
                NSError *error =
                    inError ? inError : refusal ? [NSError ALError:(ALErrorCode)[refusal unsignedIntegerValue] userInfo:nil] : nil;
                onVerdict(error, (refusal != nil));
            }];
        }
    }];
}

- (id<ALDuplexStream>)detach {
    return [_socket detach];
}

- (void)installAuthorization:(NSString *)inAuthorization onVerdict:(ALELM327SessionOnAuthorizationVerdict)inOnVerdict {
    ALELM327SessionOnAuthorizationVerdict onVerdict = [inOnVerdict copy];
    [self sendLine:@"ALAA" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        AL_LOG_ELM327SESSION(@"%@ -> %@", inCommand, inResponse);
        if (inError) {
            if (onVerdict) {
                onVerdict(inError);
            }
            return;
        }

        [self sendLine:inAuthorization onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
            AL_LOG_ELM327SESSION(@"%@ -> %@", inCommand, inResponse);
            NSError *error =
                inError ? inError : [inResponse isEqualToString:@"OK"] ? nil : [NSError ALError:kALError_Unauthorized userInfo:@{ @"authorization" : inAuthorization }];
            if (onVerdict) {
                onVerdict(error);
            }
        }];
    }];
}

- (void)noMoreCommands:(ALELM327SessionOnNoMoreCommands)inOnDone {
    BOOL idle = ([_script count] == 0);
    [_script addObject:_endOfScript];
    [self _onResponse:nil];
    _onNoMoreCommands = [inOnDone copy];
    if (idle) {
        [self _runScript];
    }
}

- (void)open:(ALELM327SessionOnOpened)inOnOpened onClosed:(ALELM327SessionOnClosed)inOnClosed {
    _onOpened = [inOnOpened copy];
    _onClosed = [inOnClosed copy];

    ALELM327Session *__weak weakSelf = self;
    NSUInteger timeoutID = [self _startTimeout:_kConnectTimeoutSecs];
    [_socket open:^(BOOL inWasInitiallyClosed) {
        [weakSelf _cancelTimeout:timeoutID];
        [weakSelf _socketUp:inWasInitiallyClosed];
    }
    onData:^(NSData *inUnretainableData) {
        [weakSelf _gotData:inUnretainableData];
    }
    onClosed:^(NSError *inError, BOOL inWasOpen) {
        [weakSelf _socketDown:inError wasUp:inWasOpen];
    }];
}

- (void)openAuthorizedSessionForAutomaticClient:(NSString *)inClientID
                        allowingUserInteraction:(BOOL)inInteractionAllowed
                                       onStatus:(ALELM327SessionOnAuthorizationStatus)inOnStatus
                                   onAuthorized:(ALELM327SessionOnAuthorized)inOnAuthorized
                                       onClosed:(ALELM327SessionOnClosed)inOnClosed {
    NSAssert(inOnAuthorized, @"nil ALELM327SessionOnAuthorized callback");

    ALELM327SessionOnAuthorizationStatus onStatus = [inOnStatus copy];
    ALELM327SessionOnAuthorized onAuthorized = [inOnAuthorized copy];
    ALELM327SessionOnClosed onClosed = [inOnClosed copy];
    [ALAuthorization waitForAuthorizationForAdapter:_adapter onVerdict:^(ALAuthStatus inAuthStatus, ALAuthorization *inAuthorization, NSError *inError) {
        if (inError) {
            if (onStatus) {
                onStatus(inAuthStatus, inError);
            }
            return;
        }
        
        // See https://phabricator.automatic.co/file/data/o7ioaavkphtunjj44ern/PHID-FILE-3hd2ur37so5x3thrkw6b/T9884-Auth_Flow-v3.pdf
        // for a diagram of authorization and authentication flow from this point.

        ALAuthorization *authAgent = inInteractionAllowed ? inAuthorization : nil;
        BOOL haveAuth = [inAuthorization exists];
        if (haveAuth) {
            AL_LOG_ELM327SESSION(@"AUTHORIZATION EXISTS.  Connecting to %@", _adapter);
            ALELM327Session * __weak weakSelf = self;
            __block BOOL elmSessionPosted = NO;
            if (onStatus) {
                onStatus(kALAuthStatus_AdapterSession_Authenticating, nil);
            }
            [self open:^(NSString *inGreeting) {
                [weakSelf authenticateClient:inClientID withAuthorization:inAuthorization onVerdict:^(NSError *inError, BOOL inRefused) {
                    ALErrorCode refusal = inRefused ? (ALErrorCode)[inError code] : kALError_None;

                    /// @todo   Check for inRefused here instead of refusal == kALError_Unauthorized
                    ///         when handling of kALError_Unsynced is implemented
                    BOOL willInteract = (refusal == kALError_Unauthorized && inInteractionAllowed);
                    NSError *error = willInteract ? nil : inError;
                    ALAuthStatus status =
                        inRefused ? kALAuthStatus_UserInteractionRequired : error ? kALAuthStatus_AdapterSession_Authenticating : kALAuthStatus_AdapterSession_Authenticated;
                    if (onStatus) {
                        onStatus(status, error);
                    }

                    if (refusal == kALError_Unauthorized) {
                        AL_LOG_ELM327SESSION(@"Discarding REJECTED AUTHORIZATION");
                        [inAuthorization discard];
                        [authAgent askForAuthorizationForClient:inClientID];
                    } else if (refusal == kALError_Unsynced) {
                        AL_LOG_ELM327SESSION(@"UNSYNCED");
                        /// @todo Initiate sync here
                    } else if (!inError) {
                        elmSessionPosted = YES;
                        onAuthorized(weakSelf, inGreeting);
                    }
                }];
            }
            onClosed:^(NSError *inError, BOOL inWasOpen) {
                if (elmSessionPosted) {
                    if (onClosed) {
                        onClosed(inError, YES);
                    }
                } else {
                    AL_LOG_ELM327SESSION(@"ABORTING AUTHENTICATION on %@", inError);
                    if (onStatus) {
                        onStatus(kALAuthStatus_AdapterSession_Authenticating, inError);
                    }
                }
            }];
        } else {
            AL_LOG_ELM327SESSION(@"NO KEY for %@%@", _adapter, inInteractionAllowed ? @"" : @".  ABORTING");
            if (onStatus) {
                NSError *error = inInteractionAllowed ? nil : [NSError ALError:kALError_Unauthorized userInfo:nil];
                onStatus(kALAuthStatus_UserInteractionRequired, error);
            }
            [authAgent askForAuthorizationForClient:inClientID];
        }
    }];
}

- (void)sendLine:(NSString *)inLine onResponse:(ALELM327SessionOnResponse)inOnResponse {
    AL_LOG_ELM327SESSION(@"%@", inLine);
    BOOL idle = ([_script count] == 0);
    NSString *trimmed = [inLine stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    [_script addObject:trimmed];
    [self _onResponse:inOnResponse];
    if (idle) {
        [self _runScript];
    }
}

- (void)startStreaming:(NSArray<NSString *>*)PIDs onResponse:(ALELM327SessionOnStream)inOnStream {
    [[NSException exceptionWithName:NSInternalInconsistencyException reason:@"method not implemented" userInfo:nil] raise];
}

- (void)stopStreaming {
    [[NSException exceptionWithName:NSInternalInconsistencyException reason:@"method not implemented" userInfo:nil] raise];
}

#pragma mark -

- (void)_abort:(NSError *)inWhy {
    NSDictionary *errorInfo = inWhy ? @{ @"why" : inWhy } : nil;
    NSError *abortError = [NSError ALError:kALError_Aborted userInfo:errorInfo];
    NSError *commandError = inWhy;

    while ([_script count] > 0) {
        NSString *command = [_script firstObject];
        ALELM327SessionOnResponse onResponse = [_onResponses firstObject];
        [_script removeObjectAtIndex:0];
        [_onResponses removeObjectAtIndex:0];
        onResponse(command, nil, commandError);
        commandError = abortError;
    }
}

- (void)
    _badResponse:(NSString *)inResponse
        expected:(id)inExpected {
    [self setError:[NSError ALError:kALError_BadResponse userInfo:@{
              @"expected" : inExpected ? inExpected : @"",
              @"got" : inResponse,
          }]];
}

- (void)_cancelTimeout:(NSUInteger)inTimeoutID {
    if (_timeoutID == inTimeoutID) {
        _timeoutID++;
        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(_onTimeout:) object:@(inTimeoutID)];
        AL_LOG_ELM327SESSION(@"Timeout [%u] canceled", (unsigned)inTimeoutID);
    }
}

- (void)_gotData:(NSData *)inData {
    AL_LOG_ELM327SESSION(@"GOT [%@]",
                         [[[NSString stringWithUTF8Data:inData]
                             stringByReplacingOccurrencesOfString:@"\n"
                                                       withString:@"\\n"]
                             stringByReplacingOccurrencesOfString:@"\r"
                                                       withString:@"\\r"]);
    NSMutableString *received = [NSMutableString stringWithString:(_receiving ? _receiving : @"")];
    [received appendUTF8Data:inData];

    NSArray *responses = [received componentsSeparatedByPattern:_endOfResponse];
    _receiving = [responses lastObject];

    NSArray *completeResponses = [responses subarrayWithRange:NSMakeRange(0, [responses count] - 1)];
    for (NSString *eachResponse in completeResponses) {
        NSString *trimmed = [eachResponse stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        [self gotResponse:trimmed];
    }
    AL_LOG_ELM327SESSION(@"deferring '%@'", _receiving);
}

- (void)gotResponse:(NSString *)inResponse {
    AL_LOG_ELM327SESSION(@"'%@'", [[inResponse
                                      stringByReplacingOccurrencesOfString:@"\n"
                                                                withString:@"\\n"]
                                      stringByReplacingOccurrencesOfString:@"\r"
                                                                withString:@"\\r"]);

    NSString *command = [_script firstObject];
    if (!command) {
        [self _badResponse:inResponse expected:nil];
        return;
    }

    NSString *echoLine = [command stringByAppendingString:_kEOL];
    NSRange echoRange = [inResponse rangeOfString:echoLine];
    NSString *sansEcho = (echoRange.location == 0) ? [inResponse substringFromIndex:NSMaxRange(echoRange)] : inResponse;
    NSString *trimmed = [sansEcho stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    ALELM327SessionOnResponse onResponse = [_onResponses firstObject];
    onResponse(command, trimmed, nil);
    [_script removeObjectAtIndex:0];
    [_onResponses removeObjectAtIndex:0];
    [self _runScript];
}

- (void)_onResponse:(ALELM327SessionOnResponse)inOnResponse {
    if (!inOnResponse) {
        [self _onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError){
        }];
    } else {
        [_onResponses addObject:[inOnResponse copy]];
    }
}

- (void)_onTimeout:(NSNumber *)inTimeoutID {
    if ([inTimeoutID unsignedIntegerValue] == _timeoutID) {
        AL_LOG_ELM327SESSION(@"TIMEOUT [%u] LAPSED", (unsigned)_timeoutID);
        _timeoutID++;
        [self setError:[NSError ALError:kALError_Timeout userInfo:nil]];
    }
}

- (void)_runScript {
    NSString *command = [_script firstObject];
    if (!command) {
        AL_LOG_ELM327SESSION(@"Nothing to do");
        return;
    }

    if (command == (id)_endOfScript) {
        AL_LOG_ELM327SESSION(@"END OF SCRIPT");
        [_script removeAllObjects];
        [_socket noMoreData:_onNoMoreCommands];
        _onNoMoreCommands = nil;
        return;
    }

    AL_LOG_ELM327SESSION(@"Runnning [%@]", command);
    NSString *withEol = [command stringByAppendingString:_kEOL];
    const char *asUtf8 = [withEol UTF8String];
    NSData *asData = [NSData dataWithBytes:asUtf8 length:strlen(asUtf8)];
    ALELM327Session *__weak weakSelf = self;
    [_socket send:asData onVerdict:^(NSData *inOriginalData, NSInteger inSentSize, NSError *inError) {
        [weakSelf _sent:inSentSize ofData:inOriginalData withError:inError];
    }];
}

- (void)_sent:(NSInteger)inSentSize ofData:(NSData *)inData withError:(NSError *)inError {
    if (inError) {
        AL_LOG_ELM327SESSION(@"Send [%@] FAILED -> %@", [[NSString stringWithUTF8Data:inData] stringByReplacingOccurrencesOfString:@"\n" withString:@"\\n"], inError);
        [self setError:inError];
    } else {
        AL_LOG_ELM327SESSION(@"SENT [%@]",
                             [[[NSString stringWithUTF8Data:[inData subdataWithRange:NSMakeRange(0, inSentSize)]]
                                 stringByReplacingOccurrencesOfString:@"\n"
                                                           withString:@"\\n"]
                                 stringByReplacingOccurrencesOfString:@"\r"
                                                           withString:@"\\r"]);
    }
}

- (void)_sessionFailed:(NSError *)inWhy {
    AL_LOG_ELM327SESSION(@"");
    [self setError:inWhy];
    [self _abort:nil];
    _onResponses = nil;
    _script = nil;
    _onOpened = nil;
    if (_onClosed) {
        _onClosed(inWhy, NO);
        _onClosed = nil;
    }
    [_socket noMoreData:nil];
}

- (void)_sessionUp:(NSString *)inGreeting {
    AL_LOG_ELM327SESSION(@"%@", inGreeting);
    ALELM327SessionOnOpened onOpened = _onOpened;
    _onOpened = nil;
    onOpened(inGreeting);
}

- (void)_socketDown:(NSError *)inWhy wasUp:(BOOL)inSocketWasUp {
    AL_LOG_ELM327SESSION(@"");
    BOOL sessionWasUp = (inSocketWasUp && _onOpened == nil);
    [self setError:inWhy];
    [self _abort:nil];
    _onResponses = nil;
    _script = nil;
    _onOpened = nil;
    if (_onClosed) {
        _onClosed(inWhy, sessionWasUp);
        _onClosed = nil;
    }
}

- (void)_socketUp:(BOOL)inSendReset {
    AL_LOG_ELM327SESSION(@"");
    _script = [NSMutableArray new];
    _onResponses = [NSMutableArray new];

    NSString *cmd = inSendReset ? @"ATZ" : @"ATI";
    ALELM327Session *__weak weakSelf = self;
    NSUInteger timeoutID = [self _startTimeout:_kConnectTimeoutSecs];
    [self sendLine:cmd onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        [weakSelf _cancelTimeout:timeoutID];
        if (inError) {
            [weakSelf _sessionFailed:inError];
        } else {
            [weakSelf _sessionUp:inResponse];
        }
    }];
}

- (NSUInteger)_startTimeout:(NSTimeInterval)inTimeoutSeconds {
    _timeoutID++;
    AL_LOG_ELM327SESSION(@"Timeout [%u] will lapse in [%.3f] secs", (unsigned)_timeoutID, (float)inTimeoutSeconds);
    [self performSelector:@selector(_onTimeout:) withObject:@(_timeoutID) afterDelay:inTimeoutSeconds];
    return _timeoutID;
}

@end
