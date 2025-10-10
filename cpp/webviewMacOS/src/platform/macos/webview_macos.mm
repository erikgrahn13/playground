#include "webview_macos.h"
#include "../../resource_manager.h"
#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

@interface WebViewDelegate : NSObject <NSWindowDelegate>
@end

@implementation WebViewDelegate
- (void)windowWillClose:(NSNotification *)notification {
    [NSApp terminate:nil];
}
@end

// Custom URL scheme handler for serving embedded resources
@interface AppSchemeHandler : NSObject <WKURLSchemeHandler>
@end

@implementation AppSchemeHandler
- (void)webView:(WKWebView *)webView startURLSchemeTask:(id<WKURLSchemeTask>)urlSchemeTask {
    NSURL *url = urlSchemeTask.request.URL;
    NSString *path = url.path;
    
    // Convert NSString to std::string
    std::string resourcePath = [path UTF8String];
    
    // Handle root path
    if (resourcePath.empty() || resourcePath == "/") {
        resourcePath = "/index.html";
    }
    
    // Get resource from embedded data
    auto resource = ResourceManager::getResource(resourcePath);
    if (resource) {
        // Create response data
        NSData *data = [NSData dataWithBytes:resource->data length:resource->size];
        
        // Determine MIME type
        NSString *mimeType = [NSString stringWithUTF8String:resource->mimeType.c_str()];
        
        // Create HTTP response with additional headers for better compatibility
        NSDictionary *headers = @{
            @"Content-Type": mimeType,
            @"Cache-Control": @"no-cache", // Ensure fresh loads during development
            @"Access-Control-Allow-Origin": @"*" // Allow cross-origin requests if needed
        };
        
        NSHTTPURLResponse *response = [[NSHTTPURLResponse alloc] 
            initWithURL:url 
            statusCode:200 
            HTTPVersion:@"HTTP/1.1" 
            headerFields:headers];
        
        // Send response
        [urlSchemeTask didReceiveResponse:response];
        [urlSchemeTask didReceiveData:data];
        [urlSchemeTask didFinish];
    } else {
        // Log missing resource for debugging
        NSLog(@"Resource not found: %@", [NSString stringWithUTF8String:resourcePath.c_str()]);
        
        // Return 404 response
        NSHTTPURLResponse *response = [[NSHTTPURLResponse alloc] 
            initWithURL:url 
            statusCode:404 
            HTTPVersion:@"HTTP/1.1" 
            headerFields:@{@"Content-Type": @"text/plain"}];
        
        NSData *errorData = [@"Resource not found" dataUsingEncoding:NSUTF8StringEncoding];
        [urlSchemeTask didReceiveResponse:response];
        [urlSchemeTask didReceiveData:errorData];
        [urlSchemeTask didFinish];
    }
}

- (void)webView:(WKWebView *)webView stopURLSchemeTask:(id<WKURLSchemeTask>)urlSchemeTask {
    // Cleanup if needed
}
@end

WebViewMacOS::WebViewMacOS() : m_window(nullptr), m_webView(nullptr) {}

WebViewMacOS::~WebViewMacOS() {
    close();
}

void WebViewMacOS::create(int width, int height, const std::string& title) {
    @autoreleasepool {
        // Create window
        NSRect frame = NSMakeRect(0, 0, width, height);
        NSWindow* window = [[NSWindow alloc] 
            initWithContentRect:frame
            styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
            backing:NSBackingStoreBuffered
            defer:NO];
        
        [window setTitle:[NSString stringWithUTF8String:title.c_str()]];
        [window center];
        
        // Create delegate
        WebViewDelegate* delegate = [[WebViewDelegate alloc] init];
        [window setDelegate:delegate];
        
        // Create WebView configuration with custom scheme handler
        WKWebViewConfiguration* config = [[WKWebViewConfiguration alloc] init];
        AppSchemeHandler* schemeHandler = [[AppSchemeHandler alloc] init];
        [config setURLSchemeHandler:schemeHandler forURLScheme:@"app"];
        
        // Create webview with configuration
        WKWebView* webView = [[WKWebView alloc] initWithFrame:[window.contentView bounds] configuration:config];
        [webView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
        [window.contentView addSubview:webView];
        
        m_window = (__bridge_retained void*)window;
        m_webView = (__bridge_retained void*)webView;
    }
}

void WebViewMacOS::navigate(const std::string& url) {
    if (!m_webView) return;
    
    @autoreleasepool {
        WKWebView* webView = (__bridge WKWebView*)m_webView;
        NSURL* nsurl = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
        NSURLRequest* request = [NSURLRequest requestWithURL:nsurl];
        [webView loadRequest:request];
    }
}

void WebViewMacOS::run() {
    if (!m_window) return;
    
    @autoreleasepool {
        // Initialize NSApplication if needed
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        NSWindow* window = (__bridge NSWindow*)m_window;
        [window makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];
        
        if (![NSApp isRunning]) {
            [NSApp run];
        }
    }
}

void WebViewMacOS::close() {
    @autoreleasepool {
        if (m_webView) {
            CFBridgingRelease(m_webView);
            m_webView = nullptr;
        }
        if (m_window) {
            NSWindow* window = (__bridge_transfer NSWindow*)m_window;
            [window close];
            m_window = nullptr;
        }
    }
}
