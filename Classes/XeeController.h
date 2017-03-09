#import <Cocoa/Cocoa.h>

#import "XeeFSRef.h"
#import "XeeImageSource.h"

typedef NS_ENUM(int, XeeDrawerMode) {
	XeeNoMode = 0,
	XeeMoveMode = 1,
	XeeCopyMode = 2
};

#define XeeDrawerEdgeWidth 6.0

@class XeeImage,XeeImageSource,XeeView,XeeDisplayWindow,XeeFullScreenWindow;
@class XeeCollisionPanel,XeeRenamePanel,XeePasswordPanel,XeeStatusBar,XeeDestinationView;
@class XeeMoveTool,XeeCropTool;



@interface XeeController:NSObject <XeeImageSourceDelegate>
{
	XeeImageSource *source;
	XeeImage *currimage;

	CGFloat zoom;
	CGFloat touchrotation,touchrotateleftpoint,touchrotaterightpoint;
	NSInteger window_focus_x,window_focus_y;
	BOOL blocked,awake,autofullscreen,delaysheet;
	XeeDrawerMode drawer_mode;

	XeeMoveTool *movetool;
	XeeCropTool *croptool;

	NSDictionary *toolbaritems;
	NSArray *toolbaridentifiers;
	NSUndoManager *undo;

	XeeFullScreenWindow *fullscreenwindow;
	NSView *savedsuperview;
	NSRect savedframe;

	NSTimer *slideshowtimer;
	int slideshowcount;

	CGImageRef copiedcgimage;

	NSMutableArray *tasks;

	IBOutlet NSWindow *window;
    IBOutlet XeeView *imageview;
    IBOutlet XeeStatusBar *statusbar;
    IBOutlet NSDrawer *drawer;
	IBOutlet NSSegmentedControl *drawerseg;
	IBOutlet XeeDestinationView *destinationtable;
	IBOutlet NSButton *closebutton;
	IBOutlet XeeCollisionPanel *collisionpanel;
	IBOutlet XeeRenamePanel *renamepanel;
	IBOutlet XeePasswordPanel *passwordpanel;
	IBOutlet NSTextField *delayfield;
	IBOutlet NSPanel *delaypanel;
}

-(id)init;
-(void)dealloc;
-(void)awakeFromNib;

-(void)windowWillClose:(NSNotification *)notification;
-(void)windowDidBecomeMain:(NSNotification *)notification;
-(void)windowDidResignMain:(NSNotification *)notification;
-(void)windowDidMove:(NSNotification *)notification;
-(void)windowDidResize:(NSNotification *)notification;
-(void)windowWillMiniaturize:(NSNotification *)notification;
-(void)windowDidMiniaturize:(NSNotification *)notification;
-(NSUndoManager *)windowWillReturnUndoManager:(NSNotification *)notification;
-(void)setStatusBarHiddenNotification:(NSNotification *)notification;
-(void)refreshImageNotification:(NSNotification *)notification;

-(void)scrollWheel:(NSEvent *)event;
-(void)beginGestureWithEvent:(NSEvent *)event;
-(void)endGestureWithEvent:(NSEvent *)event;
-(void)magnifyWithEvent:(NSEvent *)event;
-(void)rotateWithEvent:(NSEvent *)event;
-(void)swipeWithEvent:(NSEvent *)event;

-(void)xeeImageSource:(XeeImageSource *)msgsource imageDidChange:(XeeImage *)image;
-(void)xeeImageSource:(XeeImageSource *)source imageListDidChange:(int)num;
-(void)xeeView:(XeeView *)view imageDidChange:(XeeImage *)image;
-(void)xeeView:(XeeView *)view imageSizeDidChange:(XeeImage *)image;
-(void)xeeView:(XeeView *)view imagePropertiesDidChange:(XeeImage *)image;

-(NSWindow *)window;
-(XeeFullScreenWindow *)fullScreenWindow;
-(XeeImageSource *)imageSource;
-(XeeImage *)image;
-(NSDrawer *)drawer;
-(XeeFSRef *)currentRef;
-(NSString *)currentFilename;
-(NSArray *)currentProperties;
-(BOOL)isFullscreen;
-(CGFloat)zoom;

-(void)setImageSource:(XeeImageSource *)newsource;
-(void)setImage:(XeeImage *)image;
-(void)setZoom:(CGFloat)newzoom;
-(void)setFrame:(NSInteger)frame;

@property (nonatomic) CGFloat zoom;
@property (readonly, getter=isFullscreen) BOOL fullscreen;
@property (nonatomic, retain) XeeImage *image;

-(void)updateWindowPosition;
-(void)setImageSize:(NSSize)size;
-(void)setImageSize:(NSSize)size resetFocus:(BOOL)reset;
-(void)setStandardImageSize;
-(void)setResizeBlock:(BOOL)block;
-(void)setResizeBlockFromSender:(id)sender;
-(BOOL)isResizeBlocked;
-(NSSize)maxViewSize;
-(NSSize)minViewSize;
-(NSRect)availableScreenSpace;

-(void)displayErrorMessage:(NSString *)title text:(NSString *)text;
-(void)displayPossibleError:(NSError *)error;
-(void)displayAlert:(NSAlert *)alert;

-(void)detachBackgroundTaskWithMessage:(NSString *)message selector:(SEL)selector target:(id)target object:(id)object;
-(void)detachBackgroundTask:(NSDictionary *)task;

-(NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)identifier willBeInsertedIntoToolbar:(BOOL)flag;
-(NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar *)toolbar;
-(void)setupToolbarItems;
-(NSArray *)makeToolbarItems;

-(BOOL)validateMenuItem:(NSMenuItem *)item;
-(BOOL)validateAction:(SEL)action;

-(void)updateStatusBar;
-(void)setStatusBarHidden:(BOOL)hidden;
-(BOOL)isStatusBarHidden;
-(IBAction)toggleStatusBar:(id)sender;

-(void)setDrawerEnableState;


-(IBAction)fullScreen:(id)sender;
-(void)autoFullScreen;

-(IBAction)confirm:(id)sender;
-(IBAction)cancel:(id)sender;

@end




@interface XeeFullScreenWindow:NSWindow
{
}

-(BOOL)canBecomeKeyWindow;

@end
