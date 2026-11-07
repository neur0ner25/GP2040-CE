#!/usr/bin/env python3
"""Compile and exercise the actual stick helpers on the host (requires a C++ compiler)."""
from pathlib import Path
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def definition(source, marker, suffix=""):
    start = source.index(marker)
    opening = source.index("{", start)
    depth = 0
    for pos in range(opening, len(source)):
        if source[pos] == "{":
            depth += 1
        elif source[pos] == "}":
            depth -= 1
            if depth == 0:
                return source[start:pos + 1] + suffix
    raise ValueError(marker)


switch = (ROOT / "src/drivers/switch2pro/Switch2ProDriver.cpp").read_text()
xbox = (ROOT / "src/hosts/Xbox360Host.cpp").read_text()
program = r"""
#include <cstdint>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#define __not_in_flash_func(x) x
""" + "\n".join((
    definition(switch, "struct PackedStickCache", ";"),
    definition(switch, "static void __not_in_flash_func(packStick)"),
    definition(switch, "static inline void packStickCached"),
    definition(xbox, "static inline void apply_radial_deadzone"),
)) + r"""
static void require(bool ok, const char *message) {
    if (!ok) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}
static void reference(int16_t x, int16_t y, int16_t dz, int16_t& ox, int16_t& oy) {
    if (dz <= 0) { ox = x; oy = y; return; }
    const uint64_t r2 = uint64_t(int64_t(x) * x) + uint64_t(int64_t(y) * y);
    if (r2 <= uint64_t(dz) * uint64_t(dz)) { ox = oy = 0; return; }
    const float fx = float(x), fy = float(y), mag = sqrtf(float(r2));
    const float scale = ((mag - float(dz)) / (32767.0f - float(dz))) * (32767.0f / mag);
    int32_t sx = int32_t(lroundf(fx * scale)), sy = int32_t(lroundf(fy * scale));
    if (sx > 32767) sx = 32767; else if (sx < -32768) sx = -32768;
    if (sy > 32767) sy = 32767; else if (sy < -32768) sy = -32768;
    ox = int16_t(sx); oy = int16_t(sy);
}
static void check_stick(PackedStickCache& cache, uint16_t x, uint16_t y) {
    uint8_t expected[3], actual[3];
    packStick(expected, x, y); packStickCached(cache, actual, x, y);
    require(std::memcmp(expected, actual, 3) == 0, "stick cache mismatch");
}
static void check_deadzone(int16_t x, int16_t y, int16_t dz) {
    int16_t ax, ay, bx, by;
    apply_radial_deadzone(x, y, dz, ax, ay); reference(x, y, dz, bx, by);
    require(ax == bx && ay == by, "deadzone mismatch");
}
int main() {
    for (unsigned i = 0; i < 3; ++i) {
        PackedStickCache cache;
        check_stick(cache, 65535, 65535); check_stick(cache, 65535, 65535);
        check_stick(cache, 0, 0); check_stick(cache, 0, 65535);
        check_stick(cache, 65535, 0); check_stick(cache, 32768, 32768);
        check_stick(cache, 32767, 32768); check_stick(cache, 32767, 32767);
    }
    check_deadzone(0, 0, 1500); check_deadzone(1499, 0, 1500);
    check_deadzone(1500, 0, 1500); check_deadzone(1501, 0, 1500);
    check_deadzone(-32768, -32768, 1500); check_deadzone(32767, 32767, 1500);
    uint32_t state = 123456789;
    for (unsigned i = 0; i < 200000; ++i) {
        state = state * 1664525u + 1013904223u; int16_t x = int16_t(state >> 16);
        state = state * 1664525u + 1013904223u; int16_t y = int16_t(state >> 16);
        check_deadzone(x, y, 1500); check_deadzone(x, y, 0);
    }
    std::puts("stick cache and 400000 randomized deadzone checks passed");
}
"""

with tempfile.TemporaryDirectory(prefix="probridge-tests-") as tmp:
    src = Path(tmp) / "stick_test.cpp"
    binary = Path(tmp) / "stick_test"
    src.write_text(program)
    subprocess.run([os.environ.get("CXX", "c++"), "-std=c++17", "-O2", "-fsanitize=undefined", str(src), "-o", str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
