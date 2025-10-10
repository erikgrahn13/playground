# Native WebView C++ Project

A lightweight, cross-platform C++ application for hosting React applications using native WebView components without external dependencies.

## Project Architecture

- **Cross-platform design**: Conditional compilation for macOS/Windows/Linux
- **Native WebView engines**: WKWebView (macOS), WebView2 (Windows), WebKitGTK (Linux)
- **Development workflow**: Debug mode connects to React dev server, Release mode serves embedded files
- **Zero external dependencies**: Only uses system-provided WebView libraries

## Key Implementation Details

- Uses PIMPL pattern for platform abstraction
- Objective-C++ for macOS WKWebView integration with ARC enabled
- CMake build system with platform-specific library linking
- Automatic mode switching based on CMAKE_BUILD_TYPE

## Development Notes

- macOS implementation is complete and functional
- Windows and Linux implementations are stubbed for future development
- Application creates a resizable window with integrated WebView
- Supports both development (localhost) and production (file://) URL schemes
