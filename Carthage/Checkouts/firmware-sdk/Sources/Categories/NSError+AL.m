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

#import "NSError+AL.h"

NSString *const kALErrorDomain = @"AutomaticLabsSDK";
NSString *const kALErrorDomainHTTP = @"HTTP";

@implementation NSError (AL)

+ (NSString*) ALErrorString:(ALErrorCode) inErrorCode {
	NSString* errorString = @"Unknown Error";
	
	switch( inErrorCode) {
    case kALError_None:
        errorString = @"No Error";
        break;

	case kALError_NewSessionFailed:
        errorString = @"New Session Failed";
        break;
        
    case kALError_BadConnection:
        errorString = @"Bad Connection";
        break;
        
    case kALError_BadResponse:
        errorString = @"Bad Response";
        break;
        
    case kALError_Aborted:
        errorString = @"Aborted";
        break;
    
    case kALError_Unauthorized:
        errorString = @"Unauthorized";
        break;
        
    case kALError_Unsynced:
        errorString = @"Unsynced";
        break;
        
    case kALError_Conflict:
        errorString = @"Conflict";
        break;
        
    case kALError_Timeout:
        errorString = @"Timeout";
        break;
	}

	return errorString;
}

+ (instancetype)ALError:(ALErrorCode)inErrorCode userInfo:(NSDictionary *)inInfo {
    return [[self class] errorWithDomain:kALErrorDomain code:inErrorCode userInfo:inInfo];
}

@end
