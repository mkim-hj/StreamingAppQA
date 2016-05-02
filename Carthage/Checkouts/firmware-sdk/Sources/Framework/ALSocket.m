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

#import "ALAdapterDetector.h"
#import "ALSocket.h"
#import "ALSocket_EAAccessory.h"
#import <CoreBluetooth/CoreBluetooth.h>

#ifndef AL_LOG_SOCKET
#if 0
#define AL_LOG_SOCKET AL_LOG
#else
#define AL_LOG_SOCKET(...)
#endif
#endif

@interface _ALSocketDetachment : NSObject <ALDuplexStream>

+ (instancetype)detachmentWithInput:(NSInputStream *)inInput output:(NSOutputStream *)inOutput;

@end

#pragma mark -

@interface ALSocket () <NSStreamDelegate>

@property (nonatomic) NSError *error;

@end

#pragma mark -

typedef void (^_ALEventHandler)(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode);

@implementation ALSocket {
  @private
    ALSocketOnOpened _onOpened;
    ALSocketOnReceivedData _onData;
    ALSocketOnClosed _onClosed;
    NSInputStream *_input;
    NSOutputStream *_output;
    BOOL _intercepting;
    id<NSStreamDelegate> __weak _suspendedRxDelegate;
    id<NSStreamDelegate> __weak _suspendedTxDelegate;
    _ALEventHandler _onRxUp;
    _ALEventHandler _onRxDown;
    _ALEventHandler _onRxData;
    _ALEventHandler _onRxEnd;
    _ALEventHandler _onRxUnexpected;
    _ALEventHandler _onTxUp;
    _ALEventHandler _onTxDown;
    _ALEventHandler _onTxBuffer;
    _ALEventHandler _onTxClose;
    _ALEventHandler _onTxUnexpected;
    NSInteger _sent;
    NSMutableArray *_backlog;
    NSInteger _backlogSize;
    NSMutableArray *_onSendVerdicts;
    ALSocketOnNoMoreData _onNoMoreData;
}

#pragma mark - Lifecycle

- (void)dealloc {
    AL_LOG_SOCKET(@"");
    [self _closeTx];
    [self _closeRx];
}

+ (instancetype)socketToPort:(NSUInteger)inPort atHost:(NSString *)inHost {
    AL_LOG_SOCKET(@"%@:%u", inHost, (unsigned)inPort);
    CFReadStreamRef input;
    CFWriteStreamRef output;
    CFStreamCreatePairWithSocketToHost(NULL, (__bridge CFStringRef)inHost, (unsigned)inPort, &input, &output);

    return [self socketWithInput:(__bridge_transfer NSInputStream *)input output:(__bridge_transfer NSOutputStream *)output];
}

+ (instancetype)socketWithInput:(NSInputStream *)inInputStream output:(NSOutputStream *)inOutputStream {
    return [[self alloc] initWithInput:inInputStream output:inOutputStream];
}

+ (instancetype)socketWithProtocol:(NSString *)inProtocol atAccessory:(EAAccessory *)inAccessory {
    return [ALSocket_EAAccessory socketWithProtocol:inProtocol atAccessory:inAccessory];
}

- (instancetype)initWithInput:(NSInputStream *)inInputStream output:(NSOutputStream *)inOutputStream {
    if ((self = [super init])) {
        _input = inInputStream;
        _output = inOutputStream;
    }
    return self;
}

#pragma mark - Property accessors

- (EASession *)accessorySession {
    return nil;
}

- (void)setError:(NSError *)inError {
    AL_LOG_SOCKET(@">> ERROR << %@", inError);
    _error = inError;
    if (_error) {
        [self _abort:_error];
    }
}

#pragma mark - ALSocket methods

- (id<ALDuplexStream>)detach {
    AL_LOG_SOCKET(@"");
    _ALSocketDetachment *detachment = [_ALSocketDetachment detachmentWithInput:_input output:_output];

    [_input setDelegate:_suspendedRxDelegate];
    [_output setDelegate:_suspendedTxDelegate];
    _input = nil;
    _output = nil;

    _onOpened = nil;
    _onData = nil;
    _onClosed = nil;

    [self _abort:nil];

    return detachment;
}

- (void)noMoreData:(ALSocketOnNoMoreData)inOnCompletion {
    [_backlog addObject:[NSNull null]];
    _onNoMoreData = [inOnCompletion copy];
    [self _sendMore];
}

- (void)onDidEndTransmission {
}

