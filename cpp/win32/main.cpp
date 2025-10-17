#include <print>
#include <windows.h>
#include <tchar.h>
#include <ios>
#include <vector>
#include <array>
#include <unordered_map>
#include <string>

#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"

#include <Shlwapi.h>
#include "index_html.h"

#include "miniz.h"


#include "Data.h"

using namespace Microsoft::WRL;

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

// Pointer to WebViewController
static wil::com_ptr<ICoreWebView2Controller> webviewController;

static wil::com_ptr<ICoreWebView2Environment> environment;

// Pointer to WebView window
static wil::com_ptr<ICoreWebView2> webview;


// test
EventRegistrationToken m_webResourceRequestedToken = {};


HINSTANCE hInst;

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, Windows desktop!");

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

std::unordered_map<std::string, std::vector<uint8_t>> extractAllFilesFromZip() {
    std::unordered_map<std::string, std::vector<uint8_t>> files;
    
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    
    if (!mz_zip_reader_init_mem(&zip, Can::dist.data(), Can::dist.size(), 0)) {
        return {}; // Failed to open zip
    }
    
    int file_count = mz_zip_reader_get_num_files(&zip);
    for (int i = 0; i < file_count; i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip, i, &file_stat)) {
            continue;
        }
        
        // Skip directories
        if (mz_zip_reader_is_file_a_directory(&zip, i)) {
            continue;
        }
        
        size_t size;
        void* data = mz_zip_reader_extract_to_heap(&zip, i, &size, 0);
        
        if (data) {
            std::vector<uint8_t> file_data((uint8_t*)data, (uint8_t*)data + size);
            files[file_stat.m_filename] = std::move(file_data);
            free(data);
        }
    }
    
    mz_zip_reader_end(&zip);
    return files;
}

// Helper function to get MIME type based on file extension
std::string getContentType(const std::string& filename) {
    if (filename.ends_with(".html")) return "text/html";
    if (filename.ends_with(".js")) return "application/javascript";
    if (filename.ends_with(".css")) return "text/css";
    if (filename.ends_with(".png")) return "image/png";
    if (filename.ends_with(".jpg") || filename.ends_with(".jpeg")) return "image/jpeg";
    if (filename.ends_with(".svg")) return "image/svg+xml";
    if (filename.ends_with(".json")) return "application/json";
    if (filename.ends_with(".woff") || filename.ends_with(".woff2")) return "font/woff";
    if (filename.ends_with(".ttf")) return "font/ttf";
    return "application/octet-stream";
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {

    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
    std::ios::sync_with_stdio(true);
    SetConsoleTitleW(L"Debug Console");

    // std::println("Hello Win32 {}", Can::index.size());

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if(!RegisterClassEx(&wcex)) {
        MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Guided Tour"), NULL);
        return 1;
    }

    hInst = hInstance;

    HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, NULL, NULL, hInstance, NULL);

    if(!hWnd) {
        MessageBox(NULL, _T("Call to CreateWindowEx failed!"), _T("Windows Desktop Guided Tour"), NULL);
        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Webview2 code starts here
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWnd](HRESULT result, ICoreWebView2Environment * env) -> HRESULT {
                environment = env;
                env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [hWnd](HRESULT result, ICoreWebView2Controller *controller) -> HRESULT {
                        if(controller) {
                            webviewController = controller;
                            webviewController->get_CoreWebView2(&webview);
                        }

                        wil::com_ptr<ICoreWebView2Settings> settings;
                        webview->get_Settings(&settings);
                        settings->put_IsScriptEnabled(TRUE);
                        settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                        settings->put_IsWebMessageEnabled(TRUE);

                        RECT bounds;
                        GetClientRect(hWnd, &bounds);
                        webviewController->put_Bounds(bounds);




                        /********************************************************************** */
                        webview->AddWebResourceRequestedFilter (L"https://app.local/*", COREWEBVIEW2_WEB_RESOURCE_CONTEXT_ALL);

                        webview->add_WebResourceRequested(Callback<ICoreWebView2WebResourceRequestedEventHandler>(
                            [hWnd](ICoreWebView2* webview, ICoreWebView2WebResourceRequestedEventArgs* args) -> HRESULT
                            {
                                // Get the requested URL
                                wil::com_ptr<ICoreWebView2WebResourceRequest> request;
                                args->get_Request(&request);
                                
                                wil::unique_cotaskmem_string uri;
                                request->get_Uri(&uri);
                                
                                // Convert URL to file path
                                std::wstring url_wide(uri.get());
                                std::string url(url_wide.begin(), url_wide.end());
                                
                                std::string filepath = "dist/";
                                size_t pos = url.find("https://app.local");
                                if (pos != std::string::npos) {
                                    std::string path = url.substr(pos + 17); // Length of "https://app.local"
                                    if (path.empty() || path == "/") {
                                        filepath += "index.html"; // Default to index.html
                                    } else {
                                        if (path.starts_with("/")) path = path.substr(1);
                                        filepath += path;
                                    }
                                }
                                
                                // Extract all files and find the requested one
                                auto erik = extractAllFilesFromZip();
                                auto it = erik.find(filepath);
                                
                                if (it == erik.end()) {
                                    // File not found - return 404
                                    std::println("File not found: {}", filepath);
                                    wil::com_ptr<ICoreWebView2WebResourceResponse> response;
                                    environment->CreateWebResourceResponse(nullptr, 404, L"Not Found", L"", &response);
                                    args->put_Response(response.get());
                                    return S_OK;
                                }
                                
                                std::println("Serving: {} ({} bytes)", filepath, it->second.size());
                                
                                // Create stream from file data
                                auto stream = SHCreateMemStream((BYTE*)it->second.data(), (UINT)it->second.size());
                                
                                // Get correct content type
                                std::string content_type = getContentType(filepath);
                                std::wstring headers = L"Content-Type: " + 
                                    std::wstring(content_type.begin(), content_type.end());
                                
                                wil::com_ptr<ICoreWebView2WebResourceResponse> response;
                                environment->CreateWebResourceResponse(stream, 200, L"OK", headers.c_str(), &response);
                                args->put_Response(response.get());
                                return S_OK;
                            }).Get(), &m_webResourceRequestedToken);
                        webview->Navigate(L"https://app.local/");
                        /********************************************************************** */



                        // webview->Navigate(L"http://localhost:5173");

                        webview->AddScriptToExecuteOnDocumentCreated(L"Object.freeze(Object);", nullptr);
						// Schedule an async task to get the document URL
						webview->ExecuteScript(L"window.document.URL;", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
							[](HRESULT errorCode, LPCWSTR resultObjectAsJson) -> HRESULT {
								LPCWSTR URL = resultObjectAsJson;
								//doSomethingWithURL(URL);
								return S_OK;
							}).Get());



                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());

    // Webview2 code ends here

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
