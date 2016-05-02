//
//  AUTLog.h
//  AUTLogKit
//
//  Created by Sylvain Rebaud on 7/14/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Foundation;
@import CocoaLumberjack;

NS_ASSUME_NONNULL_BEGIN

typedef NSInteger AUTLogContextIdentifier;

typedef NS_OPTIONS(NSUInteger, AUTLogFlag) {
    AUTLogFlagError      = DDLogFlagError, // 0...00001
    AUTLogFlagInfo       = DDLogFlagInfo,  // 0...00100
};

typedef NS_OPTIONS(NSUInteger, AUTLogLevel) {
    AUTLogLevelOff   = 0,
    AUTLogLevelError = AUTLogFlagError,
    AUTLogLevelInfo  = (AUTLogLevelError | AUTLogFlagInfo),
    AUTLogLevelAll   = NSUIntegerMax,
};

/// A context to associate a group of related log statements.
///
/// For example, all logs related to network connections could be grouped into
/// a context.

@interface AUTLogContext : NSObject

/// A log level for this context.
@property (atomic, assign) AUTLogLevel level;

/// Human readable name for this context.
@property (readonly, atomic, copy) NSString *name;

/// A unique identifier representing this context.
@property (readonly, atomic, assign) AUTLogContextIdentifier identifier;

- (instancetype)init NS_UNAVAILABLE;

/// Create a context by name and level automatically registering it. The list
/// of registered contexts can be retrieved using `registeredContexts`.
- (instancetype)initWithName:(NSString *)name level:(AUTLogLevel)level NS_DESIGNATED_INITIALIZER;

/// Returns a list of previously registered contexts
+ (NSArray<AUTLogContext *> *)registeredContexts;

/// Returns a context given its identifier, nil if invalid.
+ (nullable AUTLogContext *)contextForIdentifier:(AUTLogContextIdentifier)identifier;

@end

/// Define version of the macro that only executes if the log level is above the threshold.
/// The compiled versions essentially look like this:
///
/// if (logFlagForThisLogMsg & logLevelForContext:XXX) { execute log message }
#define AUT_LOG_MAYBE(async, ctx, flg, tag, fnct, frmt, ...) \
    do { if(ctx.level & flg) LOG_MACRO(async, (DDLogLevel)ctx.level, (DDLogFlag)flg, ctx.identifier, tag, fnct, frmt, ##__VA_ARGS__); } while(0)

/// Ready to use context specific log macros.
///
/// Macros are asynchronous for all flags. This prevents a deadlock inside
/// CocoaLumberjack when a combination of synchronous vs asynchronous calls
/// are used. This is at the expense of potentially missing some critical logs
/// in the case of a crash.
#define AUTLogError(ctx, frmt, ...) AUT_LOG_MAYBE(YES, ctx, AUTLogFlagError, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define AUTLogInfo(ctx, frmt, ...)  AUT_LOG_MAYBE(YES, ctx, AUTLogFlagInfo, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)

/// A macro to define a given context in a header file as extern.
#define AUTLOGKIT_CONTEXT_DECLARE(ctx) extern AUTLogContext *ctx;

/// A macro to initialize a given context in an implementation file passing
/// a default level and a context name, automatically registering it at load
/// time.
#define AUTLOGKIT_CONTEXT_INIT(ctx, _level, name) \
    AUTLogContext *ctx; \
    @interface AUTLogContext ## ctx : AUTLogContext \
    @end \
    @implementation AUTLogContext ## ctx \
        + (void)load { \
            static dispatch_once_t onceToken; \
            dispatch_once(&onceToken, ^{ \
                ctx = [[AUTLogContext alloc] initWithName:@(name) level:_level]; \
            }); \
        } \
    @end

NS_ASSUME_NONNULL_END
