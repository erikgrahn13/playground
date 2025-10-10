#pragma once

#include <string>

class WebViewWindows {
public:
    WebViewWindows();
    ~WebViewWindows();
    
    void create(int width, int height, const std::string& title);
    void navigate(const std::string& url);
    void run();
    void close();
    
private:
    void* m_hwnd;        // HWND
    void* m_controller;  // ICoreWebView2Controller*
};
