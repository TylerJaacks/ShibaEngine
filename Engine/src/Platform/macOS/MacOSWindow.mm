#if defined(__APPLE__)

#include <Platform/Window.h>

#import <AppKit/AppKit.h>
#import <QuartzCore/CAMetalLayer.h>

@interface MacOSWindowDelegate : NSObject <NSWindowDelegate>
{
@public
    Window* owner;
}
@end

@implementation MacOSWindowDelegate

- (BOOL)windowShouldClose:(id)sender {
    (void)sender;

    if (owner != nullptr) {
        owner->RequestClose();
    }

    return YES;
}

@end

struct Window::PlatformData {
    NSWindow* window = nil;
    NSView* view = nil;
    CAMetalLayer* metalLayer = nil;
    MacOSWindowDelegate* delegate = nil;
};

namespace {
    void EnsureApplication() {
        [NSApplication sharedApplication];

        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        static bool didFinishLaunching = false;

        if (!didFinishLaunching) {
            [NSApp finishLaunching];
            [NSApp activateIgnoringOtherApps:YES];

            didFinishLaunching = true;
        }
    }

    NSWindowStyleMask MakeWindowStyleMask(bool fullscreen) {
        NSWindowStyleMask style =
            NSWindowStyleMaskTitled |
            NSWindowStyleMaskClosable |
            NSWindowStyleMaskMiniaturizable |
            NSWindowStyleMaskResizable;

        if (fullscreen) {
            style |= NSWindowStyleMaskFullScreen;
        }

        return style;
    }

    NSString* ToNSString(const std::string& string) {
        return [NSString stringWithUTF8String:string.c_str()];
    }
}

Window::Window()
    : m_Properties(),
      m_EventCallback(),
      m_PlatformData(nullptr),
      m_shouldClose(false) {
}

Window::~Window() {
    Shutdown();
}

void Window::Create(const WindowProperties& properties) {
    if (m_PlatformData != nullptr) {
        Shutdown();
    }

    EnsureApplication();

    m_Properties = properties;
    m_shouldClose = false;

    m_PlatformData = new PlatformData();

    const NSRect frame = NSMakeRect(
        100.0,
        100.0,
        static_cast<CGFloat>(properties.Width),
        static_cast<CGFloat>(properties.Height)
    );

    m_PlatformData->window = [[NSWindow alloc]
        initWithContentRect:frame
        styleMask:MakeWindowStyleMask(properties.Fullscreen)
        backing:NSBackingStoreBuffered
        defer:NO
    ];

    [m_PlatformData->window setTitle:ToNSString(properties.Title)];
    [m_PlatformData->window center];
    [m_PlatformData->window makeKeyAndOrderFront:nil];

    m_PlatformData->view = [[NSView alloc] initWithFrame:frame];
    [m_PlatformData->view setWantsLayer:YES];

    m_PlatformData->metalLayer = [CAMetalLayer layer];
    [m_PlatformData->metalLayer setFramebufferOnly:YES];
    [m_PlatformData->metalLayer setPresentsWithTransaction:NO];

    CGSize drawableSize = CGSizeMake(
        static_cast<CGFloat>(properties.Width),
        static_cast<CGFloat>(properties.Height)
    );

    [m_PlatformData->metalLayer setDrawableSize:drawableSize];

    [m_PlatformData->view setLayer:m_PlatformData->metalLayer];
    [m_PlatformData->window setContentView:m_PlatformData->view];

    m_PlatformData->delegate = [[MacOSWindowDelegate alloc] init];
    m_PlatformData->delegate->owner = this;

    [m_PlatformData->window setDelegate:m_PlatformData->delegate];

    if (properties.Fullscreen) {
        [m_PlatformData->window toggleFullScreen:nil];
    }
}

void Window::Update() {
    if (m_PlatformData == nullptr || m_shouldClose) {
        return;
    }

    @autoreleasepool {
        NSEvent* event = nil;

        do {
            event = [NSApp
                nextEventMatchingMask:NSEventMaskAny
                untilDate:nil
                inMode:NSDefaultRunLoopMode
                dequeue:YES
            ];

            if (event != nil) {
                [NSApp sendEvent:event];
            }
        } while (event != nil);

        [NSApp updateWindows];

        if (m_PlatformData->window != nil) {
            const NSRect contentRect = [[m_PlatformData->window contentView] bounds];

            m_Properties.Width = static_cast<unsigned int>(contentRect.size.width);
            m_Properties.Height = static_cast<unsigned int>(contentRect.size.height);

            if (m_PlatformData->metalLayer != nil) {
                const CGFloat scale = [m_PlatformData->window backingScaleFactor];

                const CGSize drawableSize = CGSizeMake(
                    contentRect.size.width * scale,
                    contentRect.size.height * scale
                );

                [m_PlatformData->metalLayer setDrawableSize:drawableSize];
            }
        }
    }
}

void Window::Shutdown() {
    if (m_shouldClose && m_PlatformData == nullptr) {
        return;
    }

    m_shouldClose = true;

    if (m_PlatformData == nullptr) {
        return;
    }

    if (m_PlatformData->window != nil) {
        [m_PlatformData->window setDelegate:nil];
        [m_PlatformData->window close];
        m_PlatformData->window = nil;
    }

    if (m_PlatformData->delegate != nil) {
        [m_PlatformData->delegate release];
        m_PlatformData->delegate = nil;
    }

    if (m_PlatformData->view != nil) {
        [m_PlatformData->view release];
        m_PlatformData->view = nil;
    }

    m_PlatformData->metalLayer = nil;

    delete m_PlatformData;
    m_PlatformData = nullptr;
}

void Window::SetEventCallback(const EventCallbackFunction& callback) {
    m_EventCallback = callback;
}

void Window::SetVSync(const bool enabled) {
    m_Properties.VSync = enabled;
}

void Window::RequestClose() {
    m_shouldClose = true;
}

unsigned int Window::GetWidth() const {
    return m_Properties.Width;
}

unsigned int Window::GetHeight() const {
    return m_Properties.Height;
}

bool Window::IsVSync() const {
    return m_Properties.VSync;
}

bool Window::ShouldClose() const {
    return m_shouldClose;
}

NativeWindowHandle Window::GetNativeHandle() const {
    NativeWindowHandle handle{};

    if (m_PlatformData == nullptr) {
        return handle;
    }

    handle.window = m_PlatformData->window;
    handle.view = m_PlatformData->view;
    handle.layer = m_PlatformData->metalLayer;

    return handle;
}

#endif // defined(__APPLE__)
