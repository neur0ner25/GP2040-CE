# ProBridge-S2

<p align="center">
  <b>Dedicated Ultra-Low-Latency Hardware Bridge: 8BitDo (USB Host) → Nintendo Switch 2 Pro Controller</b>
  <br />
  <i>Optimized bare-metal firmware for Raspberry Pi RP2040 / Waveshare RP2040-Zero</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Polling%20Rate-1000%20Hz%20(1ms)-success" />
  <img src="https://img.shields.io/badge/Mode-Nintendo%20Switch%202%20Pro-blue" />
  <img src="https://img.shields.io/badge/Input-8BitDo%202.4G%20%2F%20USB-orange" />
  <img src="https://img.shields.io/badge/Firmware%20Size-315%20KB-brightgreen" />
  <img src="https://img.shields.io/badge/Flash%20Binary-164%20KB-blueviolet" />
  <img src="https://img.shields.io/badge/License-MIT-green" />
</p>

---

## Overview / О проекте

**ProBridge-S2** — это высокопроизводительная, монолитная bare-metal прошивка для микроконтроллера **RP2040** (целевая плата: **Waveshare RP2040-Zero**), созданная для подключения беспроводных (2.4G-ресивер) и проводных геймпадов **8BitDo** к консоли **Nintendo Switch / Switch 2** в качестве нативного проводного **Switch 2 Pro Controller** (`057E:2069`).

В отличие от универсальных комбайнов, из **ProBridge-S2** полностью вырезаны все неиспользуемые компоненты: веб-интерфейс, стек TCP/IP/lwIP, HTTP-сервер, эмуляция нерелевантных консолей (PS3/4/5, Xbox, Retro), экранные меню, анимационные движки и неиспользуемые аддоны (удалено более **23 400 строк мертвого кода**, исходники сокращены до компактных 20 файлов).

Преимущества архитектуры:
- **Мгновенный старт (0 мс)** без задержек на сетевые сервисы или сканирование экранов.
- **Чистый цикл 1000 Гц (1 мс)**: нулевой инпут-лаг, нулевой джиттер, отсутствие динамических аллокаций памяти в куче во время игры.
- **Ультракомпактный размер**: итоговый `.uf2` уменьшен с 2.6 МБ (в универсальных сборках) до **315 КБ** (всего 164 КБ чистого бинарного кода во flash).
- **Сборка за 1 секунду**.

---

## Key Features / Ключевые возможности

### 1. Нативный протокол Nintendo Switch 2 Pro Controller
- Полная эмуляция дескрипторов и структуры HID-репортов оригинального контроллера Nintendo Switch 2 (`057E:2069`).
- Частота опроса **1000 Гц (1 мс интервал)**.
- Корректная обработка всех субкоманд консоли:
  - `0x01` (NFC / Amiibo handshake)
  - `0x02` (Flash Memory Read: калибровки стиков, гироскопа, триггеров GameCube)
  - `0x04` (Set Max Report Length)
  - `0x08` (Charge Grip status)
  - `0x10` (Firmware Info)
  - `0x21` / `0x80` (Pairing, Bluetooth LTK Cryptographic Challenge через аппаратный AES-128 mbedTLS)

### 2. Математическая компенсация стиков 8BitDo Hall / TMR
Датчики Холла и туннельного магнитосопротивления (TMR) в геймпадах 8BitDo подвержены межосевому магнитному затуханию при диагональных отклонениях ($45^\circ$). В **ProBridge-S2** интегрирована нелинейная эллиптическая коррекция:
$$r_{eff} = r \cdot (1 + 0.30 \sin^2(2\theta))$$
с ограничением радиуса $1.0$ и масштабированием под полный диапазон стиков Switch 2 ($1600$ вокруг центра $2048$). Это гарантирует идеальный круговой охват ($100\%$ диапазона) во всех играх без «срезания» диагоналей.

