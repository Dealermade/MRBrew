//
//  MRBrewOutputParser.h
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

extern NSString * const MRBrewOutputParserErrorDomain;

enum {
    MRBrewOutputParserErrorSyntax,
    MRBrewOutputParserErrorUnsupportedOperation
};

@class MRBrewOperation;

/** The `MRBrewOutputParser` class provides rudimentary support for parsing
 * objects from the output generated when performing operations using `MRBrew`'s
 * `performOperation:delegate:` method.
 */
@interface MRBrewOutputParser : NSObject

/**-----------------------------------------------------------------------------
 * @name Creating an Output Parser
 * -----------------------------------------------------------------------------
 */

/** Creates and returns an output parser.
 *
 * @return An initialised output parser.
 */
+ (instancetype)outputParser;

/**-----------------------------------------------------------------------------
 * @name Parsing Objects
 * -----------------------------------------------------------------------------
 */

/** Returns an array containing one or more objects parsed from the output
 * of an operation that was performed using `MRBrew`'s
 * `performOperation:delegate:` method.
 *
 * Parsing is only supported for output generated by `MRBrewOperation` objects
 * whose `name` property (equivalent to the _command_ in Homebrew terminology)
 * matches one of the constants `MRBrewOperationListIdentifier`,
 * `MRBrewOperationSearchIdentifier` or `MRBrewOperationOptionsIdentifier`.
 *
 * @param operation The operation object that generated the output.
 * @param output The output string to parse.
 * @param error A pointer to an error object that is set to an NSError instance
 * if parsing was unsuccessful. This parameter is optional and can be passed
 * `nil`.
 * @return An array of objects parsed from an operation's output or `nil` if
 * the operation type is unsupported or the string could not be parsed (or is
 * empty). For operations whose `name` property is equal to one of the constants
 * `MRBrewOperationListIdentifier` or `MRBrewOperationSearchIdentifier`,
 * the returned array will contain one or more `MRBrewFormula` objects.
 * In addition, for `MRBrewOperationListIdentifier` operations, each operation
 * object will have its `isInstalled` property set to `YES`. For operations
 * whose `name` property matches the `MRBrewOperationOptionsIdentifier`
 * constant, the returned array will contain one or more `MRBrewInstallOption`
 * objects.
 */
- (NSArray *)objectsForOperation:(MRBrewOperation *)operation output:(NSString *)output error:(NSError **)error;

@end
