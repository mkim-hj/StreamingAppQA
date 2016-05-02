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

#import "ALAuthorization.h"
#import "ALAuthorizationJob.h"
#import "ALAutomaticAdapter.h"
#import "NSData+AL.h"
#import <CommonCrypto/CommonDigest.h>
#import <UIKit/UIKit.h>

#ifndef AL_LOG_AUTHORIZATION
#if 0
#define AL_LOG_AUTHORIZATION AL_LOG
#else
#define AL_LOG_AUTHORIZATION(...)
#endif
#endif

@implementation ALAuthorization {
    NSString *_adapterID;
    NSDictionary *_secretKey_cached;
}

#pragma mark - Lifecycle

+ (instancetype)authorizationForAdapter:(ALAutomaticAdapter *)inAdapter {
    return [[self alloc] initWithAdapter:inAdapter];
}

- (instancetype)initWithAdapter:(ALAutomaticAdapter *)inAdapter {
    if ((self = [super init])) {
        _adapterID = [[inAdapter accessory] serialNumber];
    }

    return self;
}

#pragma mark - Property accessors

- (BOOL)exists {
    NSDictionary *secretKey = [self _secretKey];
    return (secretKey != nil);
}

#pragma mark - ALAuthorization methods

- (BOOL)askForAuthorizationForClient:(NSString *)inAutomaticClientID {
    NSDictionary *stateDict = @{
        @"class" : @"ALAuthorization",
        @"deviceID" : _adapterID,
    };
    NSData *stateData = [NSKeyedArchiver archivedDataWithRootObject:stateDict];
    NSString *stateUnescapedBase64 = [stateData base64EncodedStringWithOptions:0];
    NSString *stateEscapedBase64 = [stateUnescapedBase64 stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet alphanumericCharacterSet]];
    NSString *urlStr = [NSString stringWithFormat:
                                     @"https://accounts.automatic.com/oauth/authorize/?"
                                     @"client_id=%@&"
                                     @"response_type=code&"
                                     @"scope=scope:adapter:basic%%20scope:public&"
                                     @"state=%@",
                                     inAutomaticClientID,
                                     stateEscapedBase64];
    NSURL *url = [NSURL URLWithString:urlStr];
    AL_LOG_AUTHORIZATION(@"OPENING AUTHORIZATION FORM %@", url);
    return [UIApplication.sharedApplication openURL:url];
}

- (NSError *)discard {
    NSDictionary *secretKey = [self _secretKey];
    if (!secretKey) {
        return nil;
    }

    NSDictionary *defunctKey = [secretKey dictionaryWithValuesForKeys:@[
        (__bridge NSString *)kSecClass,
        (__bridge NSString *)kSecAttrService,
    ]];
    OSStatus secStatus = SecItemDelete((__bridge CFDictionaryRef)defunctKey);
    if (secStatus == noErr) {
        return nil;
    }

    return [NSError errorWithDomain:NSOSStatusErrorDomain code:secStatus userInfo:defunctKey];
}

- (BOOL)installAuthorizationWithOAuthToken:(NSString *)inToken tokenType:(NSString *)inTokenType onStatus:(ALAuthorizationOnStatus)inOnStatus onAuthorized:(ALAuthorizationOnAuthorized)inOnAuthorized {
    ALAuthorizationJob *job = [ALAuthorizationJob jobForAdapter:_adapterID];
    if (!job) {
        return NO;
    }

    [job runWithOAuthToken:inToken tokenType:inTokenType onStatus:inOnStatus onAuthorized:inOnAuthorized];

    return YES;
}

+ (BOOL)installAuthorizationFromURL:(NSURL *)inAccountAuthRedirectURL withSecret:(NSString *)inClientSecret onStatus:(ALAuthorizationOnStatus)inOnStatus onAuthorized:(ALAuthorizationOnAuthorized)inOnAuthorized {
    ALAuthorizationJob *job = [ALAuthorizationJob jobForURL:inAccountAuthRedirectURL];
    if (!job) {
        return NO;
    }

    [job runWithSecret:inClientSecret onStatus:inOnStatus onAuthorized:inOnAuthorized];

    return YES;
}

