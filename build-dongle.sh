#!/usr/bin/env bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$DIR"

echo "=================================================="
echo "  Сборка прошивки WaveshareZeroDongle (GP2040-CE)"
echo "=================================================="

export PICO_SDK_PATH="${PICO_SDK_PATH:-/home/neur0ner/pico-sdk}"
GP2040_BOARDCONFIG=WaveshareZeroDongle cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"

echo ""
echo "=================================================="
echo "  СБОРКА УСПЕШНО ЗАВЕРШЕНА!"
echo "  Готовый файл для прошивки:"
echo "  $DIR/build/GP2040-CE_0.7.12_WaveshareZeroDongle.uf2"
echo "=================================================="
