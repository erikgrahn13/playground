#pragma once

#include <string>

class WebViewWrapper {
public:
    WebViewWrapper();
    ~WebViewWrapper();
    
    void create(int width, int height, const std::string& title);
    void navigate(const std::string& url);
    void run();
    void close();
    
    // Development/Production mode
    void setDevelopmentMode(bool isDev);
    
private:
    class Impl;
    Impl* pImpl;
    bool m_developmentMode = true;
};