- (NSString *)proofWithChallenge:(NSString *)inChallenge {
    CC_SHA1_CTX sha1Context;
    CC_SHA1_Init(&sha1Context);

    NSData *challengeData = [NSData dataWithHex:inChallenge];
    CC_SHA1_Update(&sha1Context, [challengeData bytes], (CC_LONG)([challengeData length]));

    NSDictionary *secretKey = [self _secretKey];
    NSData *keyData = secretKey[(__bridge NSString *)kSecValueData];
    CC_SHA1_Update(&sha1Context, [keyData bytes], (CC_LONG)([keyData length]));

    uint32_t digest[CC_SHA1_DIGEST_LENGTH >> 2];
    CC_SHA1_Final((uint8_t *)((void *)digest), &sha1Context);

    NSString *response = [NSString stringWithFormat:@"%08x%08x%08x%08x%08x",
                                                    htonl(digest[0]),
                                                    htonl(digest[1]),
                                                    htonl(digest[2]),
                                                    htonl(digest[3]),
                                                    htonl(digest[4])];
    AL_LOG_AUTHORIZATION(@"SHA1(%@ || %@) -> %@ (%@)",
                         challengeData,
                         keyData,
                         [NSData dataWithBytesNoCopy:digest length:sizeof(digest) freeWhenDone:NO],
                         response);

    return response;
}

- (NSError *)saveSecret:(NSData *)inSecret {
    NSDictionary *keyDict = @{
        (__bridge NSString *)kSecClass : (__bridge NSString *)kSecClassGenericPassword,
        (__bridge NSString *)kSecAttrService : [NSString stringWithFormat:@"com.automatic.adapter.%@", _adapterID],
        (__bridge NSString *)kSecValueData : inSecret,
    };
    CFTypeRef newKey = NULL;
    OSStatus secStatus = SecItemAdd((__bridge CFDictionaryRef)keyDict, &newKey);
    if (secStatus == noErr) {
        return nil;
    }

    return [NSError errorWithDomain:NSOSStatusErrorDomain code:secStatus userInfo:keyDict];
}

+ (BOOL)waitForAuthorizationForAdapter:(ALAutomaticAdapter *)inAdapter onVerdict:(ALAuthorizationOnWaitVerdict)inOnVerdict {
    ALAuthorizationOnWaitVerdict onVerdict = [inOnVerdict copy];
    ALAuthorization *authorization = [self authorizationForAdapter:inAdapter];
    BOOL waiting = [ALAuthorizationJob waitIfAuthorizingAdapter:inAdapter onVerdict:^(ALAuthStatus inAuthStatus, NSError *inError) {
        if (onVerdict) {
            onVerdict(inAuthStatus, (inError ? nil : authorization), inError);
        }
    }];

    if (!waiting) {
        ALAuthStatus status = [authorization exists] ? kALAuthStatus_AdapterAccessPrivileges_Installed : kALAuthStatus_UserInteractionRequired;
        inOnVerdict(status, authorization, nil);
    }

    return waiting;
}

#pragma mark -

- (NSDictionary *)_secretKey {
    if (!_secretKey_cached) {
        NSMutableDictionary *wantItem = [NSMutableDictionary dictionaryWithDictionary:@{
            (__bridge NSString *)kSecClass : (__bridge NSString *)kSecClassGenericPassword,
            (__bridge NSString *)kSecAttrService : [NSString stringWithFormat:@"com.automatic.adapter.%@", _adapterID],
            (__bridge NSString *)kSecMatchCaseInsensitive : @YES,
            (__bridge NSString *)kSecReturnAttributes : @YES,
        }];
        CFTypeRef haveItem = NULL;
        OSStatus secStatus = SecItemCopyMatching((__bridge CFDictionaryRef)wantItem, &haveItem);
        if (secStatus != noErr) {
            return nil;
        }

        NSMutableDictionary *keyDict = [NSMutableDictionary dictionaryWithDictionary:(__bridge NSDictionary *)haveItem];
        CFRelease(haveItem);
        haveItem = NULL;

        [wantItem removeObjectForKey:(__bridge NSString *)kSecReturnAttributes];
        wantItem[(__bridge NSString *)kSecReturnData] = @YES;
        secStatus = SecItemCopyMatching((__bridge CFDictionaryRef)wantItem, &haveItem);
        if (secStatus != noErr) {
            return nil;
        }

        keyDict[(__bridge NSString *)kSecValueData] = (NSData *)(CFBridgingRelease(haveItem));
        [keyDict addEntriesFromDictionary:[wantItem dictionaryWithValuesForKeys:@[
                     (__bridge NSString *)kSecClass,
                 ]]];

        _secretKey_cached = keyDict;
    }

    return _secretKey_cached;
}

@end
