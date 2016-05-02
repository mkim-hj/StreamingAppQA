#import "ALMockELM327Session.h"
#import "ALVehicleCommands.h"

NSString* const ELMErrorDomain = @"autoemote.elm327";
NSInteger const ELMErrorUnimplementedCommand = 1000;

NSString* const ELMSpeedInquiry = @"010D";

unsigned const ELMSpeedStopped = 0;
unsigned const ELMSpeedLimit = 170;



@implementation ALMockELM327Session
{
    BOOL streaming;
    BOOL accelerating;
    unsigned currentSpeed;
    unsigned currentRPM;
    float currentMPG;
    float currentOdometer;
    NSMutableDictionary *responses;
    NSTimer *streamingTimer;

    // RPM - Rotations Per Minute
    // MPG - Miles Per Gallon
    // KPH - Kilometer Per Hour
    // IAT - Intake Air Temp
    // MAP - Manifold Air Pressure
    // MAF - Mass Air Flow
    // LOAD - Engine Load
    // VOLT - Battery Voltage
}

- (instancetype)init
{
    if (self = [super init]) {
        accelerating = YES;
        currentSpeed = 0;
        responses = [NSMutableDictionary new];
    }

    return self;
}

- (BOOL)isConnected {
    return YES;
}

- (void)sendLine:(NSString *)inLine onResponse:(ALELM327SessionOnResponse)inOnResponse
{
    NSString *response = nil;
    NSError *error = nil;
    NSString *commandName = [ALVehicleCommands ALVehicleCommandsMap][inLine];
    NSDictionary *commandInfo = [ALVehicleCommands ALVehicleCommandsInfo][commandName];
    
    if ([ELMSpeedInquiry isEqualToString:inLine]) {
        float accel = 2.0f * ((float)rand()/RAND_MAX); // random accelerations every sample

        if( accelerating) {
            currentSpeed += (unsigned)accel;
            if( currentSpeed > ELMSpeedLimit) {
                currentSpeed = ELMSpeedLimit; // don't go over
                accelerating = NO;
            }
        }
        else {
            if( currentSpeed < accel) {
                currentSpeed = ELMSpeedStopped; // don't underflow
                accelerating = YES;
            }
            else {
                currentSpeed -= (unsigned)accel;
            }
        }
        
        response = [NSString stringWithFormat:@"00 00 %X", currentSpeed];
    }
    else if (commandInfo) {
        NSString *responseType = commandInfo[ALVehicleCommandResponseTypeKey];
        if ([responseType isEqualToString:ALVehicleCommandResponseTypeString]) {
            response = [NSString stringWithFormat:@"STRING"];
        }
        else if ([responseType isEqualToString:ALVehicleCommandResponseTypeNumber]) {
            double number = 0;
            NSArray* limits = commandInfo[ALVehicleCommandResponseLimitsKey];
            if (limits && limits.count == 2) {
                double minLimit = [commandInfo[ALVehicleCommandResponseLimitsKey][0] doubleValue];
                double maxLimit = [commandInfo[ALVehicleCommandResponseLimitsKey][1] doubleValue];
                double scaled = (double)rand()/RAND_MAX;
                number = ((maxLimit-minLimit+1)*scaled)+minLimit;
            }
            response = [NSString stringWithFormat:@"00 00 %X", (unsigned)number];
        }
        else if ([responseType isEqualToString:ALVehicleCommandResponseTypeArray]) {
            response = [NSString stringWithFormat:@"00 00 %X,%X,%X,%X", 1, 2, 3, 4];
        }
        else if ([responseType isEqualToString:ALVehicleCommandResponseTypePacked]) {
            response = @"PACKED";
        }
        else if ([responseType isEqualToString:ALVehicleCommandResponseTypeBits]) {
            response = @"BITS";
        }
    }
    else {
        error = [NSError
            errorWithDomain:ELMErrorDomain code:ELMErrorUnimplementedCommand
            userInfo:@{NSLocalizedDescriptionKey:@"Unimplemented ELM327 Command"}];
    }
    
    inOnResponse(inLine, response, error);
}

- (BOOL)isStreaming {
    return (streamingTimer != nil);
}

- (void)startStreaming:(NSArray<NSString *>*)PIDs onResponse:(ALELM327SessionOnStream)inOnStream {
    streamingTimer = [NSTimer
        scheduledTimerWithTimeInterval:0.1
        target:self
        selector:@selector(onStream:)
        userInfo:@{@"pids":PIDs,@"block":inOnStream}
        repeats:YES];
}

- (void)stopStreaming {
    [streamingTimer invalidate];
    streamingTimer = nil;
}

- (IBAction)onStream:(NSTimer *)timer {
    __block NSMutableArray *results = [NSMutableArray new];
    NSArray *PIDs = [timer userInfo][@"pids"];
    ALELM327SessionOnStream block = [timer userInfo][@"block"];
    for (NSString *PID in PIDs) {
        [self sendLine:PID onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
            if (!inError) {
                [results addObject:inResponse];
            }
            else {
                streaming = NO;
                [[NSException
                    exceptionWithName:NSInvalidArgumentException
                    reason:[NSString stringWithFormat:@"PID read failed: %@ with error: %@", PID, inError]
                    userInfo:@{NSUnderlyingErrorKey:inError}] raise];
            }
        }];
    }
    block(PIDs, results, nil);
}

@end
