//
//  MTLModel+AUTValidateNonnull.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 11/10/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>

NS_ASSUME_NONNULL_BEGIN

@interface MTLModel (AUTValidateNonnull)

/// Validates that the provided value is non-null.
///
/// For use with the KVC validation APIs.
///
/// @param value The value that should be validated.
///
/// @param error A pass-by-reference error populated if the value is null with
///        an error in the AUTClientErrorDomain domain and with the
///        AUTClientErrorValidationFailed code.
///
/// @return YES if the value is non-null, NO otherwise.
- (BOOL)aut_validateValueIsNonnull:(id _Nullable *)value forKey:(NSString *)key error:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
