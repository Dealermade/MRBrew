//
//  MRBrewWorker.m
//  MRBrew
//
//  Copyright (c) 2013 Marc Ransome <marc.ransome@fidgetbox.co.uk>
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

#import "MRBrewWorker.h"
#import "MRBrewWorker+Private.h"
#import "MRBrew.h"
#import "MRBrewOperation.h"
#import "MRBrewConstants.h"
#import "MRBrewDelegate.h"
#import "MRBrewWorkerTaskConstants.h"

static NSString * const MRBrewErrorDomain = @"uk.co.fidgetbox.MRBrew";
static const NSTimeInterval MRBrewWorkerTaskTerminationTimeout = 5.0;

@implementation MRBrewWorker

@synthesize executing = _executing;
@synthesize finished = _finished;

- (instancetype)init
{
    if (self = [super init]) {
        _task = [[NSTask alloc] init];
        _taskTerminationMode = MRBrewWorkerTaskTerminationModeInterrupt;
    }
    
    return self;
}

- (void)start
{
    if ([self isCancelled]) {
        [self changeFinishedState:YES];
        return;
    }
    
    [self changeExecutingState:YES];
    
    // configure the brew task instance
    [[self task] setLaunchPath:[[MRBrew sharedBrew] brewPath]];
    [[self task] setArguments:_arguments];
    [[self task] setStandardOutput:[NSPipe pipe]];
    
    NSDictionary *environment = [[MRBrew sharedBrew] environment];
    if (environment) {
        [[self task] setEnvironment:environment];
    }

    // register for task termination notification
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(taskExited:) name:NSTaskDidTerminateNotification object:[self task]];

    // configure read handler for asynchronous brew output
    [[[[self task] standardOutput] fileHandleForReading] setReadabilityHandler:^(NSFileHandle *file) {
        NSData *data = [file availableData];
        NSString *output = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
        if ([_delegate respondsToSelector:@selector(brewOperation:didGenerateOutput:)]) {
            [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                [_delegate brewOperation:_operation didGenerateOutput:output];
            }];
        }
    }];
    
    [self main];
}

- (void)main
{
    @try {
        [[self task] launch];

        // polling loop for operation cancellation and task termination
        while ([[self task] isRunning]) {

            // spin run loop to allow for delivery of task termination notification
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:1.0]];

            // poll for cancellation status and skip to the next iteration of
            // the loop unless we have received a cancellation message
            if (![self isCancelled]) continue;

            // a cancellation message was received so we attempt to terminate the task,
            // starting with a SIGINT signal and then increasing the severity of the
            // signal in subsequent attempts if the timeout period has been reached
            static BOOL taskSentInitialInterrupt = NO;
            if (!taskSentInitialInterrupt || [[self taskTerminationTime] timeIntervalSinceDate:[NSDate date]] >= MRBrewWorkerTaskTerminationTimeout) {
                taskSentInitialInterrupt = YES;
                [self setTaskTerminationTime:[NSDate date]];

                // signal task termination using the current termination mode and increase
                // the severity to the next level for subsequent attempts (SIGINT->SIGTERM->SIGKILL)
                switch ([self taskTerminationMode]) {
                    case MRBrewWorkerTaskTerminationModeInterrupt:
                        [[self task] interrupt];
                        [self setTaskTerminationMode:MRBrewWorkerTaskTerminationModeTerminate];
                        break;
                    case MRBrewWorkerTaskTerminationModeTerminate:
                        [[self task] terminate];
                        [self setTaskTerminationMode:MRBrewWorkerTaskTerminationModeKill];
                        break;
                    case MRBrewWorkerTaskTerminationModeKill:
                        kill([[self task] processIdentifier], SIGKILL);
                        break;
                }
            }
        }
    }
    @catch (NSException *exception) {
        NSLog(@"MRBrewWorker: An internal exception was raised (%@: %@)",[exception name], exception);
    }
    @finally {
        // cleanup
        [self changeExecutingState:NO];
        [self changeFinishedState:YES];
    }
}

- (BOOL)isAsynchronous
{
    return YES;
}

- (BOOL)isExecuting
{
    return _executing;
}

- (BOOL)isFinished
{
    return _finished;
}

- (void)changeFinishedState:(BOOL)finished
{
    [self willChangeValueForKey:@"isFinished"];
    _finished = finished;
    [self didChangeValueForKey:@"isFinished"];
}

- (void)changeExecutingState:(BOOL)executing
{
    [self willChangeValueForKey:@"isExecuting"];
    _executing = executing;
    [self didChangeValueForKey:@"isExecuting"];
}

- (void)taskExited:(NSNotification *)notification
{
    if ([[self task] terminationStatus] == MRBrewWorkerTaskExitedNormally) {
        [self notifyDelegateOperationCompleted];
    }
    else {
        [self notifyDelegateOperationFailed];
    }

    // stop reading and cleanup file handle's structures
    [[[[self task] standardOutput] fileHandleForReading] setReadabilityHandler:nil];
}

- (void)notifyDelegateOperationFailed {
    NSInteger errorCode = [[self task] terminationStatus] == MRBrewWorkerTaskCancelled ? MRBrewErrorOperationCancelled : MRBrewErrorUnknown;
    NSError *error = [NSError errorWithDomain:MRBrewErrorDomain code:errorCode userInfo:nil];
    if ([_delegate respondsToSelector:@selector(brewOperation:didFailWithError:)]) {
        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
            [_delegate brewOperation:_operation didFailWithError:error];
        }];
    }
}

- (void)notifyDelegateOperationCompleted {
    if ([_delegate respondsToSelector:@selector(brewOperationDidFinish:)]) {
        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
            [_delegate brewOperationDidFinish:_operation];
        }];
    }
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:NSTaskDidTerminateNotification object:[self task]];
}

@end
