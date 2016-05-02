//
//  TestViewController.m
//  Oregon
//
//  Created by Maruchi Kim on 10/28/15.
//  Copyright © 2015 Maruchi Kim. All rights reserved.
//

#import <CoreBluetooth/CoreBluetooth.h>

#import "TestViewController.h"
#import "AutomaticAdapter/AutomaticAdapter.h"
#import "DashboardViewController.h"
#import "AppDelegate.h"
#import "Reachability.h"
#import <AudioToolbox/AudioServices.h>

#define captionSize ([[UIScreen mainScreen] bounds].size.height * .025)
#define labelSize ([[UIScreen mainScreen] bounds].size.height * 0.03)
#define titleSize ([[UIScreen mainScreen] bounds].size.height * .04)

@interface TestViewController () <EAAccessoryDelegate>
@property (weak, nonatomic) IBOutlet UIButton *nextButton;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *activityIndicatorView;
@property (weak, nonatomic) IBOutlet UIButton *exitButton;
@property (strong, nonatomic) CBCentralManager *bluetoothManager;
@property (weak, nonatomic) IBOutlet UILabel *counterLabel;
@end

@implementation TestViewController

Reachability* internetReachable;
NSTimer * myTimer;
int tryCount = 0;
int testCounter = 0;
bool testCompleted = false;
int receivedCount = 0;
uint32_t minimumFirmwareVersion = 0;

- (void) displayAlertWithTitle:(NSString *) title withMessage:(NSString *) message {
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:title
                                                                   message:message
                                                            preferredStyle:UIAlertControllerStyleAlert];

    UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK"
                                                            style:UIAlertActionStyleDefault
                                                          handler:^(UIAlertAction * action) {
                                                              [self exitPressed:self];
                                                          }];

    [alert addAction:defaultAction];
    [self presentViewController:alert animated:YES completion:nil];
}

- (void)accessoryDidDisconnect:(EAAccessory *)accessory {
    [self displayAlertWithTitle:@"Lost Bluetooth Connection" withMessage:@"It looks like the bluetooth connection was lost after acquisition. Unfortunately, you'll have to restart the test"];
}

- (void) detectInternetWithNotification:(NSNotification *) notification {
    [self detectInternet];
}

- (void)detectInternet {
    NetworkStatus internetStatus = [internetReachable currentReachabilityStatus];
    NSString *stateString = nil;
    bool presentInternetOffAlert = false;
    switch (internetStatus) {
        case NotReachable:
        {
            stateString = @"The internet is down. This is required for this test to ensure the results from this test are sent to Oregon DEQ.";
            presentInternetOffAlert = true;
            break;
        }
        case ReachableViaWiFi:
        {
            stateString = @"The internet is working via WIFI.";
            break;
        }
        case ReachableViaWWAN:
        {
            stateString = @"The internet is working via WWAN.";
            break;
        }
    }

    if (presentInternetOffAlert) {
        [self displayAlertWithTitle:@"Internet Connectivity Error" withMessage:stateString];
    }
}

- (void)detectBluetooth {
    if(!self.bluetoothManager)
    {
        // Put on main queue so we can call UIAlertView from delegate callbacks.
        self.bluetoothManager = [[CBCentralManager alloc] initWithDelegate:self queue:dispatch_get_main_queue()];
    }
    [self centralManagerDidUpdateState:self.bluetoothManager]; // Show initial state
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    NSString *stateString = nil;
    bool presentBluetoothOffAlert = false;
    switch(_bluetoothManager.state) {
        case CBCentralManagerStateResetting:
            stateString = @"The connection with the system service was momentarily lost, update imminent.";
            NSLog(@"%@", stateString);
            break;
        case CBCentralManagerStateUnsupported:
            stateString = @"The platform doesn't support Bluetooth Low Energy.";
            NSLog(@"%@", stateString);
            break;
        case CBCentralManagerStateUnauthorized:
            stateString = @"The app is not authorized to use Bluetooth Low Energy.";
            NSLog(@"%@", stateString);
            break;
        case CBCentralManagerStatePoweredOff:
            stateString = @"Bluetooth is currently powered off. Please turn bluetooth on, and restart the smog check.";
            presentBluetoothOffAlert = true;
            break;
        case CBCentralManagerStatePoweredOn:
            stateString = @"Bluetooth is currently powered on and available to use.";
            [self dismissViewControllerAnimated:YES completion:nil];
            break;
        default:
            stateString = @"State unknown, update imminent.";
            break;
    }

    if (presentBluetoothOffAlert) {
        [self displayAlertWithTitle:@"Bluetooth Error" withMessage:stateString];
    }
}

