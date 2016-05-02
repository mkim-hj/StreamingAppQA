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

#import "ALAdapterDetector.h"
#import "ALAppDelegate.h"
#import "ALELM327Session.h"
#import "ALRunViewController.h"

#define USE_ALAUTOMATICADAPTER_INTERFACE (1)

@interface ALRunViewController () <UITextFieldDelegate>

@property (weak, nonatomic) IBOutlet UITextField *pinField;

@end

#pragma mark -

static NSDictionary *_kAuthStatusNames;

@implementation ALRunViewController {
    @private
    ALAdapterDetector *_detector;
    ALELM327Session *_elm327Session;
    EASession *_eaSession;
}

- (void)initialize {
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        _kAuthStatusNames = @{
            @(kALAuthStatus_AdapterAccessPrivileges_Acquiring) : @"kALAuthStatus_AdapterAccessPrivileges_Acquiring",
            @(kALAuthStatus_AdapterAccessPrivileges_Installed) : @"kALAuthStatus_AdapterAccessPrivileges_Installed",
            @(kALAuthStatus_AdapterAccessPrivileges_Installing) : @"kALAuthStatus_AdapterAccessPrivileges_Installing",
            @(kALAuthStatus_AdapterSession_Authenticated) : @"kALAuthStatus_AdapterSession_Authenticated",
            @(kALAuthStatus_AdapterSession_Authenticating) : @"kALAuthStatus_AdapterSession_Authenticating",
            @(kALAuthStatus_AccountAccessPrivileges_Acquiring) : @"kALAuthStatus_AccountAccessPrivileges_Acquiring",
            @(kALAuthStatus_None) : @"kALAuthStatus_None",
            @(kALAuthStatus_UserInteractionRequired) : @"kALAuthStatus_UserInteractionRequired",
        };
    });
}

#pragma mark - IBActions

- (IBAction)onPINEntered:(UITextField *)inPINField {
    [self onRun];
}

- (IBAction)onRun {
    [self reset];

    ALRunViewController *__weak weakSelf = self;
    _detector = [ALAdapterDetector
		watchForPIN:[_pinField text]
		onDetected:^(NSArray *inAdapterAccessories) {
			[weakSelf gotAdapters:inAdapterAccessories];
		}];
}

#pragma mark - Property accessors

- (void)setAccessory:(EAAccessory *)inAccessory {
    [self reset];
    _accessory = inAccessory;

    ALRunViewController *__weak weakSelf = self;

    //
    // To open an EASession with access to engine data, do the following.  (See
    // also the example in -setAutomaticAdapter: for an alternative technique
    // that opens a high-level ALELM327Session interface instead.)
    //
    [_accessory
		openAuthorizedSessionForAutomaticClient:ALAppDelegate.automaticClientID
        allowingUserInteraction:YES
        onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
            NSLog(@"%@ %@", _kAuthStatusNames[@(inAuthStatus)], inError);
        }
        onAuthorized:^(EASession *inSession, NSString *inGreeting) {

            // The EASession can now be used to communicate with the adapter and access
            // engine data.  Access to engine data will persist until the data connection
            // to the adapter is closed.  Once closed, the
            // openAuthorizedSessionForAutomaticClient... method must be called again to
            // access engine data.  However, user interaction is normally only necessary
            // once ever per adapter, the first time that adapter is used.  Thereafter,
            // the credentials to access that adapter's engine data are saved on the
            // app's keychain and no more user interaction is needed to gain access.
            //
            // User interaction might (and currently does) switch to Safari, which could
            // cause the current app to quit.  The current app will be launched again (if
            // it isn't already running) to open an authorization URL if the user okays
            // access to the adapter. See the openURL handler in ALAppDelegate for an
            // example of how to process that URL.

            NSLog(@"AUTHORIZED %@", inSession);
            [weakSelf eaSessionUp:inSession withGreeting:inGreeting];
        }];
}

- (void)setAutomaticAdapter:(ALAutomaticAdapter *)inAutomaticAdapter {
    ALRunViewController *__weak weakSelf = self;
    [self reset];
    _automaticAdapter = inAutomaticAdapter;

    //
    // To open an ALELM327Session with access to engine data, do the following.
    // (See also the example in -setAccessory: for an alternative technique
    // that opens a low-level EASession interface instead.)
    //
    [_automaticAdapter
		openAuthorizedSessionForAutomaticClient:[ALAppDelegate automaticClientID]
		allowingUserInteraction:YES
        onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
            NSLog(@"%@ %@", _kAuthStatusNames[@(inAuthStatus)], inError);
        }
        onAuthorized:^(ALELM327Session *inSession, NSString *inGreeting) {
            // The ALELM327Session can now be used to communicate with the adapter and
            // access engine data.  Access to engine data will persist until the data
            // connection to the adapter is closed.  Once closed, the
            // openAuthorizedSessionForAutomaticClient... method must be called again to
            // access engine data.  However, user interaction is normally only necessary
            // once ever per adapter, the first time that adapter is used.  Thereafter,
            // the credentials to access that adapter's engine data are saved on the
            // app's keychain and no more user interaction is needed to gain access.
            //
            // User interaction might (and currently does) switch to Safari, which could
            // cause the current app to quit.  The current app will be launched again (if
            // it isn't already running) to open an authorization URL if the user okays
            // access to the adapter. See the openURL handler in ALAppDelegate for an
            // example of how to process that URL.

            NSLog(@"AUTHORIZED %@", inSession);
            [weakSelf elm327SessionUp:inSession withGreeting:inGreeting];
        }
        onClosed:^(NSError *inError) {
            NSLog(@"CLOSED %@", inError);
            [weakSelf sessionDown:inError];
        }];
}

