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

#import <ExternalAccessory/ExternalAccessory.h>
#import <Foundation/Foundation.h>

#import "ALDuplexStream.h"
#import "NSError+AL.h"

@interface ALSocket : NSObject

@property (nonatomic, readonly) NSError *error;
@property (nonatomic, readonly) EASession *accessorySession;

typedef void (^ALSocketOnOpened)(BOOL inWasInitiallyClosed);
typedef void (^ALSocketOnClosed)(NSError *inError, BOOL inWasOpen);
typedef void (^ALSocketOnNoMoreData)(void);
typedef void (^ALSocketOnReceivedData)(NSData *inUnretainableData);
typedef void (^ALSocketOnSendVerdict)(NSData *inOriginalData, NSInteger inSentSize, NSError *inError);

+ (instancetype)socketWithProtocol:(NSString *)inProtocol atAccessory:(EAAccessory *)inAccessory;

+ (instancetype)socketToPort:(NSUInteger)inPort atHost:(NSString *)inHost;

+ (instancetype)socketWithInput:(NSInputStream *)inInputStream output:(NSOutputStream *)inOutputStream;

- (id)initWithInput:(NSInputStream *)inInputStream output:(NSOutputStream *)inOutputStream;

- (void)open:(ALSocketOnOpened)inOnOpened onData:(ALSocketOnReceivedData)inOnReceivedData onClosed:(ALSocketOnClosed)inOnClosed;

- (NSInteger)send:(NSData *)inRetainableData onVerdict:(ALSocketOnSendVerdict)inOnVerdict;

- (id<ALDuplexStream>)detach;

- (void)noMoreData:(ALSocketOnNoMoreData)inOnCompletion;

- (void)onDidEndTransmission;

- (NSError *)onWillOpenInput:(NSInputStream *__autoreleasing *)outInputStream output:(NSOutputStream *__autoreleasing *)outOutputStream;

@end
