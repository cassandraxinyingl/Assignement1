@echo off
REM =============================================================================
REM build_web.bat – Build GLPIFrameworkIntro-Modern for WebGL (Emscripten/Windows)
REM
REM Prerequisites:
REM   1. Install Emscripten SDK.
REM   2. Edit EMSDK_PATH below if required.
REM   3. Run this file from the project root.
REM
REM Output:
REM   web/index.html
REM   web/index.js
REM   web/index.wasm
REM   web/index.data
REM
REM Run:
REM   cd web
REM   py -m http.server 8080
REM
REM Open:
REM   http://localhost:8080
REM =============================================================================

SET EMSDK_PATH=C:\emsdk
CALL "%EMSDK_PATH%\emsdk_env.bat"

IF NOT EXIST web MKDIR web

REM ============================================================================
REM GLFW backend (Recommended)
REM ============================================================================

emcc -std=c++17 ^
    Scene/main.cpp ^
    Scene/Renderer.cpp ^
    Scene/Triangle.cpp ^
    Scene/Square.cpp ^
    -IScene ^
    -DUSE_GLFW ^
    -s USE_GLFW=3 ^
    -s USE_WEBGL2=1 ^
    -s FULL_ES3=1 ^
    -s WASM=1 ^
    -s ALLOW_MEMORY_GROWTH=1 ^
    --preload-file android/app/src/main/assets/shader@assets/shader ^
    -o web/index.html

REM ============================================================================
REM SDL2 backend (Optional)
REM Uncomment this block and comment the GLFW block above if desired.
REM ============================================================================
REM emcc -std=c++17 ^
REM     Scene/main.cpp ^
REM     Scene/Renderer.cpp ^
REM     Scene/Triangle.cpp ^
REM     Scene/Square.cpp ^
REM     -IScene ^
REM     -s USE_SDL=2 ^
REM     -s USE_WEBGL2=1 ^
REM     -s FULL_ES3=1 ^
REM     -s WASM=1 ^
REM     -s ALLOW_MEMORY_GROWTH=1 ^
REM     --preload-file android/app/src/main/assets/shader@assets/shader ^
REM     -o web/index.html

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo Build FAILED.
    pause
    exit /b 1
)

echo.
echo ==========================================
echo Build succeeded.
echo ==========================================
echo.
echo Run:
echo     cd web
echo     py -m http.server 8080
echo.
echo Then open:
echo     http://localhost:8080
echo.
pause