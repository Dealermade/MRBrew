//
//  MRBrewOperation.m
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

#import "MRBrewOperation.h"
#import "MRBrewFormula.h"
#import "MRBrewConstants.h"

@implementation MRBrewOperation

#pragma mark - Lifecycle

- (instancetype)initWithType:(MRBrewOperationType)type formula:(MRBrewFormula *)formula parameters:(NSArray *)parameters
{
    if (self = [super init]) {
        switch (type) {
            case MRBrewOperationUpdate:
                _name = MRBrewOperationUpdateIdentifier;
                break;
            case MRBrewOperationList:
                _name = MRBrewOperationListIdentifier;
                break;
            case MRBrewOperationSearch:
                _name = MRBrewOperationSearchIdentifier;
                break;
            case MRBrewOperationInstall:
                _name = MRBrewOperationInstallIdentifier;
                break;
            case MRBrewOperationInfo:
                _name = MRBrewOperationInfoIdentifier;
                break;
            case MRBrewOperationRemove:
                _name = MRBrewOperationRemoveIdentifier;
                break;
            case MRBrewOperationOptions:
                _name = MRBrewOperationOptionsIdentifier;
                break;
            case MRBrewOperationOutdated:
                _name = MRBrewOperationOutdatedIdentifier;
                break;
        }
        _formula = [formula copy];
        _parameters = [parameters copy];
    }
    
    return self;
}

- (instancetype)initWithName:(NSString *)name formula:(MRBrewFormula *)formula parameters:(NSArray *)parameters
{
    if (self = [super init]) {
        _name = [name copy];
        _formula = [formula copy];
        _parameters = [parameters copy];
    }
    
    return self;
}

+ (instancetype)operationWithType:(MRBrewOperationType)type formula:(MRBrewFormula *)formula parameters:(NSArray *)parameters
{
    return [[self alloc] initWithType:type formula:formula parameters:parameters];
}

+ (instancetype)operationWithName:(NSString *)name formula:(MRBrewFormula *)formula parameters:(NSArray *)parameters
{
    return [[self alloc] initWithName:name formula:formula parameters:parameters];
}

+ (instancetype)updateOperation
{
    return [[self alloc] initWithType:MRBrewOperationUpdate formula:nil parameters:nil];
}

+ (instancetype)listOperation
{
    return [[self alloc] initWithType:MRBrewOperationList formula:nil parameters:nil];
}

+ (instancetype)searchOperation
{
    return [[self alloc] initWithType:MRBrewOperationSearch formula:nil parameters:nil];
}

+ (instancetype)searchOperation:(MRBrewFormula *)formula
{
    return [[self alloc] initWithType:MRBrewOperationSearch formula:formula parameters:nil];
}

+ (instancetype)installOperation:(MRBrewFormula *)formula
{
    return [[self alloc] initWithType:MRBrewOperationInstall formula:formula parameters:nil];
}

+ (instancetype)infoOperation:(MRBrewFormula *)formula
{
    return [[self alloc] initWithType:MRBrewOperationInfo formula:formula parameters:nil];
}

+ (instancetype)removeOperation:(MRBrewFormula *)formula
{
    return [[self alloc] initWithType:MRBrewOperationRemove formula:formula parameters:nil];
}

+ (instancetype)optionsOperation:(MRBrewFormula *)formula
{
    return [[self alloc] initWithType:MRBrewOperationOptions formula:formula parameters:nil];
}

+ (instancetype)outdatedOperation
{
    return [[self alloc] initWithType:MRBrewOperationOutdated formula:nil parameters:nil];
}

#pragma mark - Equality

- (BOOL)isEqualToOperation:(MRBrewOperation *)operation
{
    if (self == operation)
        return YES;
    
    if (!operation || ![operation isKindOfClass:[self class]])
        return NO;
    
    if (![[self name] isEqualToString:[operation name]])
        return NO;

    if ([self formula] && [operation formula]) {
        if (![[self formula] isEqualToFormula:[operation formula]]) {
            return NO;
        }
    }
    else if (![self formula] ^ ![operation formula]) {
        return NO;
    }

    if ([self parameters] && [operation parameters]) {
        if (![[self parameters] isEqualToArray:[operation parameters]]) {
            return NO;
        }
    }
    else if (![self parameters] ^ ![operation parameters]) {
        return NO;
    }
    
    return YES;
}

#pragma mark - NSCopying protocol

- (id)copyWithZone:(NSZone *)zone
{
    MRBrewOperation *copy = [[[self class] allocWithZone:zone] init];
    [copy setName:[[self name] copy]];
    [copy setFormula:[[self formula] copy]];
    [copy setParameters:[[self parameters] copy]];
    
    return copy;
}

- (NSString *)description
{
    NSMutableString *command = [NSMutableString string];
    
    if ([self name]) {
        [command appendFormat:@"%@ ", [self name]];
    }
    
    for (NSString *parameter in [self parameters]) {
        [command appendFormat:@"%@ ", parameter];
    }
    
    if ([self formula]) {
        [command appendFormat:@"%@", [[self formula] name]];
    }

    return [command stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
}

@end