static NSArray* getAttachedDevices() {
    EAAccessoryManager* accessoryManager = [EAAccessoryManager sharedAccessoryManager];
    if (accessoryManager) {
        return [accessoryManager connectedAccessories];
    }
    return NULL;
}

- (void) openAdapterSession:(ALAutomaticAdapter *) myAutomaticAdapter {
    TestViewController * __weak weakSelf = self;

    [myAutomaticAdapter
     openAuthorizedSessionForAutomaticClient:@"1544288664bb7debe2de"
     allowingUserInteraction:NO
     onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
         // Monitor authentication status and handle any errors here...
         NSLog(@"onStatus: %d, ERROR: %@", inAuthStatus, inError);

         if ((inError.code == kALError_Timeout) && ([inError.domain isEqualToString:kALErrorDomain])) {
             NSLog(@"TIMEOUT! %ld", (long) inError.code);
             [weakSelf performSelector:_cmd withObject:myAutomaticAdapter afterDelay:.5];
             return;
         }

         if ((inAuthStatus == kALAuthStatus_UserInteractionRequired) && (inError != nil)) {
             AppDelegate * delegate = [[UIApplication sharedApplication] delegate];
             [myAutomaticAdapter authorizeWithOAuthToken:delegate.client.credential.accessToken tokenType:delegate.client.credential.tokenType onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
                 NSLog(@"authorizeWithOAuthToken onStatus: %d, ERROR: %@", inAuthStatus, inError);
                 
                  if (inError.code == kALError_Unauthorized && inAuthStatus == 2) {
                      [weakSelf displayAlertWithTitle:@"Unauthorization Error" withMessage:@"Please log back in and try again. If this issue persists, please contact smog@automatic.com"];
                      return;
                  }
             } onAuthorized:^(ALAutomaticAdapter *inAdapter, NSString *inClientID, ALAutomaticAdapterOnAuthorizationStatus inStatusCallback) {
                 NSLog(@"Authorized, will attempt to open session.");
                 [weakSelf performSelectorOnMainThread:_cmd withObject:myAutomaticAdapter waitUntilDone:NO];
             }];
         }
     }
     onAuthorized:^(ALELM327Session *inSession, NSString *inGreeting) {
         // This block is called if authentication succeeds. The ALELM327Session can now
         // be used to communicate with the adapter and access engine data.  For example:
         NSLog(@"Succeeded");
         _currentSession = inSession;
         [_currentSession.adapter.accessory setDelegate:weakSelf];
         [myTimer invalidate];
         [self pollTheThing:@selector(getIgnition:) withInterval:2];
     }
     onClosed:^(NSError *inError) {
         // This block is called when the session ends (unless it was detached as
         // shown below)
         NSLog(@"Session Ended");
     }];
}