- (NSError *)onWillOpenInput:(NSInputStream *__autoreleasing *)outInputStream output:(NSOutputStream *__autoreleasing *)outOutputStream {
    NSAssert(outInputStream, @"nil NSInputStream storage");
    NSAssert(outOutputStream, @"nil NSOutputStream storage");
    *outInputStream = _input;
    *outOutputStream = _output;

    return nil;
}

- (void)open:(ALSocketOnOpened)inOnOpened onData:(ALSocketOnReceivedData)inOnReceivedData onClosed:(ALSocketOnClosed)inOnClosed {
    NSAssert(inOnOpened, @"nil ALSocketOnOpened method");
    NSAssert(inOnClosed, @"nil ALSocketOnClosed method");

    _onOpened = [inOnOpened copy];
    _onData = [inOnReceivedData copy];
    _onClosed = [inOnClosed copy];

    NSInputStream *__autoreleasing input;
    NSOutputStream *__autoreleasing output;
    NSError *error = [self onWillOpenInput:&input output:&output];
    if (error) {
        [self _connectionDown:error];
        return;
    }

    _input = input;
    _output = output;

    NSStreamStatus rxStatus = [_input streamStatus];
    NSStreamStatus txStatus = [_output streamStatus];
    _intercepting = (rxStatus != NSStreamStatusNotOpen || txStatus != NSStreamStatusNotOpen);
    if (_intercepting) {
        _suspendedRxDelegate = [_input delegate];
        _suspendedTxDelegate = [_output delegate];
    } else {
        [_input scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        [_output scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    }

    [_input setDelegate:self];
    [_output setDelegate:self];

    [self _onRxUnexpected:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotRxUnexpectedEvent:inEventCode onStream:(NSInputStream *)inStream];
    }];
    _onRxUp = _onRxUnexpected;
    _onRxDown = _onRxUnexpected;
    _onRxData = _onRxUnexpected;
    _onRxEnd = _onRxUnexpected;

    [self _onTxUnexpected:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotTxUnexpectedEvent:inEventCode onStream:(NSOutputStream *)inStream];
    }];
    _onTxUp = _onTxUnexpected;
    _onTxDown = _onTxUnexpected;
    _onTxBuffer = _onTxUnexpected;
    _onTxClose = _onTxUnexpected;

    [self _connect];
}

- (NSInteger)send:(NSData *)inData onVerdict:(ALSocketOnSendVerdict)inOnVerdict {
    [_backlog addObject:inData];
    [self _onSendVerdict:inOnVerdict];
    _backlogSize += [inData length];

    if ([_output hasSpaceAvailable]) {
        [self _sendMore:_output];
    }

    return (_backlogSize - _sent);
}

#pragma mark -

- (void)_abort:(NSError *)inWhy {
    NSDictionary *errorInfo = inWhy ? @{ @"why" : inWhy } : nil;
    NSError *abortError = [NSError ALError:kALError_Aborted userInfo:errorInfo];
    NSError *sendError = inWhy;

    while ([_backlog count] > 0) {
        [self _sendDone:sendError];
        sendError = abortError;
    }
}

- (void)_closeRx {
    [self _closeRx:_input];
}

- (void)_closeRx:(NSInputStream *)inStream {
    // If the socket intercepted the stream, then just detach by restoring any suspended
    // delegate without closing the stream:
    if (_intercepting) {
        [inStream setDelegate:_suspendedRxDelegate];
        _suspendedRxDelegate = nil;
    } else {
        [self _closeStream:inStream];
    }
    _onRxData = _onRxUnexpected;
    _onRxEnd = _onRxUnexpected;
    _input = nil;
}

- (void)_closeStream:(NSStream *)inStream {
    AL_LOG_SOCKET(@"%@", inStream);
    [inStream close];
    [inStream setDelegate:nil];
}

- (void)_closeTx {
    [self _closeTx:_output];
}

- (void)_closeTx:(NSOutputStream *)inStream {
    [self _onRxEnd:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotRxEnd:(NSInputStream *)inStream];
    }];

    // If the socket intercepted the stream, then just detach by restoring any suspended
    // delegate without closing the stream:
    if (_intercepting) {
        [inStream setDelegate:_suspendedTxDelegate];
        _suspendedTxDelegate = nil;
    } else {
        [self _closeStream:inStream];
    }
    _onTxBuffer = _onTxUnexpected;
    _onTxClose = _onTxUnexpected;
    _output = nil;
    [self onDidEndTransmission];
}

