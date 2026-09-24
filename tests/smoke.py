#!/usr/bin/env python3
"""Run against ./run.sh; verifies the actual HTTP responses and file bytes."""
import concurrent.futures
import hashlib
import http.client
from pathlib import Path
import socket
import time

ROOT = Path(__file__).resolve().parents[1]
PAGE = (ROOT / 'hello.html').read_bytes()


def request(path, headers=None, method='GET'):
    connection = http.client.HTTPConnection('127.0.0.1', 9999, timeout=10)
    connection.request(method, path, headers=headers or {})
    response = connection.getresponse()
    result = response.status, dict(response.getheaders()), response.read()
    connection.close()
    return result


for path in ('/', '/hello', '/hello?test=1'):
    status, headers, body = request(path)
    assert status == 200 and body == PAGE, (path, status)
    assert int(headers['Content-Length']) == len(body)
    assert headers['Connection'] == 'close'
print('PASS: home and hello pages match the source bytes')

for path in ('/missing', '/favicon.ico', '/hell', '/hello/missing', '/100Missing'):
    assert request(path)[0] == 404, path
assert request('/hello', method='POST')[0] == 405
print('PASS: unknown routes and unsupported methods return without hanging')

status, headers, body = request('/100M')
assert status == 200 and len(body) == 100 * 1024 * 1024
assert hashlib.sha256(body).digest() == hashlib.sha256((ROOT / 'build-local/100M').read_bytes()).digest()
for first, last in ((0, 0), (0, 1023), (5, 37), (100 * 1024 * 1024 - 8, 100 * 1024 * 1024 + 50)):
    status, headers, part = request('/100M', {'Range': f'bytes={first}-{last}'})
    assert status == 206 and part == body[first:last + 1]
    assert int(headers['Content-Length']) == len(part)
assert request('/100M', {'Range': 'bytes=200000000-200000001'})[0] == 416
print('PASS: 100 MiB download hash and inclusive byte ranges')

with socket.create_connection(('127.0.0.1', 9999), timeout=5) as sock:
    sock.sendall(b'GET /hel')
    time.sleep(0.05)
    sock.sendall(b'lo HTTP/1.1\r\nHost: localhost\r\n\r\n')
    result = b''
    while True:
        chunk = sock.recv(4096)
        if not chunk:
            break
        result += chunk
    assert result.split(b'\r\n\r\n', 1)[1] == PAGE
with concurrent.futures.ThreadPoolExecutor(max_workers=8) as executor:
    assert all(status == 200 and content == PAGE for status, _, content in executor.map(request, ['/hello'] * 50))
with socket.create_connection(('127.0.0.1', 9999), timeout=5) as sock:
    sock.sendall(b'GET /100M HTTP/1.1\r\nHost: localhost\r\n\r\n')
assert request('/hello')[0] == 200
print('PASS: fragmented requests, 50 concurrent requests, and interrupted download')

# Slow clients must not monopolize the event loop.
import subprocess
slow = subprocess.Popen(['curl', '--silent', '--limit-rate', '1m', '--max-time', '6',
                         '-o', '/dev/null', 'http://127.0.0.1:9999/100M'])
try:
    time.sleep(0.3)
    durations = []
    for _ in range(10):
        start = time.monotonic()
        assert request('/hello')[2] == PAGE
        durations.append(time.monotonic() - start)
    assert max(durations) < 1.0, durations
    assert slow.poll() is None, 'download must still be active during page requests'
    assert request('/hello', {'Range': 'bytes=7-37'})[2] == PAGE[7:38]
    print(f'PASS: slow download stays active; page max latency {max(durations)*1000:.1f} ms')
finally:
    slow.terminate()
    slow.wait()
