//
//  LoginViewController.m
//  Oregon
//
//  Created by Maruchi Kim on 10/22/15.
//  Copyright © 2015 Maruchi Kim. All rights reserved.
//

#import "LoginViewController.h"
#import "ExternalAccessory/ExternalAccessory.h"
#import "DashboardViewController.h"
#import "AppDelegate.h"

#define isIPhone4  ([[UIScreen mainScreen] bounds].size.height == 480)?TRUE:FALSE

@import AUTAPIClient;
@import AFNetworking;

@interface LoginViewController ()

@property (readwrite, nonatomic, strong) AUTClient *client;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *activityIndicatorView;
@property (weak, nonatomic) IBOutlet UIButton *loginButton;
@property (weak, nonatomic) IBOutlet UILabel *sloganLabel;
@property (weak, nonatomic) IBOutlet UILabel *loginLabel;
@property (weak, nonatomic) IBOutlet UILabel *titleLabel;
@end

@implementation LoginViewController

-(void)dismissKeyboard {
    [_passwordText resignFirstResponder];
    [_usernameText resignFirstResponder];
}

-(void)textFieldDidBeginEditing:(UITextField *)textField {
    [self animateTextField:textField up:YES];
}

- (void)textFieldDidEndEditing:(UITextField *)textField {
    [self animateTextField:textField up:NO];
}

- (void)animateTextField:(UITextField*)textField up:(BOOL)up {
    static bool displayed = false;
    
    if ((!displayed && up) || (!up && displayed)) {
        displayed = !displayed;
        const int movementDistance = -130; // tweak as needed
        const float movementDuration = 0.3f; // tweak as needed
        
        int movement = (up ? movementDistance : -movementDistance);
        
        [UIView beginAnimations: @"animateTextField" context: nil];
        [UIView setAnimationBeginsFromCurrentState: YES];
        [UIView setAnimationDuration: movementDuration];
        self.view.frame = CGRectOffset(self.view.frame, 0, movement);
        [UIView commitAnimations];
    }
}

- (void)formatTextBoxes:(UITextField*)textField {
    UIView *padding = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 10, 20)];
    [textField setBorderStyle:UITextBorderStyleNone];
    textField.layer.borderWidth = .3;
    textField.layer.borderColor = [UIColor colorWithRed:0.565 green:0.608 blue:0.643 alpha:.5].CGColor;
    textField.leftViewMode = UITextFieldViewModeAlways;
    textField.backgroundColor = [UIColor colorWithWhite:1 alpha:1];
    textField.leftView = padding;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self.activityIndicatorView stopAnimating];

    // Format title
    NSMutableAttributedString *attributedString = [[NSMutableAttributedString alloc] initWithString:@"STREAMING APP"];
    UIFont *font = [UIFont fontWithName:@"Panton-ExtraBold" size:30.0];

    [attributedString addAttribute:NSKernAttributeName value:@(2.1f) range:NSMakeRange(0, [@"STREAMING APP" length])];
    [attributedString addAttribute:NSFontAttributeName value:font range:NSMakeRange(0, [@"STREAMING APP" length])];
    _titleLabel.attributedText = attributedString;

    //Format text boxes
    [self formatTextBoxes:_passwordText];
    [self formatTextBoxes:_usernameText];

    // register for keyboard notifications
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(textFieldDidBeginEditing:)
                                                 name:UIKeyboardWillShowNotification
                                               object:self.view.window];
    // register for keyboard notifications
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(textFieldDidEndEditing:)
                                                 name:UIKeyboardWillHideNotification
                                               object:self.view.window];
    
    _loginButton.layer.cornerRadius = 30;

    //declutter for iPhone 4
    if (isIPhone4) {
        _sloganLabel.hidden = YES;
        _loginLabel.hidden = YES;
    }
    
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]
                                   initWithTarget:self
                                   action:@selector(dismissKeyboard)];
    
    [self.view addGestureRecognizer:tap];

    self.client = [[AUTClient alloc] initWithClientID:@"8db4c6db287749f2efbe" clientSecret:@"dc8596e8b8fefd24599e2c18c05241f1e2562607"];
    AppDelegate * delegate = [[UIApplication sharedApplication] delegate];
    delegate.client = self.client;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)loginButtonPressed:(id)sender {
    static int pressedCount = 0;
    
    if (pressedCount != 0) {
        return;
    } else {
        pressedCount++;
    }

    [self dismissKeyboard];
    
    [self.activityIndicatorView startAnimating];
    [_loginButton setTitle: @"" forState:UIControlStateNormal];
    
    
    
    [[[[[self.client
         authenticateWithUsername:[_usernameText.text lowercaseString]
         password:_passwordText.text
         scopes:AUTClientAuthorizationScopesPublic | AUTClientAuthorizationScopesUserProfile | AUTClientAuthorizationScopesVehicleVin | AUTClientAuthorizationScopesAdapterBasic]
        flattenMap:^RACStream *(id value) {
            return [self.client authenticateLegacyWithUsername:[_usernameText.text lowercaseString] password:_passwordText.text];
        }]
       logAll]
      deliverOnMainThread]
     subscribeError:^(NSError *error) {
         NSLog(@"%@", error);
         UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Authentication Failed."
                                                         message:@"Please check your username and password and try again."
                                                        delegate:nil
                                               cancelButtonTitle:@"OK"
                                               otherButtonTitles:nil];
         [alert show];
         [self.activityIndicatorView stopAnimating];
         [_loginButton setTitle: @"LOGIN" forState:UIControlStateNormal];
         pressedCount = 0;
     } completed:^{
         if (pressedCount == 1) {
             [self.client storeCredential];
             [self.activityIndicatorView stopAnimating];
             [_loginButton setTitle: @"LOGIN" forState:UIControlStateNormal];
             [self performSegueWithIdentifier:@"toDashboard" sender:self];
             pressedCount = 0;
         }
     }];
}

- (IBAction)usernameReturnKeyWasHit:(id)sender {
    [_passwordText becomeFirstResponder];
}

-(IBAction)passwordReturnKeyWasHit:(id)sender {
    [self dismissKeyboard];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    AppDelegate * delegate = [[UIApplication sharedApplication] delegate];
    
    delegate.dashboardController = [segue destinationViewController];
    delegate.loginController = self;
}

@end