- (void)_connect {
    AL_LOG_SOCKET(@"");
    _onSendVerdicts = [NSMutableArray new];
    _backlog = [NSMutableArray new];
    [self _onRxUp:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotRxUp_WhileRxDownTxDown];
    }];
    [self _onTxUp:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotTxUp_WhileRxDownTxDown];
    }];
    [self _onTxBuffer:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotTxBuffer_WhileTxDown:(NSOutputStream *)inStream eventCode:inEventCode];
    }];

    NSStreamStatus status = [_input streamStatus];
    if (status == NSStreamStatusNotOpen) {
        [_input open];
    } else {
        AL_LOG_SOCKET(@"ATTACHING to open stream %@", _input);
        if (status != NSStreamStatusOpening) {
            [self stream:_input handleEvent:NSStreamEventOpenCompleted];
        }
    }
}

- (void)_connectionDown:(NSError *)inError {
    AL_LOG_SOCKET(@"");
    [self setError:inError];
    if (_onClosed) {
        BOOL wasOpened = (_onOpened == nil);
        _onClosed(inError, wasOpened);
        _onOpened = nil;
        _onClosed = nil;
    }
}

- (void)_connectionUp {
    AL_LOG_SOCKET(@"");
    if (_onOpened) {
        BOOL wasInitiallyClosed = !_intercepting;
        _onOpened(wasInitiallyClosed);
        _onOpened = nil;

        if ([_output hasSpaceAvailable]) {
            [self _sendMore];
        }
    }
}

- (void)_gotInputEvent:(NSStreamEvent)inEventCode {
    NSInputStream *stream = _input;
    if (_onRxUp && (inEventCode & NSStreamEventOpenCompleted)) {
        _onRxUp(self, stream, inEventCode);
    }

    if (_onRxData && (inEventCode & NSStreamEventHasBytesAvailable)) {
        _onRxData(self, stream, inEventCode);
    }

    if (_onRxEnd && (inEventCode & NSStreamEventEndEncountered)) {
        _onRxEnd(self, stream, inEventCode);
    }

    if (_onRxDown && (inEventCode & NSStreamEventErrorOccurred)) {
        _onRxDown(self, stream, inEventCode);
    }

    const NSStreamEvent kUnexpected = ~(
        NSStreamEventEndEncountered |
        NSStreamEventErrorOccurred |
        NSStreamEventHasBytesAvailable |
        NSStreamEventOpenCompleted |
        0);
    if (_onRxUnexpected && (inEventCode & kUnexpected)) {
        _onRxUnexpected(self, stream, inEventCode);
    }
}

- (void)_gotOutputEvent:(NSStreamEvent)inEventCode {
    NSOutputStream *stream = _output;
    if (_onTxUp && (inEventCode & NSStreamEventOpenCompleted)) {
        _onTxUp(self, stream, inEventCode);
    }

    if (_onTxBuffer && (inEventCode & NSStreamEventHasSpaceAvailable)) {
        _onTxBuffer(self, stream, inEventCode);
    }

    if (_onTxClose && (inEventCode & NSStreamEventEndEncountered)) {
        _onTxClose(self, stream, inEventCode);
    }

    if (_onTxDown && (inEventCode & NSStreamEventErrorOccurred)) {
        _onTxDown(self, stream, inEventCode);
    }

    const NSStreamEvent kUnexpected = ~(
        NSStreamEventEndEncountered |
        NSStreamEventErrorOccurred |
        NSStreamEventHasSpaceAvailable |
        NSStreamEventOpenCompleted |
        0);
    if (_onTxUnexpected && (inEventCode & kUnexpected)) {
        _onTxUnexpected(self, stream, inEventCode);
    }
}

#pragma mark -

- (void)_gotRxData:(NSInputStream *)inStream {
    AL_LOG_SOCKET(@"");
    if (!_onData) {
        return;
    }
    uint8_t *buffer = NULL;
    NSUInteger size = 0;
    if ([inStream getBuffer:&buffer length:&size]) {
        NSData *asNSData = [NSData dataWithBytesNoCopy:buffer length:size freeWhenDone:NO];
        _onData(asNSData);
    } else {
        uint8_t rawBuffer[64];
        NSInteger got = 0;
        do {
            got = [inStream read:rawBuffer maxLength:sizeof(rawBuffer)];
            if (got > 0) {
                _onData([NSData dataWithBytesNoCopy:rawBuffer length:got freeWhenDone:NO]);
            }
        } while (got > 0 && [inStream hasBytesAvailable]);
    }
}

- (void)_gotRxEnd:(NSInputStream *)inStream {
    AL_LOG_SOCKET(@"");
    [self _closeRx:inStream];
    [self _connectionDown:nil];
}

- (void)_gotRxUnexpectedEvent:(NSStreamEvent)inEventCode onStream:(NSInputStream *)inStream {
    AL_LOG_SOCKET(@"");
    [self _closeRx:inStream];
    [self _gotUnexpectedEvent:inEventCode onStream:inStream];
}

