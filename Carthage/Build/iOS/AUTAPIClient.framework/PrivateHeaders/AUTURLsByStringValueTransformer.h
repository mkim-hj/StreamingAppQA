//
//  AUTURLsByStringValueTransformer.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 11/4/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

/// Transforms an dictionary of [NSString: NSString] to a dictionary of
/// [NSString: NSURL].
@interface AUTURLsByStringValueTransformer : NSValueTransformer <MTLTransformerErrorHandling>

@end

NS_ASSUME_NONNULL_END