- (void) getELMSession:(NSTimer *) timer {
    if (!_currentSession) {
        NSArray *deviceFirmwareArray;
        ALAutomaticAdapter * myAutomaticAdapter;
        NSArray* connectedDevices = getAttachedDevices();
        bool foundAutomatic = false;

        //TODO HANDLE CASE WHEN MORE THAN ONE LINK IS FOUND
        for (id device in connectedDevices) {
            myAutomaticAdapter = [device asAutomaticAdapter];
            if (myAutomaticAdapter) {
                foundAutomatic = true;
                deviceFirmwareArray = [[myAutomaticAdapter.accessory firmwareRevision] componentsSeparatedByString:@"."];
                if ( (([deviceFirmwareArray[0] intValue] << 24) +
                      ([deviceFirmwareArray[1] intValue] << 16) +
                      ([deviceFirmwareArray[2] intValue]) ) >= minimumFirmwareVersion) {
                    // Greater than minimum firmware version, then break
                    break;
                } else {
                    // Don't save the adapter as it's not supported via firmware
                    myAutomaticAdapter = nil;
                }
            }
        }

//        if (tryCount == 0) {
//            ALAuthorization *authorization = [ALAuthorization authorizationForAdapter:myAutomaticAdapter];
//            [authorization discard];
//        }

        if (foundAutomatic == false && !myAutomaticAdapter) {
            tryCount++;
            NSLog(@"Attempted: %i", tryCount);
        } else if (foundAutomatic == true && !myAutomaticAdapter) {
            //found Automatic but not correct firmware version
            [myTimer invalidate];
            [self displayAlertWithTitle:@"Firmware Error" withMessage:@"Unfortunately, your Automatic Adapter doesn't have the supported firmware to run a smog test"];
        } else {
            [myTimer invalidate];
            [self openAdapterSession:myAutomaticAdapter];
        }

    }
}

-(void) getIgnition: (NSTimer *) timer {
    TestViewController * __weak weakSelf = self;
    
    [_currentSession sendLine:@"01 00" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        // Check inError and use inResponse here...
        NSLog(@"Polling 0100");
        NSLog(@"%@", inResponse);
        NSLog(@"%@", inError);

        // If response NOT equal to NO DATA, means Link is in DRIVE MODE
        if (!([inResponse isEqual: @"NO DATA"])) {
            [myTimer invalidate]; // stop polling, link is active

            [_currentSession sendLine:@"ATE0" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
                NSLog(@"%@ -> %@", inCommand, inResponse);
            }];
            [_currentSession sendLine:@"ATL0" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
                NSLog(@"%@ -> %@", inCommand, inResponse);
            }];
            [_currentSession sendLine:@"ATS0" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
                NSLog(@"%@ -> %@", inCommand, inResponse);
            }];
            
            if ([self.testSubtitle.text isEqual:@"Scantool Mode ON"]) {
                NSLog(@"sending AL SCAN 1");
                [_currentSession sendLine:@"AL SCAN 1" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
                    NSLog(@"%@ -> %@", inCommand, inResponse);
                    [weakSelf runTest];
                }];
            } else {
                [weakSelf runTest];
            }
        }
    }];
}

-(void)runTest {
    // Kick off 30 second timer
    [self pollTheThing:@selector(flagTestCompleted:) withInterval:1];
    testCounter = 0;
    
    // RUN SPECIFIC TEST HERE
    if ([self.testTitle.text isEqual:@"Chained Test"]) {
        NSLog(@"Running Pids per Second Test");
        [self chainedTest:@(0)];
    } else if ([self.testTitle.text isEqual:@"Burst Test"]) {
        NSLog(@"Running Burst Test");
        [self burstTest:@(0)];
    } else if ([self.testTitle.text isEqual:@"Loopback Test"]) {
        NSLog(@"Running Loopback Test");
        [self loopbackTest:@"A"];
    }
}

-(void)flagTestCompleted:(NSTimer *) timer {
    testCounter++;
    
    if (testCounter >= 30) {
        testCompleted = true;
        [myTimer invalidate];
    }
}

-(void) chainedTest:(NSNumber*)count {
    TestViewController * __weak weakSelf = self;

    self.testLabel.text    = [NSString stringWithFormat:@"%d%% complete", (int) floor((testCounter / 30.0f) * 100)];
    self.testCaption.text  = [NSString stringWithFormat:@"%d Seconds Left", 30-testCounter];
    _counterLabel.text = [NSString stringWithFormat:@"%.2f", ([count floatValue] / (float) testCounter) ];
    
    if (testCompleted) {
        self.testCaption.text  = [NSString stringWithFormat:@"Test finished, %@ transactions received over 30 seconds.", count];
        return;
    }

    [_currentSession sendLine:_testCommand.text onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        [weakSelf performSelector:_cmd withObject:@([count intValue]+1) afterDelay:0];
    }];

}

