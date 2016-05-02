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

#import "ALSocket_EAAccessory.h"
#import "EASession+AL.h"

#ifndef AL_LOG_SOCKET_EAACCESSORY
#if 0
#define AL_LOG_SOCKET_EAACCESSORY AL_LOG
#else
#define AL_LOG_SOCKET_EAACCESSORY(...)
#endif
#endif

@implementation ALSocket_EAAccessory {
  @private
    EASession *_session;
}

@synthesize accessory = _accessory;
@synthesize protocol = _protocol;

#pragma mark - Lifecycle

+ (instancetype)socketWithProtocol:(NSString *)inProtocol atAccessory:(EAAccessory *)inAccessory {
    return [[self alloc] initWithProtocol:inProtocol atAccessory:inAccessory];
}

- (instancetype)initWithProtocol:(NSString *)inProtocol atAccessory:(EAAccessory *)inAccessory {
    if ((self = [super init])) {
        _accessory = inAccessory;
        _protocol = inProtocol;
    }

    return self;
}

#pragma mark - Property accessors

- (EAAccessory *)accessory {
    return [_session accessory];
}

- (NSString *)protocol {
    return [_session protocolString];
}

#pragma mark - ALSocket overrides

- (EASession *)accessorySession {
    return _session;
}

- (id<ALDuplexStream>)detach {
    EASession *session = _session;
    [super detach];
    _session = nil;

    return session;
}

- (void)onDidEndTransmission {
    AL_LOG_SOCKET_EAACCESSORY(@"");
    [super onDidEndTransmission];
    _session = nil;
}

- (NSError *)onWillOpenInput:(NSInputStream *__autoreleasing *)outInputStream output:(NSOutputStream *__autoreleasing *)outOutputStream {
    _session = [[EASession alloc] initWithAccessory:_accessory forProtocol:_protocol];
    if (!_session) {
        return [NSError ALError:kALError_NewSessionFailed userInfo:nil];
    }

    NSError *error = [super onWillOpenInput:outInputStream output:outOutputStream];
    if (error) {
        return error;
    }

    NSAssert(outInputStream, @"nil NSInputStream storage");
    NSAssert(outOutputStream, @"nil NSOutputStream storage");
    *outInputStream = [_session inputStream];
    *outOutputStream = [_session outputStream];

    return nil;
}

@end
