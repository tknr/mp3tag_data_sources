@echo off
setlocal
cd /d "%~dp0"
title Beatport Proxy - Setup

REM ============================================================
REM  Setup.bat - prepares a local Python environment for
REM  building/running the Beatport proxy from source.
REM
REM  NOT needed if you just use the prebuilt beatport_proxy.exe.
REM
REM  What it does:
REM    1. Checks that Python 3.10+ is installed.
REM    2. Creates an isolated virtual environment (.venv) here.
REM    3. Installs the required packages into .venv:
REM         curl_cffi, pystray, pillow  (to run beatport_proxy.py)
REM         pyinstaller                 (to rebuild the .exe)
REM    4. Verifies the packages import correctly.
REM
REM  Safe to run again: existing .venv and packages are reused;
REM  only newer versions are pulled (pip --upgrade).
REM ============================================================

echo === Beatport Proxy Setup ===
echo.
echo This creates a local .venv and installs build/run dependencies.
echo (Not needed if you only use the prebuilt beatport_proxy.exe.)
echo.

REM 1. Check Python
where py >nul 2>nul
if errorlevel 1 (
    echo [ERROR] Python launcher 'py' not found.
    echo Install Python 3.10+ from https://www.python.org/downloads/
    echo or run:  winget install Python.Python.3.12
    echo.
    pause
    exit /b 1
)

py --version
echo.

REM 2. Create venv if missing
if exist ".venv\Scripts\python.exe" (
    echo [skip] .venv already exists.
) else (
    echo Creating .venv ...
    py -m venv .venv
    if errorlevel 1 (
        echo [ERROR] venv creation failed.
        pause
        exit /b 1
    )
)
echo.

REM 3. Install/upgrade dependencies
REM    Runtime (to run beatport_proxy.py): curl_cffi, pystray, pillow
REM    Build  (to rebuild the .exe):       pyinstaller
echo Installing dependencies into .venv ...
echo   (curl_cffi, pystray, pillow, pyinstaller)
".\.venv\Scripts\python.exe" -m pip install --upgrade --quiet curl_cffi pystray pillow pyinstaller
if errorlevel 1 (
    echo [ERROR] pip install failed.
    pause
    exit /b 1
)
echo.

REM 4. Verify
".\.venv\Scripts\python.exe" -c "import curl_cffi, pystray, PIL, PyInstaller; print('OK  curl_cffi', curl_cffi.__version__, '| pystray (ok) | pillow', PIL.__version__, '| pyinstaller', PyInstaller.__version__)"
if errorlevel 1 (
    echo [ERROR] dependency import failed.
    pause
    exit /b 1
)

echo.
echo === Setup complete ===
echo Run 'Start Beatport Proxy.bat' to launch the proxy.
echo.
pause
