//
//  DashboardViewController.m
//  
//
//  Created by Maruchi Kim on 10/27/15.
//
//

#import "DashboardViewController.h"
#import "LoginViewController.h"
#import "TestViewController.h"
#import "AppDelegate.h"

@interface DashboardViewController ()
@property (strong, nonatomic) UIView *overlay;
@property (weak, nonatomic) IBOutlet UIButton *logoutButton;
@property (weak, nonatomic) IBOutlet UIButton *pidsPerSecondButton;
@property (weak, nonatomic) IBOutlet UIButton *messagesPerSecondButton;
@property (weak, nonatomic) IBOutlet UIButton *burstButton;
@property (weak, nonatomic) IBOutlet UIButton *loopbackButton;
@property (weak, nonatomic) IBOutlet UISwitch *scantoolSwitch;
@property (weak, nonatomic) IBOutlet UITextField *commandInput;
@end

@implementation DashboardViewController

- (IBAction)commandEntered:(id)sender {
    [self dismissKeyboard];
}

-(void)dismissKeyboard {
    [_commandInput resignFirstResponder];
}

#pragma mark helpers
- (void) hideOverlay {
    _overlay.hidden = YES;
}

- (void) showOverlay {
    _overlay.hidden = NO;
}

- (void) logout {
    AppDelegate * delegate = [[UIApplication sharedApplication] delegate];

    //Preserve state
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:nil forKey:@"emailAddress"];
    [defaults synchronize];

    [delegate goToLogin];
}

- (void) renderModalViewController:(UIViewController*) controller withHeight:(CGFloat) height {
    [self showOverlay];
    controller.modalPresentationStyle = UIModalPresentationPopover;
    [self addChildViewController:controller];
    controller.view.frame = CGRectMake(20,
                                       self.view.frame.size.height/2 - height/2,
                                       self.view.frame.size.width-40,
                                       height);
    controller.view.layer.cornerRadius = 5;
    controller.view.layer.masksToBounds = YES;
    [self.view addSubview:controller.view];
    [controller didMoveToParentViewController:self];
}

#pragma mark view callbacks
- (void)viewDidLoad {
    [super viewDidLoad];
    
    _logoutButton.layer.cornerRadius = _logoutButton.frame.size.height/2;
    _pidsPerSecondButton.layer.cornerRadius = _pidsPerSecondButton.frame.size.height/2;
    _messagesPerSecondButton.layer.cornerRadius = _messagesPerSecondButton.frame.size.height/2;
    _burstButton.layer.cornerRadius = _burstButton.frame.size.height/2;
    _loopbackButton.layer.cornerRadius = _loopbackButton.frame.size.height/2;
    
    [_scantoolSwitch setOn:false];
    
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]
                                   initWithTarget:self
                                   action:@selector(dismissKeyboard)];
    [self.view addGestureRecognizer:tap];

}

- (void) viewDidAppear:(BOOL)animated {
    //Construct overlay for modal views, and then hide
    _overlay = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height)];
    [_overlay setBackgroundColor:[UIColor colorWithRed:0 green:0 blue:0 alpha:0.5]];
    [self.view addSubview:_overlay];
    [self hideOverlay];
    
    //Preserve state
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults synchronize];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

#pragma mark IB Actions
- (IBAction)logOutPressed:(id)sender {
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    UIViewController *controller = [storyboard instantiateViewControllerWithIdentifier:@"Logout"];
    [self renderModalViewController:controller withHeight:250.0f];
}
    
- (IBAction) pidsPerSecPressed:(id)sender {
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    UIViewController *controller = [storyboard instantiateViewControllerWithIdentifier:@"TestViewController"];
    [self showOverlay];
    [self renderModalViewController:controller withHeight:(self.view.frame.size.height - 40)];
    
    ((TestViewController *) controller).testTitle.text    = @"Chained Test";
    ((TestViewController *) controller).testCommand.text  = _commandInput.text;
    if (_scantoolSwitch.on) {
        ((TestViewController *) controller).testSubtitle.text = @"Scantool Mode ON";
    } else {
        ((TestViewController *) controller).testSubtitle.text = @"Scantool Mode OFF";
    }
}

- (IBAction) burstPressed:(id)sender {
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    UIViewController *controller = [storyboard instantiateViewControllerWithIdentifier:@"TestViewController"];
    [self showOverlay];
    [self renderModalViewController:controller withHeight:(self.view.frame.size.height - 40)];
    
    ((TestViewController *) controller).testTitle.text    = @"Burst Test";
    ((TestViewController *) controller).testCommand.text  = _commandInput.text;
    if (_scantoolSwitch.on) {
        ((TestViewController *) controller).testSubtitle.text = @"Scantool Mode ON";
    } else {
        ((TestViewController *) controller).testSubtitle.text = @"Scantool Mode OFF";
    }
}

- (IBAction) loopbackPressed:(id)sender {
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    UIViewController *controller = [storyboard instantiateViewControllerWithIdentifier:@"TestViewController"];
    [self showOverlay];
    [self renderModalViewController:controller withHeight:(self.view.frame.size.height - 40)];
    
    ((TestViewController *) controller).testTitle.text    = @"Looback Test";
    ((TestViewController *) controller).testCommand.text  = _commandInput.text;
    if (_scantoolSwitch.on) {
        ((TestViewController *) controller).testSubtitle.text = @"Scantool Mode ON";
    } else {
        ((TestViewController *) controller).testSubtitle.text = @"Scantool Mode OFF";
    }
}




@end