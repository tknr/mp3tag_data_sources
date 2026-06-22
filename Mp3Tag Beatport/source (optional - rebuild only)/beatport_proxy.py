"""
Local Cloudflare-bypass proxy for Mp3tag Beatport scripts.

Mp3tag -> http://127.0.0.1:8787/<path>  ->  curl_cffi (Chrome impersonation)  ->  https://www.beatport.com/<path>

Runs in the system tray. Right-click the tray icon for Quit.
Auto-shuts down after IDLE_TIMEOUT seconds with no requests (set 0 to disable).
"""

import base64
import os
import sys
import time
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from io import BytesIO
from urllib.parse import urlsplit, urlunsplit, unquote

from curl_cffi import requests
import pystray
from PIL import Image

HOST = "127.0.0.1"
PORT = 8787
UPSTREAM = "https://www.beatport.com"
IMPERSONATE = "chrome124"
IDLE_TIMEOUT = 1800  # 30 minutes; set 0 to disable
REQUEST_TIMEOUT = 30

# Resolve install dir (PyInstaller --onefile: next to .exe; .py: next to script)
if getattr(sys, "frozen", False):
    SCRIPT_DIR = os.path.dirname(sys.executable)
else:
    SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
LOG_PATH = os.path.join(SCRIPT_DIR, "proxy.log")

_log_lock = threading.Lock()
_lock = threading.Lock()
_last_request = time.monotonic()
_server = None
_tray = None
session = requests.Session(impersonate=IMPERSONATE)


def log(msg):
    line = f"{time.strftime('%H:%M:%S')}  {msg}\n"
    with _log_lock:
        try:
            with open(LOG_PATH, "a", encoding="utf-8") as f:
                f.write(line)
        except Exception:
            pass


def touch():
    global _last_request
    with _lock:
        _last_request = time.monotonic()


def shutdown(reason=""):
    log(f"[exit] {reason}")
    if _server is not None:
        threading.Thread(target=_server.shutdown, daemon=True).start()
    if _tray is not None:
        _tray.stop()


def idle_watchdog():
    if IDLE_TIMEOUT <= 0:
        return
    while True:
        time.sleep(30)
        with _lock:
            idle = time.monotonic() - _last_request
        if idle > IDLE_TIMEOUT:
            shutdown(f"idle {idle:.0f}s > {IDLE_TIMEOUT}s")
            return


class Handler(BaseHTTPRequestHandler):
    def log_message(self, fmt, *args):
        pass  # silence default stderr logging

    def do_GET(self):
        touch()
        log(f"REQ  GET {self.path}")

        # Routing: /http(s)://... → use embedded URL; else → forward to UPSTREAM
        raw = self.path
        decoded = unquote(raw)
        if decoded.startswith("/http://") or decoded.startswith("/https://"):
            upstream_url = decoded[1:]
        else:
            parts = urlsplit(raw)
            upstream_url = UPSTREAM + urlunsplit(("", "", parts.path, parts.query, ""))

        try:
            r = session.get(upstream_url, timeout=REQUEST_TIMEOUT, allow_redirects=True)
        except Exception as e:
            log(f"ERR  upstream {upstream_url}: {e}")
            self.send_error(502, f"upstream error: {e}")
            return

        log(f"RESP {r.status_code}  {upstream_url}  bytes={len(r.content)}")

        body = r.content
        ctype = r.headers.get("content-type", "")
        if "text" in ctype or "json" in ctype or "html" in ctype:
            for needle in (b"https://www.beatport.com", b"http://www.beatport.com"):
                body = body.replace(needle, f"http://{HOST}:{PORT}".encode())

        self.send_response(r.status_code)
        skip = {"transfer-encoding", "content-encoding", "content-length", "connection", "set-cookie"}
        for k, v in r.headers.items():
            if k.lower() in skip:
                continue
            self.send_header(k, v)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)


ICON_PNG_B64 = (
    "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"
    "5KcAAAAASUVORK5CYII="
)


def make_icon():
    return Image.open(BytesIO(base64.b64decode(ICON_PNG_B64)))


def serve():
    global _server
    try:
        _server = ThreadingHTTPServer((HOST, PORT), Handler)
    except OSError as e:
        log(f"[fatal] bind {HOST}:{PORT} failed: {e}")
        if _tray is not None:
            _tray.stop()
        return
    log(f"Server listening on {HOST}:{PORT}  (impersonate={IMPERSONATE})")
    try:
        _server.serve_forever()
    except Exception as e:
        log(f"[server exit] {e}")


def on_quit(icon, item):
    shutdown("user quit")


def on_open_log(icon, item):
    try:
        os.startfile(LOG_PATH)
    except Exception as e:
        log(f"open_log failed: {e}")


def main():
    threading.Thread(target=serve, daemon=True).start()
    threading.Thread(target=idle_watchdog, daemon=True).start()

    global _tray
    _tray = pystray.Icon(
        "beatport_proxy",
        make_icon(),
        f"Beatport Proxy  ({HOST}:{PORT})",
        menu=pystray.Menu(
            pystray.MenuItem("Open log", on_open_log),
            pystray.MenuItem("Quit", on_quit),
        ),
    )
    _tray.run()


if __name__ == "__main__":
    main()