- (void)_gotRxUp_WhileRxDown {
    AL_LOG_SOCKET(@"");
    _onRxUp = _onRxUnexpected;
    [self _onRxData:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotRxData:(NSInputStream *)inStream];
    }];
}

- (void)_gotRxUp_WhileRxDownTxDown {
    AL_LOG_SOCKET(@"");
    [self _gotRxUp_WhileRxDown];
    [self _onTxUp:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotTxUp_WhileRxUpTxDown];
    }];

    NSStreamStatus status = [_output streamStatus];
    if (status == NSStreamStatusNotOpen) {
        [_output open];
    } else {
        AL_LOG_SOCKET(@"ATTACHING to open stream %@", _output);
        if (status != NSStreamStatusOpening) {
            [self stream:_output handleEvent:NSStreamEventOpenCompleted];
        }
    }
}

- (void)_gotRxUp_WhileRxDownTxUp {
    AL_LOG_SOCKET(@"");
    [self _gotRxUp_WhileRxDown];
    [self _connectionUp];
}

- (void)_gotTxBuffer:(NSOutputStream *)inStream {
    AL_LOG_SOCKET(@"");
    [self _sendMore:inStream];
}

- (void)_gotTxBuffer_WhileTxDown:(NSOutputStream *)inStream eventCode:(NSStreamEvent)inEventCode {
    AL_LOG_SOCKET(@"");
}

- (void)_gotTxUnexpectedEvent:(NSStreamEvent)inEventCode onStream:(NSOutputStream *)inStream {
    AL_LOG_SOCKET(@"");
    [self _closeTx:inStream];
    [self _gotUnexpectedEvent:inEventCode onStream:inStream];
}

- (void)_gotTxUp:(NSOutputStream *)inStream {
    AL_LOG_SOCKET(@"");
    _onTxUp = _onTxUnexpected;
    [self _onTxBuffer:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotTxBuffer:(NSOutputStream *)inStream];
    }];
    //	[self _connectionUp];
}

- (void)_gotTxUp_WhileRxDownTxDown {
    AL_LOG_SOCKET(@"");
    [self _gotTxUp_WhileTxDown];
    [self _onRxUp:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotRxUp_WhileRxDownTxUp];
    }];
}

- (void)_gotTxUp_WhileRxUpTxDown {
    AL_LOG_SOCKET(@"");
    [self _gotTxUp_WhileTxDown];
    [self _connectionUp];
}

- (void)_gotTxUp_WhileTxDown {
    AL_LOG_SOCKET(@"");
    _onTxUp = _onTxUnexpected;
    [self _onTxBuffer:^(ALSocket *inSelf, NSStream *inStream, NSStreamEvent inEventCode) {
        [inSelf _gotTxBuffer:(NSOutputStream *)inStream];
    }];
    //	[self _connectionUp];
}

- (void)_gotUnexpectedEvent:(NSStreamEvent)inEventCode onStream:(NSStream *)inStream {
    AL_LOG_SOCKET(@"");
    NSError *streamError = [inStream streamError];
    NSError *error = streamError ? streamError : [NSError ALError:kALError_BadConnection userInfo:@{
        @"NSStreamEvent" : @(inEventCode),
        @"NSStream" : inStream,
    }];
    [self _connectionDown:error];
}

#pragma mark -

- (_ALEventHandler)_onRxUp:(_ALEventHandler)inOnRxUp {
    _ALEventHandler oldMethod = _onRxUp;
    _onRxUp = [inOnRxUp copy];
    return oldMethod;
}

- (_ALEventHandler)_onRxDown:(_ALEventHandler)inOnRxDown {
    _ALEventHandler oldMethod = _onRxDown;
    _onRxDown = [inOnRxDown copy];
    return oldMethod;
}

- (_ALEventHandler)_onRxData:(_ALEventHandler)inOnRxData {
    _ALEventHandler oldMethod = _onRxData;
    _onRxData = [inOnRxData copy];
    return oldMethod;
}

- (_ALEventHandler)_onRxEnd:(_ALEventHandler)inOnRxEnd {
    _ALEventHandler oldMethod = _onRxEnd;
    _onRxEnd = [inOnRxEnd copy];
    return oldMethod;
}

- (_ALEventHandler)_onRxUnexpected:(_ALEventHandler)inOnRxUnexpected {
    _ALEventHandler oldMethod = _onRxUnexpected;
    _onRxUnexpected = [inOnRxUnexpected copy];
    return oldMethod;
}

#pragma mark -

