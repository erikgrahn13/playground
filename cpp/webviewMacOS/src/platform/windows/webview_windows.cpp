#include "webview_windows.h"

#ifdef _WIN32
#include <windows.h>
#include <WebView2.h>
#include <wil/com.h>

#include "webview_windows.h"
#include "../../resource_manager.h"

#ifdef _WIN32
#include <windows.h>
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>
#include <string>
#include <iostream>

using namespace Microsoft::WRL;

// Custom resource handler for serving embedded resources
class WebResourceHandler : public RuntimeClass<RuntimeClassFlags<ClassicCom>, ICoreWebView2WebResourceRequestedEventHandler> {
public:
    HRESULT STDMETHODCALLTYPE Invoke(
        ICoreWebView2* sender,
        ICoreWebView2WebResourceRequestedEventArgs* args) override {
        
        wil::com_ptr<ICoreWebView2WebResourceRequest> request;
        args->get_Request(&request);
        
        wil::unique_cotaskmem_string uri;
        request->get_Uri(&uri);
        
        // Convert wide string to std::string
        std::wstring wideUri(uri.get());
        std::string uriStr(wideUri.begin(), wideUri.end());
        
        // Extract path from URI (remove "app://localhost")
        std::string path = "/";
        size_t pathStart = uriStr.find("app://localhost");
        if (pathStart != std::string::npos) {
            pathStart += 15; // Length of "app://localhost"
            if (pathStart < uriStr.length()) {
                path = uriStr.substr(pathStart);
            }
        }
        
        // Handle root path
        if (path.empty() || path == "/") {
            path = "/index.html";
        }
        
        // Get resource from embedded data
        auto resource = ResourceManager::getResource(path);
        if (resource) {
            // Create memory stream
            wil::com_ptr<IStream> stream;
            HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, resource->size);
            if (hGlobal) {
                void* pData = GlobalLock(hGlobal);
                memcpy(pData, resource->data, resource->size);
                GlobalUnlock(hGlobal);
                CreateStreamOnHGlobal(hGlobal, TRUE, &stream);
            }
            
            // Create response
            wil::com_ptr<ICoreWebView2Environment> environment;
            sender->get_Environment(&environment);
            
            wil::com_ptr<ICoreWebView2WebResourceResponse> response;
            std::wstring mimeType(resource->mimeType.begin(), resource->mimeType.end());
            environment->CreateWebResourceResponse(
                stream.get(), 200, L"OK", 
                (L"Content-Type: " + mimeType).c_str(), 
                &response);
            
            args->put_Response(response.get());
        } else {
            // Create 404 response
            wil::com_ptr<ICoreWebView2Environment> environment;
            sender->get_Environment(&environment);
            
            wil::com_ptr<ICoreWebView2WebResourceResponse> response;
            environment->CreateWebResourceResponse(
                nullptr, 404, L"Not Found", L"Content-Type: text/plain", &response);
            
            args->put_Response(response.get());
        }
        
        return S_OK;
    }
};

WebViewWindows::WebViewWindows() : m_hwnd(nullptr), m_controller(nullptr) {}

WebViewWindows::~WebViewWindows() {
    close();
}

void WebViewWindows::create(int width, int height, const std::string& title) {
    // Create window
    WNDCLASS wc = {};
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"WebViewWindow";
    RegisterClass(&wc);
    
    std::wstring wideTitle(title.begin(), title.end());
    HWND hwnd = CreateWindowEx(
        0, L"WebViewWindow", wideTitle.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        width, height, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    
    m_hwnd = hwnd;
    ShowWindow(hwnd, SW_SHOW);
    
    // Create WebView2 environment and controller
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                
                // Register custom scheme
                ICoreWebView2EnvironmentOptions* options;
                if (SUCCEEDED(env->QueryInterface(IID_PPV_ARGS(&options)))) {
                    ICoreWebView2CustomSchemeRegistration schemes[] = {
                        {L"app", COREWEBVIEW2_CUSTOM_SCHEME_OPTION_STANDARD}
                    };
                    options->put_AdditionalBrowserArguments(L"--allow-file-access-from-files");
                }
                
                env->CreateCoreWebView2Controller((HWND)m_hwnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                            m_controller = controller;
                            
                            wil::com_ptr<ICoreWebView2> webview;
                            controller->get_CoreWebView2(&webview);
                            
                            // Add resource request handler
                            auto handler = Make<WebResourceHandler>();
                            webview->AddWebResourceRequestedFilter(L"app://localhost/*", COREWEBVIEW2_WEB_RESOURCE_CONTEXT_ALL);
                            webview->add_WebResourceRequested(handler.Get(), nullptr);
                            
                            return S_OK;
                        }).Get());
                return S_OK;
            }).Get());
}

void WebViewWindows::navigate(const std::string& url) {
    if (!m_controller) return;
    
    wil::com_ptr<ICoreWebView2> webview;
    ((ICoreWebView2Controller*)m_controller)->get_CoreWebView2(&webview);
    
    std::wstring wideUrl(url.begin(), url.end());
    webview->Navigate(wideUrl.c_str());
}

void WebViewWindows::run() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void WebViewWindows::close() {
    if (m_controller) {
        ((ICoreWebView2Controller*)m_controller)->Close();
        m_controller = nullptr;
    }
    if (m_hwnd) {
        DestroyWindow((HWND)m_hwnd);
        m_hwnd = nullptr;
    }
}

#else
// Stub implementation for non-Windows platforms
WebViewWindows::WebViewWindows() : m_hwnd(nullptr), m_controller(nullptr) {}
WebViewWindows::~WebViewWindows() {}
void WebViewWindows::create(int width, int height, const std::string& title) {}
void WebViewWindows::navigate(const std::string& url) {}
void WebViewWindows::run() {}
void WebViewWindows::close() {}
#endif

WebViewWindows::~WebViewWindows() {
    close();
}

void WebViewWindows::create(int width, int height, const std::string& title) {
    // Windows implementation - placeholder for cross-platform support
    // Full implementation would require WebView2 setup
}

void WebViewWindows::navigate(const std::string& url) {
    // Windows navigation implementation
}

void WebViewWindows::run() {
    // Windows message loop
}

void WebViewWindows::close() {
    // Cleanup Windows resources
}

#endif
