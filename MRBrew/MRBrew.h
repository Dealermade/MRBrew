//
//  MRBrew.h
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

#import <Cocoa/Cocoa.h>
#import "MRBrewOperation.h"

/** These constants indicate the type of error that resulted in an operation's
 * failure.
 */
typedef NS_ENUM(NSInteger, MRBrewError) {
    /** Indicates the absence of an error condition. Used exclusively for unit
     * testing.
     */
    MRBrewErrorNone,
    /** Indicates that an unknown Homebrew error occurred when performing the
     * operation.
     */
    MRBrewErrorUnknown,
    /** Indicates that the operation failed to complete due to a cancellation
     * message.
     */
    MRBrewErrorOperationCancelled
};

@protocol MRBrewDelegate;
@class MRBrewWorker;

/** The `MRBrew` class manages the execution of Homebrew operations. Operation
 * objects (defined by the MRBrewOperation class) are added to a queue and
 * remain there until they are explicitly cancelled or finish executing.
 *
 * Each call to performOperation:delegate: places an operation into the queue.
 * When an operation starts executing it will spawn a subprocess from a separate
 * thread. Multiple operations can be performed by making repeated
 * calls to performOperation:delegate:.
 *
 * By default, operations are executed concurrently, but this behaviour can be
 * controlled using setConcurrentOperations:.
 *
 * `MRBrew`'s delegate methods—defined by the MRBrewDelegate protocol—allow
 * an object to receive callbacks regarding the success or failure of an
 * operation and output from Homebrew as it occurs.
 *
 * @warning Attempting to perform two operations that reference the same formula
 * concurrently may result in the failure of one of those operations. This is
 * the default behaviour for Homebrew.
 *
 * @warning All operations performed by the `MRBrew` class inherit the
 * environment from which those operation were launched. Use `setEnvironment:`
 * to define your own environment variables.
 */
@interface MRBrew : NSObject

/**-----------------------------------------------------------------------------
 * @name Accessing the Shared Brew Instance
 * -----------------------------------------------------------------------------
 */

/** Returns the shared `MRBrew` instance, creating it if necessary.
 *
 * @return The shared `MRBrew` instance.
 */
+ (instancetype)sharedBrew;

/**-----------------------------------------------------------------------------
 * @name Modifying the Homebrew path
 * -----------------------------------------------------------------------------
 */

/** Returns the absolute path of the Homebrew executable.
 *
 * @return The Homebrew executable path.
 */
- (NSString *)brewPath;

/** Sets the absolute path of the Homebrew executable.
 *
 * @param path The absolute path of the Homebrew executable. If `nil` the
 * default path `/usr/local/bin/brew` will be used.
 */
- (void)setBrewPath:(NSString *)path;

/**-----------------------------------------------------------------------------
 * @name Performing an Operation
 * -----------------------------------------------------------------------------
 */

/** Performs an operation.
 *
 * Operations are placed in a queue for execution and will always execute on
 * separate threads. Use setConcurrentOperations: to control how queued
 * operations are executed (i.e. concurrently, or serially).
 *
 * @param operation The operation to perform.
 * @param delegate The delegate object for the operation. The delegate will
 * receive delegate messages during execution of the operation when output is
 * generated and upon completion or failure of the operation.
 */
- (void)performOperation:(MRBrewOperation *)operation delegate:(id<MRBrewDelegate>)delegate;

/**-----------------------------------------------------------------------------
 * @name Stopping an Operation
 * -----------------------------------------------------------------------------
 */

/** Cancels all queued and executing operations.
 *
 * This method has no effect if there are currently no queued operations.
 */
- (void)cancelAllOperations;

/** Cancels a queued or executing operation.
 *
 * This method has no effect if the operation has already finished executing.
 *
 * @param operation The operation to cancel.
 */
- (void)cancelOperation:(MRBrewOperation *)operation;

/** Cancels all queued and executing operations of the specified type.
 *
 * This method has no effect if there are currently no queued or executing
 * operations of the specified type.
 *
 * @param type The type of operations to cancel.
 */
- (void)cancelAllOperationsOfType:(MRBrewOperationType)type;

/**-----------------------------------------------------------------------------
 * @name Managing Operations
 * -----------------------------------------------------------------------------
 */

/** Sets the concurrent execution of operations.
 *
 * By default, operations are executed concurrently. Changing concurrency type
 * does not affect operations that are currently executing. Operations are
 * always executed in separate threads.
 *
 * @param concurrency If `YES`, operations are executed concurrently. If `NO`,
 * operations are executed serially.
 */
- (void)setConcurrentOperations:(BOOL)concurrency;

/** Returns the number of operations queued for execution.
 *
 * The value returned by this method will change as operations are completed.
 *
 * @return The number of operations currently queued for execution.
 */
- (NSUInteger)operationCount;

/**-----------------------------------------------------------------------------
 * @name Managing the Environment
 * -----------------------------------------------------------------------------
 */

/** Returns a dictionary of variables for the environment that operations will
 * execute with.
 *
 * @return A dictionary of environment variables whose keys represent variable
 * names.
 */
- (NSDictionary *)environment;

/** Sets the environment for all future operations.
 *
 * If this method is never called, operations will inherit the environment of
 * the process that launches them.
 *
 * @param environment A dictionary of environment variables whose keys represent
 * variable names.
 */
- (void)setEnvironment:(NSDictionary *)environment;

@end
