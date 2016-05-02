//
//  NSData+Hexadecimal.h
//  AUTBinaryTools
//
//  Created by Sylvain Rebaud on 5/13/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSData (Hexadecimal)

+ (nullable NSData*)aut_dataFromHexString:(nullable NSString *)string;

- (NSString *)aut_hexString;

@end

NS_ASSUME_NONNULL_END
