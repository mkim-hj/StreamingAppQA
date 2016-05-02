//
//  AUTLog_Private.h
//  AUTLogKit
//
//  Created by Sylvain Rebaud on 7/14/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Foundation;

NS_ASSUME_NONNULL_BEGIN

#define AUTLOGKIT_CONTEXT_CREATE(ctx, level) AUTLOGKIT_CONTEXT_CREATE_WITH_NAME(ctx, level, #ctx)

/// A macro to create and register a log context within the context of a method.
/// Only used for testing purposes.
#define AUTLOGKIT_CONTEXT_CREATE_WITH_NAME(_ctx, _level, _name) \
    AUTLogContext *_ctx = [[AUTLogContext alloc] initWithName:@(_name) level:_level];

@interface AUTLogContext (ResetContexts)

/// A method to reset list of previously registered contexts. Only used for
/// testing purposes.
+ (void)resetRegisteredContexts;

@end

NS_ASSUME_NONNULL_END
