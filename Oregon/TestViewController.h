//
//  TestViewController.h
//  Oregon
//
//  Created by Maruchi Kim on 10/28/15.
//  Copyright © 2015 Maruchi Kim. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AutomaticAdapter/AutomaticAdapter.h>
#import "AppDelegate.h"
#import <CoreBluetooth/CoreBluetooth.h>


@interface TestViewController : UIViewController <CBCentralManagerDelegate>
    @property (retain, nonatomic) ALELM327Session * currentSession;
@property (weak, nonatomic) IBOutlet UILabel *testTitle;
@property (weak, nonatomic) IBOutlet UILabel *testSubtitle;
@property (weak, nonatomic) IBOutlet UILabel *testLabel;
@property (weak, nonatomic) IBOutlet UILabel *testCaption;
@property (weak, nonatomic) IBOutlet UILabel *testCommand;
@end
