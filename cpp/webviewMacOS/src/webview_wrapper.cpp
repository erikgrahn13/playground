#include "webview_wrapper.h"

#ifdef __APPLE__
    #include "platform/macos/webview_macos.h"
#elif _WIN32
    #include "platform/windows/webview_windows.h"
#elif __linux__
    #include "platform/linux/webview_linux.h"
#endif

class WebViewWrapper::Impl {
public:
#ifdef __APPLE__
    WebViewMacOS webview;
#elif _WIN32
    WebViewWindows webview;
#elif __linux__
    WebViewLinux webview;
#endif
};

WebViewWrapper::WebViewWrapper() : pImpl(new Impl()) {}

WebViewWrapper::~WebViewWrapper() {
    delete pImpl;
}

void WebViewWrapper::create(int width, int height, const std::string& title) {
    pImpl->webview.create(width, height, title);
}

void WebViewWrapper::navigate(const std::string& url) {
    pImpl->webview.navigate(url);
}

void WebViewWrapper::run() {
    pImpl->webview.run();
}

void WebViewWrapper::close() {
    pImpl->webview.close();
}

void WebViewWrapper::setDevelopmentMode(bool isDev) {
    m_developmentMode = isDev;
}
