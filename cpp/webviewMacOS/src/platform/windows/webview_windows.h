#pragma once

#ifdef _WIN32
#include <windows.h>
#include <string>
#include <wrl.h>
#include <WebView2.h>

using namespace Microsoft::WRL;

class WebViewWindows {
public:
    WebViewWindows();
    ~WebViewWindows();
    
    void create(int width, int height, const std::string& title);
    void navigate(const std::string& url);
    void run();
    void close();

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void createWebView();
    void onWebViewCreated(ICoreWebView2Controller* controller);
    void setWebViewBounds();
    
    HWND m_hwnd;
    ComPtr<ICoreWebView2Environment> m_environment;
    ComPtr<ICoreWebView2Controller> m_controller;
    ComPtr<ICoreWebView2> m_webview;
    
    std::string m_pendingUrl;
    bool m_webviewReady;
    int m_width, m_height;
};

#endif
