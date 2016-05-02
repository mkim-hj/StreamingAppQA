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

#import "ALPINFormatter.h"

static NSCharacterSet *_validChars;
static NSCharacterSet *_invalidChars;

@implementation ALPINFormatter

+ (void)initialize {
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        _validChars = [NSCharacterSet characterSetWithCharactersInString:@"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"];
        _invalidChars = [_validChars invertedSet];
    });
}

+ (BOOL)isCompletePIN:(NSString *)inCandidatePIN {
    NSString *normalized = [[self singleton] stringForObjectValue:inCandidatePIN];
    NSUInteger length = [normalized length];
    return (length == 7);
}

+ (instancetype)singleton {
    static ALPINFormatter *sSingleton;
    if (!sSingleton) {
        sSingleton = [ALPINFormatter new];
    }
    return sSingleton;
}

- (NSString *)stringForObjectValue:(NSString *)inPIN {
    // @"-*([^-])-*([^-])-*([^-])(-?)"
    NSString *uppercase = [inPIN uppercaseString];
    NSArray *validRuns = [uppercase componentsSeparatedByCharactersInSet:_invalidChars];
    NSString *untruncted = [validRuns componentsJoinedByString:@""];
    NSUInteger length = MIN([untruncted length], 6);
    NSString *truncated = [untruncted substringToIndex:length];
    NSString *hyphenated = length > 3 ? [@[ [truncated substringToIndex:3], @"-", [truncated substringFromIndex:3] ] componentsJoinedByString:@""] : truncated;

    return hyphenated;
}

@end
