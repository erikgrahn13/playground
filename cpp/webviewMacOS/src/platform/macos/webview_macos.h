#pragma once

#include <string>

class WebViewMacOS {
public:
    WebViewMacOS();
    ~WebViewMacOS();
    
    void create(int width, int height, const std::string& title);
    void navigate(const std::string& url);
    void run();
    void close();
    
private:
    void* m_window;   // NSWindow*
    void* m_webView;  // WKWebView*
};