-(void) burstTest:(NSNumber*)count {
    TestViewController * __weak weakSelf = self;
    
    for (int i = 0; i < 1000; i++) {
        [_currentSession sendLine:_testCommand.text onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
            weakSelf.testLabel.text    = [NSString stringWithFormat:@"%d%% complete", (int) floor((i / 999.0f) * 100)];
            weakSelf.testCaption.text  = [NSString stringWithFormat:@"%d transactions left to receive", 999 - i];
            _counterLabel.text = [NSString stringWithFormat:@"%.2f", (i / (float) testCounter) ];
            
            if (i >= 999) {
                weakSelf.testCaption.text  = [NSString stringWithFormat:@"Test finished, 1000 transactions received over %d seconds.", testCounter];
            }
        }];
    }
}

-(void) loopbackTest:(NSString *) string {
    TestViewController * __weak weakSelf = self;

    if ([string length] >= 512) {
        return;
    }
    
    [_currentSession sendLine:string onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
        NSLog(@"RESPONSE:%@ LENGTH:%lu", inResponse, (unsigned long) [string length]);
        [weakSelf performSelector:_cmd withObject:[string stringByAppendingString:@"A"] afterDelay:0];
    }];
}

- (void) pollTheThing:(SEL) function withInterval:(int) interval {
    myTimer = [NSTimer scheduledTimerWithTimeInterval:interval
                                               target:self
                                             selector:function
                                             userInfo:nil
                                              repeats:YES];

//    [myTimer fire];
}

- (IBAction)startTest:(id)sender {
    if ([_nextButton.currentTitle isEqual: @"Next"]) {
       

    } else {
        [self displayAlertWithTitle:@"Ignition Off" withMessage:@"Please check to make sure your car's ignition is on and try again."];
    }
}

- (IBAction)exitPressed:(id)sender {
    [myTimer invalidate];
    [(DashboardViewController *) self.parentViewController hideOverlay];
    [self.view removeFromSuperview];
    [self removeFromParentViewController];
}

- (void)drawButtonBorder:(UIButton *)button {
    [[button layer] setBorderWidth:.4f];
    [[button layer] setBorderColor:[UIColor colorWithRed:.906 green:0.902 blue:0.894 alpha:1].CGColor];
}

- (void)scaleFontSizes:(UIView *) view {
    for (UIView *subview in view.subviews)
    {
        [self scaleFontSizes:subview];
        if ([subview.restorationIdentifier isEqual:@"UITitleLabel"]) {
            [((UILabel *) subview) setFont: [((UILabel *) subview).font fontWithSize: titleSize]];
        } else if ([subview.restorationIdentifier isEqual:@"UILabelLabel"]) {
            [((UILabel *) subview) setFont: [((UILabel *) subview).font fontWithSize: labelSize]];
        } else if ([subview.restorationIdentifier isEqual:@"UICaptionLabel"]) {
            [((UILabel *) subview) setFont: [((UILabel *) subview).font fontWithSize: captionSize]];
        }
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self.activityIndicatorView startAnimating];

    [self drawButtonBorder:_nextButton];
    [self drawButtonBorder:_exitButton];

    [self scaleFontSizes:self.view];

    // For bluetooth off error handling
    [self detectBluetooth];
    
    self.testLabel.text    = @"0% Complete";
    self.testCaption.text  = @"30 Seconds Left";

    NSLog(@"%@", self);
}

- (void) viewWillDisappear:(BOOL)animated {
    tryCount = 0;
    testCompleted = false;
    [_currentSession.adapter.accessory setDelegate:nil];
    _currentSession = nil;
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kReachabilityChangedNotification object:nil];
}

- (void) viewDidAppear:(BOOL)animated {
    //Link with Streaming SDK!
    if (!_currentSession && [self.restorationIdentifier isEqual:@"TestViewController"]) {
        [self pollTheThing:@selector(getELMSession:) withInterval:1];
    }

    //For Internet error handling
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(detectInternetWithNotification:)
                                                 name:kReachabilityChangedNotification
                                               object:nil];

    internetReachable = [Reachability reachabilityForInternetConnection];
    [internetReachable startNotifier];
    [self detectInternet]; //initial status
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

@end