#pragma mark - ViewController methods

- (void)eaSessionUp:(EASession *)inSession withGreeting:(NSString *)inGreeting {
    NSLog(@"'%@'", inGreeting);
    _eaSession = inSession;
    [[_eaSession outputStream] write:(const uint8_t *)"ATE1\r" maxLength:5];
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        char response[32];
        NSInteger len = [[_eaSession inputStream] read:(uint8_t *)response maxLength:sizeof(response) - 1];
        if (len >= 0) {
            response[len] = '\0';
            NSLog(@"GOT '%s'", response);
        } else {
            NSLog(@"READ FAILED");
        }
    });
}

- (void)elm327SessionUp:(ALELM327Session *)inSession withGreeting:(NSString *)inGreeting {
    NSLog(@"'%@'", inGreeting);
    _elm327Session = inSession;

    // Try AT commands
    [_elm327Session sendLine:@"ATE0" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"%@ -> %@", inCommand, inResponse);
    }];
    [_elm327Session sendLine:@"ATE1" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"%@ -> %@", inCommand, inResponse);
    }];
    [_elm327Session sendLine:@"ATZ" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"%@ -> %@", inCommand, inResponse);
    }];
    [_elm327Session sendLine:@"ATD" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"%@ -> %@", inCommand, inResponse);
    }];
    [_elm327Session sendLine:@"ATWS" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"%@ -> %@", inCommand, inResponse);
    }];
    [_elm327Session sendLine:@"ATL1" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"%@ -> %@", inCommand, inResponse);
    }];
    [_elm327Session sendLine:@"ATL0" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"%@ -> %@", inCommand, inResponse);
    }];
    [_elm327Session sendLine:@"ATI" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"%@ -> %@", inCommand, inResponse);
    }];
    [_elm327Session sendLine:@"ATSP0" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"%@ -> %@", inCommand, inResponse);
    }];
    [_elm327Session sendLine:@"ATS0" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"%@ -> %@", inCommand, inResponse);
    }];

    // Enumerate supported PIDs starting with Mode 1 PID 00
    ALRunViewController *__weak weakSelf = self;
    [_elm327Session sendLine:@"01 00"
                  onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
                      NSLog(@"%@ -> %@", inCommand, inResponse);
                      [weakSelf gotResponse:inResponse forMode:0x01 metaPID:0];
                  }];
}

- (void)gotAdapters:(NSArray *)inAdapters {
    NSLog(@"%@", inAdapters);
    ALAutomaticAdapter *adapter = [inAdapters firstObject];

#if USE_ALAUTOMATICADAPTER_INTERFACE
    [self setAutomaticAdapter:adapter];
#else
    [self setAccessory:[adapter accessory]];
#endif
}

- (void)gotResponse:(nullable NSString *)inResponse forMode:(uint8_t)inMode metaPID:(uint8_t)inMetaPID {
    if (inResponse == nil) {
        return;
    }

    // The response should be a bitmap of supported PIDs represented in hex.  Convert it to a native integer:
    NSString *hex =
        [[inResponse componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceCharacterSet]] componentsJoinedByString:@""];
    unsigned long bitmap = strtoul([hex UTF8String] + 4, NULL, 16);
    ALRunViewController *__weak weakSelf = self;

    // Send a request for each PID marked in the bitmap:
    for (uint8_t pid = (inMetaPID + 1); (pid & (0x20 - 1)); pid++) {
        if (bitmap & 0x80000000) {
            [_elm327Session sendLine:[NSString stringWithFormat:@"%02X %02X", inMode, pid] onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
                NSLog(@"%@ -> %@", inCommand, inResponse);
            }];
        }

        bitmap <<= 1;
    }

    // Request the next meta-PID if the bitmap indicates it is supported.  Otherwise, end the session.
    if (bitmap & 0x80000000) {
        uint8_t nextMetaPID = inMetaPID + 0x20;
        [_elm327Session sendLine:[NSString stringWithFormat:@"%02X %02X", inMode, nextMetaPID] onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
            NSLog(@"%@ -> %@", inCommand, inResponse);
            [weakSelf gotResponse:inResponse forMode:inMode metaPID:nextMetaPID];
        }];
    } else if (inMode < 0x0A) {
        uint8_t nextMode = inMode + 1;
        [_elm327Session sendLine:[NSString stringWithFormat:@"%02X 00", nextMode] onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
            NSLog(@"%@ -> %@", inCommand, inResponse);
            [weakSelf gotResponse:inResponse forMode:nextMode metaPID:0];
        }];
    } else {
        [_elm327Session noMoreCommands:nil];
    }
}

- (void)reset {
    _detector = nil;
    _elm327Session = nil;
    _eaSession = nil;
    _accessory = nil;
    _automaticAdapter = nil;
}

- (void)sessionDown:(NSError *)inWhy {
    NSLog(@"%@", inWhy);
    _elm327Session = nil;
}

#pragma mark - UIViewController overrides

- (void)viewWillAppear:(BOOL)inAnimated {
    [_pinField becomeFirstResponder];
}

- (void)viewWillDisappear:(BOOL)inAnimated {
    _elm327Session = nil;
    _eaSession = nil;
    _detector = nil;
}

#pragma mark - UITextFieldDelegate protocol

- (BOOL)textFieldShouldBeginEditing:(UITextField *)inTextField {
    return (_elm327Session == nil);
}

- (BOOL)textFieldShouldReturn:(UITextField *)inTextField {
    [inTextField resignFirstResponder];
    return YES;
}

@end
