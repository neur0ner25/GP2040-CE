#!/usr/bin/env bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$DIR"

echo "=================================================="
echo "  Сборка прошивки ProBridge-S2 (Waveshare RP2040-Zero)"
echo "=================================================="

export PICO_SDK_PATH="${PICO_SDK_PATH:-/home/neur0ner/pico-sdk}"

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"

UF2_FILE=$(ls "$DIR/build"/ProBridge-S2_*.uf2 2>/dev/null | head -n 1)

echo ""
echo "=================================================="
echo "  СБОРКА УСПЕШНО ЗАВЕРШЕНА!"
echo "  Готовый файл для прошивки:"
echo "  $UF2_FILE"
echo "  Размер: $(ls -lh "$UF2_FILE" | awk '{print $5}')"
echo "=================================================="
