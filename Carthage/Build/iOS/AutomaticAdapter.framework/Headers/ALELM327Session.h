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

#import <AutomaticAdapter/ALAuthorization.h>
#import <ExternalAccessory/ExternalAccessory.h>
#import <Foundation/Foundation.h>

@class ALSocket;
@class ALAutomaticAdapter;

@protocol ALDuplexStream;

@interface ALELM327Session : NSObject

@property (nonatomic, readonly) NSError *error;
@property (nonatomic, readonly) ALAutomaticAdapter *adapter;
@property (nonatomic, readonly) ALSocket *socket;
@property (nonatomic, readonly) BOOL isConnected;
@property (nonatomic, readonly) BOOL isStreaming;

typedef void (^ALELM327SessionOnOpened)(NSString *inGreeting);
typedef void (^ALELM327SessionOnClosed)(NSError *inError, BOOL inWasOpen);
typedef void (^ALELM327SessionOnResponse)(NSString *inCommand, NSString *inResponse, NSError *inError);
typedef void (^ALELM327SessionOnStream)(NSArray<NSString *>*inPIDS, NSArray<NSString *>*inResponses, NSError *inError);
typedef void (^ALELM327SessionOnNoMoreCommands)(void);
typedef void (^ALELM327SessionOnAuthorizationVerdict)(NSError *inError);
typedef void (^ALELM327SessionOnAuthorized)(ALELM327Session *inSession, NSString *inGreeting);
typedef void (^ALELM327SessionOnAuthenticationVerdict)(NSError *inError, BOOL inRefused);

typedef ALAuthorizationOnStatus ALELM327SessionOnAuthorizationStatus;

+ (void)openAuthorizedSessionWithAdapter:(ALAutomaticAdapter *)inAdapter
                      forAutomaticClient:(NSString *)inClientID
                 allowingUserInteraction:(BOOL)inInteractionAllowed
                                onStatus:(ALELM327SessionOnAuthorizationStatus)inOnStatus
                            onAuthorized:(ALELM327SessionOnAuthorized)inOnAuthorized
                                onClosed:(ALELM327SessionOnClosed)inOnClosed;

+ (instancetype)sessionWithAdapter:(ALAutomaticAdapter *)inAdapter;

+ (instancetype)sessionWithSocket:(ALSocket *)inSocket;

+ (void)installAuthorization:(NSString *)inAuthorization
                   onAdapter:(ALAutomaticAdapter *)inAdapter
                   onVerdict:(ALELM327SessionOnAuthorizationVerdict)inOnVerdict;

- (void)open:(ALELM327SessionOnOpened)inOnOpened
    onClosed:(ALELM327SessionOnClosed)inOnClosed;

- (void)openAuthorizedSessionForAutomaticClient:(NSString *)inClientID
                        allowingUserInteraction:(BOOL)inInteractionAllowed
                                       onStatus:(ALELM327SessionOnAuthorizationStatus)inOnStatus
                                   onAuthorized:(ALELM327SessionOnAuthorized)inOnAuthorized
                                       onClosed:(ALELM327SessionOnClosed)inOnClosed;

- (void)installAuthorization:(NSString *)inAuthorization
                   onVerdict:(ALELM327SessionOnAuthorizationVerdict)inOnVerdict;

- (void)authenticateClient:(NSString *)inAutomaticClientID
         withAuthorization:(ALAuthorization *)inAuthorization
                 onVerdict:(ALELM327SessionOnAuthenticationVerdict)inOnVerdict;

- (void)sendLine:(NSString *)inLine
      onResponse:(ALELM327SessionOnResponse)inOnResponse;

- (void)startStreaming:(NSArray<NSString *>*)PIDs
      onResponse:(ALELM327SessionOnStream)inOnStream;

- (void)stopStreaming;

- (id<ALDuplexStream>)detach;

- (void)noMoreCommands:(ALELM327SessionOnNoMoreCommands)inOnDone;

@end
