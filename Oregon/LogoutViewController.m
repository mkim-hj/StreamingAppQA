//
//  LogoutViewController.m
//  Oregon
//
//  Created by Maruchi Kim on 3/22/16.
//  Copyright © 2016 Automatic, Inc. All rights reserved.
//

#import "LogoutViewController.h"
#import "DashboardViewController.h"

@implementation LogoutViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)noPressed:(id)sender {
    [(DashboardViewController *) self.parentViewController hideOverlay];
    [self.view removeFromSuperview];
    [self removeFromParentViewController];
}

- (IBAction)yesPressed:(id)sender {
    [(DashboardViewController *) self.parentViewController hideOverlay];
    [self.view removeFromSuperview];
    [(DashboardViewController *) self.parentViewController logout];
    [self removeFromParentViewController];
}

@end
