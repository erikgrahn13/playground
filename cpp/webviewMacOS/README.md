# Native WebView C++ Project

A lightweight, cross-platform C++ application for hosting React applications using native WebView components without external dependencies.

## Features

- **Zero Dependencies**: Uses system-provided WebView components
- **Cross-Platform**: Supports macOS (WKWebView), Windows (WebView2), and Linux (WebKitGTK)
- **Development/Production Modes**: Automatically switches between dev server and embedded files
- **Lightweight**: Minimal binary size compared to alternatives like Electron or CEF
- **Native Performance**: Direct integration with OS WebView engines

## Platform Support

| Platform | WebView Engine | System Requirement |
|----------|----------------|-------------------|
| macOS    | WKWebView      | macOS 10.10+     |
| Windows  | WebView2       | Windows 10+       |
| Linux    | WebKitGTK      | GTK 3.0+         |

## Project Structure

```
├── src/
│   ├── main.cpp                    # Application entry point
│   ├── webview_wrapper.h/.cpp      # Cross-platform WebView wrapper
│   └── platform/
│       ├── macos/                  # macOS WKWebView implementation
│       ├── windows/                # Windows WebView2 implementation (stub)
│       └── linux/                  # Linux WebKitGTK implementation (stub)
├── CMakeLists.txt                  # Cross-platform build configuration
└── README.md
```

## Building

### Prerequisites

- **macOS**: Xcode Command Line Tools
- **Windows**: Visual Studio 2019+ with WebView2 SDK
- **Linux**: GTK 3.0+ development headers, WebKitGTK development headers

### Build Commands

```bash
mkdir build
cd build
cmake ..
make  # or cmake --build . on Windows
```

### Debug vs Release

```bash
# Debug build (connects to React dev server)
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build (serves embedded files)
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Usage

### Development Mode

1. Start your React development server:
   ```bash
   npm run dev  # or yarn dev
   ```

2. Build and run the native app in debug mode:
   ```bash
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   make
   ./NativeWebView
   ```

The app will connect to `http://localhost:5173/` and display your React application.

### Production Mode

1. Build your React app:
   ```bash
   npm run build
   ```

2. Build and run the native app in release mode:
   ```bash
   cmake -DCMAKE_BUILD_TYPE=Release ..
   make
   ./NativeWebView
   ```

The app will serve embedded files from the build directory.

## Development Workflow

This project is designed for React developers who want to create desktop applications:

1. **Develop**: Use your normal React development workflow with hot reloading
2. **Test**: Build the native app in debug mode to test WebView integration
3. **Deploy**: Build in release mode to create a distributable desktop application

## Advantages Over Alternatives

| Feature | Native WebView | Electron | CEF | Saucer |
|---------|---------------|----------|-----|---------|
| Binary Size | ~1-2MB | ~150MB | ~100MB | ~150MB |
| Memory Usage | Low | High | High | Medium |
| Startup Time | Fast | Slow | Medium | Medium |
| Dependencies | System only | Bundled | Bundled | Many |
| Auto Updates | OS updates | Manual | Manual | Manual |

## Current Implementation Status

- ✅ **macOS**: Fully implemented with WKWebView
- ⚠️ **Windows**: Structure in place, needs WebView2 implementation
- ⚠️ **Linux**: Structure in place, needs WebKitGTK implementation

## Contributing

To implement Windows or Linux support:

1. See `src/platform/windows/webview_windows.cpp` for Windows WebView2 integration
2. See `src/platform/linux/webview_linux.cpp` for Linux WebKitGTK integration
3. Follow the same pattern as the macOS implementation

## License

MIT License - feel free to use in your projects.

## Examples

Check out these projects using native WebView approaches:
- [VS Code](https://github.com/microsoft/vscode) - Uses Electron but considering native WebView
- [Tauri](https://github.com/tauri-apps/tauri) - Rust-based native WebView framework
- [WebView](https://github.com/webview/webview) - Minimal WebView library (what we improved upon)
