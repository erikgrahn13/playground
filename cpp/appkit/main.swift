import SwiftUI
import WebKit

@main
struct MyApp : App {
    var body: some Scene {
        WindowGroup {
            WebViewRepresentable(url: URL(string: "http://localhost:5173")!)
        }
    }
}

struct WebViewRepresentable: NSViewRepresentable {
    let url: URL
    
    func makeNSView(context: Context) -> WKWebView {
        let webView = WKWebView()
        webView.load(URLRequest(url: url))
        return webView
    }
    
    func updateNSView(_ nsView: WKWebView, context: Context) {}
}