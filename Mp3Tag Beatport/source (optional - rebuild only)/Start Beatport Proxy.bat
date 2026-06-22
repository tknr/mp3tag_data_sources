@echo off
cd /d "%~dp0"
title Beatport Proxy (close to stop)
".\.venv\Scripts\python.exe" beatport_proxy.py
pause
