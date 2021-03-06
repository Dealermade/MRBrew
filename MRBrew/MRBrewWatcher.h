//
//  MRBrewWatcher.h
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
#import "MRBrewWatcherDelegate.h"

/** These constants indicate the location to watch for events. */
typedef NS_OPTIONS(NSInteger, MRBrewWatcherLocation) {
    /** The Homebrew `Library` path */
    MRBrewWatcherLibraryLocation    = 1 << 0,
    /** The Homebrew `Formula` path */
    MRBrewWatcherFormulaLocation    = 1 << 1,
    /** The Homebrew `Taps` path */
    MRBrewWatcherTapsLocation       = 1 << 2,
    /** The Homebrew `Aliases` path */
    MRBrewWatcherAliasesLocation    = 1 << 3,
    /** The Homebrew `LinkedKegs` path */
    MRBrewWatcherLinkedKegsLocation = 1 << 4,
    /** The Homebrew `PinnedKegs` path */
    MRBrewWatcherPinnedKegsLocation = 1 << 5
};

/** An `MRBrewWatcher` waits for a file system event (e.g. file modification,
 deletion or creation) to occur in one or more Homebrew directories and then
 sends a message—defined by the MRBrewWatcherDelegate protocol—to the delegate
 object informing it of the event. For example, you could create a watcher
 object that sends a message to your controller when an event occurs in the
 Homebrew formula directory, and respond accordingly.
 
 To create an `MRBrewWatcher` object that watches one of the default Homebrew
 directories for events, use the initWithLocation:delegate: or
 watcherWithLocation:delegate: methods and specify one of the
 MRBrewWatcherLocation constants as the location to watch for events:

   MRBrewWatcherLibraryLocation
   MRBrewWatcherFormulaLocation
   MRBrewWatcherTapsLocation
   MRBrewWatcherAliasesLocation
   MRBrewWatcherLinkedKegsLocation
   MRBrewWatcherPinnedKegsLocation
 
 These constants represent the default Homebrew paths for the named locations,
 and can be combined using the C-Bitwise OR operator in order to watch multiple
 locations for events with a single watcher object.
 
 If you have installed Homebrew outside of the default path, create your watcher
 object using either initWithPath:delegate: or watcherWithPath:delegate: and
 specify the absolute path to the directory to watch for events.
 
 To start watching for events call the startWatching method, and to stop
 watching call the stopWatching method.
 
 @warning After starting a watcher object, calls to the MRBrew class method
 performOperation:delegate: may result in file system events occurring at the
 watched location, which will trigger a delegate callback. If you are only
 interested in external events then you should stop the watcher object prior to
 calling performOperation:delegate: and restart it once the operation has
 completed.
 */
@interface MRBrewWatcher : NSObject

/** The delegate object for this watcher. */
@property (weak) id<MRBrewWatcherDelegate> delegate;

/**-----------------------------------------------------------------------------
 * @name Initialising a Watcher
 * -----------------------------------------------------------------------------
 */

/** Returns an initialized `MRBrewWatcher` object with the specified location
 * and delegate.
 *
 * @param location The location to watch for file system events. See
 * MRBrewWatcherLocation for the options. Multiple locations can be watched by
 * combining constants using the C-Bitwise OR operator.
 * @param delegate The delegate object for this watcher. The delegate will
 * receive a message when file system events occur at the specified location.
 * @return A watcher with the specified location and delegate.
 */
- (instancetype)initWithLocation:(MRBrewWatcherLocation)location delegate:(id<MRBrewWatcherDelegate>)delegate;

/** Returns an initialized `MRBrewWatcher` object with the specified path
 * and delegate.
 *
 * @param path The absolute path to watch for file system events.
 * @param delegate The delegate object for this watcher. The delegate will
 * receive a message when file system events occur at the specified path.
 * @return A watcher with the specified path and delegate.
 */
- (instancetype)initWithPath:(NSString *)path delegate:(id<MRBrewWatcherDelegate>)delegate;

/**-----------------------------------------------------------------------------
 * @name Creating a Watcher
 * -----------------------------------------------------------------------------
 */

/** Returns a watcher with the specified location and delegate.
 *
 * @param location The location to watch for file system events. See
 * MRBrewWatcherLocation for the options. Multiple locations can be watched by
 * combining constants using the C-Bitwise OR operator.
 * @param delegate The delegate object for this watcher. The delegate will
 * receive a message when file system events occur at the specified location.
 * @return A watcher with the specified location and delegate.
 */
+ (instancetype)watcherWithLocation:(MRBrewWatcherLocation)location delegate:(id<MRBrewWatcherDelegate>)delegate;

/** Returns a watcher with the specified path and delegate.
 *
 * @param path The absolute path to watch for file system events.
 * @param delegate The delegate object for this watcher. The delegate will
 * receive a message when file system events occur at the specified path.
 * @return A watcher with the specified path and delegate.
 */
+ (instancetype)watcherWithPath:(NSString *)path delegate:(id<MRBrewWatcherDelegate>)delegate;

/**-----------------------------------------------------------------------------
 * @name Starting and Stopping a Watcher
 * -----------------------------------------------------------------------------
 */

/** Causes the receiver to start watching for file system events. */
- (void)startWatching;

/** Causes the receiver to stop watching for file system events. */
- (void)stopWatching;

/** Indicates whether the receiver is watching. */
- (BOOL)isWatching;

@end
