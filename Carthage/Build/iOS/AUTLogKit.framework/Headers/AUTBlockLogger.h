//
//  AUTBlockLogger.h
//  AUTLogKit
//
//  Created by Sylvain Rebaud on 7/15/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import UIKit;
@import CocoaLumberjack;

NS_ASSUME_NONNULL_BEGIN

/// A block receiving the formatted log message and the original message
typedef void (^LogBlock)(NSString *formattedMessage, DDLogMessage *originalMessage);

/// A logger that invokes block whenever it logs.
@interface AUTBlockLogger : DDAbstractLogger <DDLogger>

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithLogBlock:(LogBlock)logBlock;

/// Initializes a logger that invokes the logBlock when it logs a message with
/// the contents of the message. Dispatches the block asynchronously on the
/// queue if one specified otherwise calls the block from the internal
/// CocoaLumberjack queue directly.
- (instancetype)initWithLogBlock:(LogBlock)logBlock queue:(nullable dispatch_queue_t)queue NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END