### 3. Физический асимметричный кроссовер HD Rumble → ERM
Прошивка на лету декодирует пакеты Switch 2 HD Rumble по спецификации драйвера ядра Linux `hid-nintendo`:
- **LF (Низкие частоты)**: направляются на тяжелый левый мотор эксцентриковой массы (ERM). Настроен стартовый овердрайв (30 мс) для преодоления сухого трения покоя (stiction) и сустейн 95 мс.
- **HF (Высокие частоты)**: направляются на легкий правый мотор. Быстрый импульс (15 мс) и сустейн 35 мс для передачи резких тактильных щелчков и текстур.

### 4. Статичная идентичность контроллера
Зафиксированы неизменный MAC-адрес (`7c:bb:8a:4a:93:2e`), постоянный серийный номер и ключ. Консоль никогда не теряет сопряжение и не создает дубликатов устройств.

### 5. Пользовательская калибровка во флеш-памяти
Выделен безопасный 256-байтный сектор флеш-памяти RP2040 (`0x1FC000`) для сохранения индивидуальной пользовательской калибровки осей стиков при вызове штатных команд калибровки в настройках Nintendo Switch.

---

## Hardware & Wiring / Схема подключения

Прошивка сконфигурирована для платы **Waveshare RP2040-Zero**:

```
                  ┌────────────────────────┐
                  │  Waveshare RP2040-Zero │
                  │                        │
  [USB-C к Switch]│ Native USB (Core 0)    │
                  │                        │
  [USB Host к     │ GP28 ──────> USB D+    │ (Pico-PIO-USB, Core 1)
   8BitDo Ресивер]│ GP29 ──────> USB D-    │
                  │ 5V   ──────> USB VBUS  │
                  │ GND  ──────> USB GND   │
                  │                        │
                  │ GP16 ──────> Onboard   │ (Статичный красный статус-LED)
                  │              RGB LED   │
                  └────────────────────────┘
```

* **Core 0**: Высокоприоритетный игровой конвейер (USB Host Pico-PIO-USB для 8BitDo + математическая коррекция стиков Hall/TMR + USB Device TinyUSB к консоли Nintendo Switch 2 с частотой 1000 Гц).
* **Core 1**: Вспомогательные задачи драйвера и управление встроенным статусным WS2812 RGB-светодиодом (GP16).

---

## Сборка прошивки / Build Instructions

### Требования
- Linux (Ubuntu / Debian / Arch / Fedora)
- `arm-none-eabi-gcc` и `arm-none-eabi-newlib`
- `cmake` (>= 3.10), `make`
- Установленный [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) (`pico-sdk`)

### Быстрая сборка
```bash
./build.sh
```

Готовый файл для прошивки:
```
build/ProBridge-S2_0.7.12_WaveshareZeroDongle.uf2
```

### Прошивка устройства
1. Зажмите кнопку **BOOT** на плате Waveshare RP2040-Zero и подключите её к компьютеру (или удерживайте хоткей перехода в BOOTSEL).
2. В системе появится съемный диск `RPI-RP2`.
3. Скопируйте файл `.uf2` на этот диск. Плата перезагрузится и сразу перейдет в режим работы.

---

## Credits & Acknowledgments / Благодарности и авторские права

Данный проект создан на базе и вдохновлен наработками следующих открытых проектов:

- **[GP2040-CE](https://github.com/OpenStickCommunity/GP2040-CE)** (Copyright © OpenStickCommunity, лицензия MIT). Мы выражаем искреннюю благодарность команде OpenStickCommunity за архитектуру и реализацию драйверов контроллеров под RP2040.
- **[TinyUSB](https://github.com/hathach/tinyusb)** (Copyright © Ha Thach).
- **[Pico-PIO-USB](https://github.com/sekigon-gonnoc/Pico-PIO-USB)** (Copyright © sekigon-gonnoc).
- **Linux Kernel `hid-nintendo` Driver** (Linux Foundation).

Лицензия проекта остается **MIT License**, в соответствии с исходными проектами.
