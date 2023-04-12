#include "Auto485.h"
#include <cstring>
#include <sstream>

Auto485::Auto485(const uint DE_pin, uart_inst_t *uart_instance, const uint RE_pin)
    : _DE_pin(DE_pin), _RE_pin(RE_pin == -1 ? DE_pin : RE_pin), _uart(uart_instance), _mode(RX)
{
    gpio_init(_DE_pin);
    gpio_set_dir(_DE_pin, GPIO_OUT); // defaults to LOW = read mode
    if (_DE_pin != _RE_pin)
    {
        gpio_init(_RE_pin);
        gpio_set_dir(_RE_pin, GPIO_OUT); // defaults to LOW = read mode
    }
}

void Auto485::set_mode(Mode mode)
{
    if (_mode == TX && mode == RX)
        flush(); // finish writing if we're moving into RX mode

    _mode = mode;
    gpio_put(_DE_pin, _mode == TX); // high if TX, low if RX
    if (_DE_pin != _RE_pin)
        gpio_put(_RE_pin, _mode == TX); // high if TX, low if RX
}

void Auto485::begin(unsigned long baudrate, uint8_t bits, uint8_t stop_bits, uart_parity_t parity)
{
    uart_init(_uart, baudrate);
    uart_set_format(_uart, bits, stop_bits, parity);
    uart_set_fifo_enabled(_uart, true);
}

void Auto485::end(void)
{
    uart_deinit(_uart);
}

int Auto485::available(void)
{
    return uart_is_readable(_uart) ? 1 : 0;
}

int Auto485::peek(void)
{
    // The peek functionality is not available in the Pico SDK
    return -1;
}

int Auto485::read(uint8_t *buf, size_t len)
{
    uart_read_blocking(_uart, buf, len);
    return len;
}

size_t Auto485::write(const uint8_t *buf, size_t len)
{
    if (_mode != TX)
        set_mode(TX);
    uart_write_blocking(_uart, buf, len);
    return len;
}

void Auto485::flush(void)
{
    while (uart_is_writable(_uart))
        ;
    set_mode(RX);
}

// Added println functions
size_t Auto485::println(const char *str)
{
    size_t written = write((const uint8_t *)str, strlen(str));
    written += write((const uint8_t *)"\r\n", 2);
    return written;
}

size_t Auto485::println(int num, int base)
{
    std::stringstream ss;
    if (base == 16)
        ss << std::hex;
    else if (base == 8)
        ss << std::oct;
    ss << num;
    return println(ss.str().c_str());
}

size_t Auto485::println(unsigned int num, int base)
{
    std::stringstream ss;
    if (base == 16)
        ss << std::hex;
    else if (base == 8)
        ss << std::oct;
    ss << num;
    return println(ss.str().c_str());
}

size_t Auto485::println(long num, int base)
{
    std::stringstream ss;
    if (base == 16)
        ss << std::hex;
    else if (base == 8)
        ss << std::oct;
    ss << num;
    return println(ss.str().c_str());
}

size_t Auto485::println(unsigned long num, int base)
{
    std::stringstream ss;
    if (base == 16)
    ss << std::hex;
else if (base == 8)
    ss << std::oct;
ss << num;
return println(ss.str().c_str());
}

size_t Auto485::println(double num, int digits)
{
std::stringstream ss;
ss.setf(std::ios::fixed);
ss.precision(digits);
ss << num;
return println(ss.str().c_str());
}

size_t Auto485::println()
{
return write((const uint8_t *)"\r\n", 2);
}