- (void)_onSendVerdict:(ALSocketOnSendVerdict)inOnVerdict {
    if (!inOnVerdict) {
        [self _onSendVerdict:^(NSData *inData, NSInteger inSent, NSError *inError){
        }];
    } else {
        [_onSendVerdicts addObject:[inOnVerdict copy]];
    }
}

#pragma mark -

- (_ALEventHandler)_onTxUp:(_ALEventHandler)inOnTxUp {
    _ALEventHandler oldMethod = _onTxUp;
    _onTxUp = [inOnTxUp copy];
    return oldMethod;
}

- (_ALEventHandler)_onTxDown:(_ALEventHandler)inOnTxDown {
    _ALEventHandler oldMethod = _onTxDown;
    _onTxDown = [inOnTxDown copy];
    return oldMethod;
}

- (_ALEventHandler)_onTxBuffer:(_ALEventHandler)inOnTxBuffer {
    _ALEventHandler oldMethod = _onTxBuffer;
    _onTxBuffer = [inOnTxBuffer copy];
    return oldMethod;
}

- (_ALEventHandler)_onTxClose:(_ALEventHandler)inOnTxClose {
    _ALEventHandler oldMethod = _onTxClose;
    _onTxClose = [inOnTxClose copy];
    return oldMethod;
}

- (_ALEventHandler)_onTxUnexpected:(_ALEventHandler)inOnTxUnexpected {
    _ALEventHandler oldMethod = _onTxUnexpected;
    _onTxUnexpected = [inOnTxUnexpected copy];
    return oldMethod;
}

#pragma mark -

- (void)_sendDone:(NSError *)inError {
    NSData *data = [_backlog firstObject];
    ALSocketOnSendVerdict onVerdict = [_onSendVerdicts firstObject];
    NSInteger sent = _sent;
    [_backlog removeObjectAtIndex:0];
    [_onSendVerdicts removeObjectAtIndex:0];
    _backlogSize -= [data length];
    _sent = 0;
    onVerdict(data, sent, inError);
}

- (void)_sendMore {
    [self _sendMore:_output];
}

- (void)_sendMore:(NSOutputStream *)inStream {
    NSInteger haveSize = -1;
    NSInteger sentSize = -1;
    while (sentSize == haveSize && [inStream hasSpaceAvailable]) {
        NSData *sending = [_backlog firstObject];
        if (!sending) {
            return;
        }
        if (sending == (id)[NSNull null]) {
            AL_LOG_SOCKET(@"END OF DATA");
            [_backlog removeAllObjects];
            [self _closeTx];
            if (_onNoMoreData) {
                _onNoMoreData();
                _onNoMoreData = nil;
            }
            return;
        }
        const uint8_t *unsent = (const uint8_t *)[sending bytes] + _sent;
        haveSize = [sending length] - _sent;
        sentSize = [inStream write:unsent maxLength:haveSize];
        if (sentSize < 0) {
            NSError *error = [inStream streamError];
            [self _sendDone:error];
            [self setError:error];
        } else {
            NSAssert(sentSize <= haveSize, @"OVERRUN: Expected %d.  Got %d.", (int)haveSize, (int)sentSize);
            _sent += sentSize;
            if (sentSize == haveSize) {
                [self _sendDone:nil];
            }
        }
    }
}

#pragma mark - NSStreamDelegate protocol

- (void)stream:(NSStream *)inStream handleEvent:(NSStreamEvent)inEventCode {
    AL_LOG_SOCKET(@"%@ -> %d", inStream, (int)inEventCode);
    if ([inStream isEqual:_input]) {
        [self _gotInputEvent:inEventCode];
    } else {
        NSAssert([inStream isEqual:_output], @"Expected %@.  Got %@", _output, inStream);
        [self _gotOutputEvent:inEventCode];
    }

    // If the event caused the socket to detach from the stream, then its delegate will revert
    // to the previously suspended delegate, if any.  Forward the event to that delegate:
    id<NSStreamDelegate> delegate = [inStream delegate];
    if (delegate != self && [delegate respondsToSelector:_cmd]) {
        [delegate stream:inStream handleEvent:inEventCode];
    }
}

@end

#pragma mark -

@implementation _ALSocketDetachment

@synthesize inputStream = _inputStream;
@synthesize outputStream = _outputStream;

+ (instancetype)detachmentWithInput:(NSInputStream *)inInput output:(NSOutputStream *)inOutput {
    return [[self alloc] initWithInput:inInput output:inOutput];
}

- (instancetype)initWithInput:(NSInputStream *)inInput output:(NSOutputStream *)inOutput {
    if ((self = [super init])) {
        _inputStream = inInput;
        _outputStream = inOutput;
    }

    return self;
}

@end
