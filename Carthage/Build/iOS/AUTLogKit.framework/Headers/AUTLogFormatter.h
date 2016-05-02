//
//  AUTLogFormatter.h
//  Automatic
//
//  Created by Sylvain Rebaud on 7/15/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Foundation;
@import CocoaLumberjack;

NS_ASSUME_NONNULL_BEGIN

// Formatting options depending on destination
// Client formatting is typical for an ASL or TTY logger.
typedef NS_ENUM(NSUInteger, AUTLogFormatterOutputOptions) {
    AUTLogFormatterOutputOptionsServer,
    AUTLogFormatterOutputOptionsClient
};

// A custom log formatter that can filter out logs coming from specific
// contexts. Additionally, it formats the log message differently depending
// on the AUTLogFormatterOutputOptions option.
@interface AUTLogFormatter : NSObject <DDLogFormatter>

- (instancetype)initWithOptions:(AUTLogFormatterOutputOptions)options;

- (instancetype)initWithOptions:(AUTLogFormatterOutputOptions)options includingLevelsByContextID:(nullable NSDictionary<NSNumber *, NSNumber *> *)levelsByContextID;

- (instancetype)initWithDateFormatter:(nullable NSDateFormatter *)formatter options:(AUTLogFormatterOutputOptions)options;

- (instancetype)initWithDateFormatter:(nullable NSDateFormatter *)formatter options:(AUTLogFormatterOutputOptions)options includingLevelsByContextID:(nullable NSDictionary<NSNumber *, NSNumber *> *)levelsByContextID NS_DESIGNATED_INITIALIZER;

/// A dictionary of [NSNumber<AUTLogContextIdentifier>: NSNumber<AUTLogLevel>]
/// specifying the highest level for each context that should be logged by this
/// formatter. All logs in a context that do not reach this level will be
/// excluded.
///
/// If nil, no logs will be filtered.
@property (readonly, nonatomic, copy, nullable) NSDictionary<NSNumber *, NSNumber *> *includedLevelsByContextID;

@end

NS_ASSUME_NONNULL_END
