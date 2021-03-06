//
//  MRBrewDelegate.h
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

#import <Foundation/Foundation.h>
#import "MRBrewOperation.h"

/** The `MRBrewDelegate` protocol defines the optional methods implemented by
 delegates of the MRBrew class.
 
 MRBrew operations that generate output call the delegate method
 brewOperation:didGenerateOutput: _after_ completing an operation and
 immediately before calling brewOperationDidFinish:. In the case of
 `MRBrewOperationInstall` operations brewOperation:didGenerateOutput: may be
 called several times during the lifetime of the operation as output is
 received from Homebrew one line at a time.
 
 The brewOperation:didFailWithError: method is called at most once, if an error
 occurs performing an operation. The NSError object's `code` will correspond
 to one of the `MRBrewError` constants.
 
 In all three methods, the MRBrewOperation object's `name` property can be
 compared to the constants defined in MRBrewConstants.h to determine the type
 of operation that initiated the method call.
 
 If you need to respond in a certain way to a specific operation then you
 should retain the operation object and use the isEqualToOperation: method of
 the MRBrewOperation class to confirm the operation that generated the
 callback, and respond accordingly in your delegate methods.
 */
@protocol MRBrewDelegate <NSObject>

@optional

/** This method is called when an operation is completed.
 *
 * @param operation The type of operation that finished.
 */
- (void)brewOperationDidFinish:(MRBrewOperation *)operation;

/** This method is called when an operation fails.
 *
 * @param operation The type of operation that generated the error.
 * @param error An error object containing details of why the operation failed.
 */
- (void)brewOperation:(MRBrewOperation *)operation didFailWithError:(NSError *)error;

/** This method is called when output is received from Homebrew. For
 * `MRBrewOperationInstall` operations this method will be called when output
 * is made available. For other operations this method is called at most once,
 * after an operation has completed and only if output was generated.
 *
 * @param operation The type of operation that generated the output.
 * @param output The output string.
 */
- (void)brewOperation:(MRBrewOperation *)operation didGenerateOutput:(NSString *)output;

@end
