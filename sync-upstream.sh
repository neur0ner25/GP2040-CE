#!/usr/bin/env bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$DIR"

echo "=================================================="
echo "  1. Синхронизация чистой ветки main с апстримом"
echo "=================================================="
git fetch origin main
git checkout main
git reset --hard origin/main
git push fork main

echo ""
echo "=================================================="
echo "  2. Обновление персональной ветки waveshare-dongle"
echo "=================================================="
git checkout waveshare-dongle
git merge main -m "merge: sync with upstream main"
git push fork waveshare-dongle

echo ""
echo "=================================================="
echo "  СИНХРОНИЗАЦИЯ УСПЕШНО ЗАВЕРШЕНА!"
echo "  Обе ветки (main и waveshare-dongle) на GitHub"
echo "  обновлены. Теперь можно запустить ./build-dongle.sh"
echo "=================================================="
