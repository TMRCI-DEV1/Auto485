#ifndef Auto485_h
#define Auto485_h

#include <stdio.h>
#include <string>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

class Auto485
{
public:
    Auto485(const uint DE_pin, uart_inst_t *uart_instance = uart0, const uint RE_pin = (uint)-1);
    enum Mode
    {
        TX,
        RX
    };

    void set_mode(Mode);
    size_t write(const uint8_t *buf, size_t len);
    void flush(void);
    void begin(unsigned long baudrate, uint8_t bits = 8, uint8_t stop_bits = 1, uart_parity_t parity = UART_PARITY_NONE);
    void end(void);
    int available(void);
    int peek(void);
    int read(uint8_t *buf, size_t len);
    
    size_t println(const std::string &s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = 10);
    size_t println(int, int = 10);
    size_t println(unsigned int, int = 10);
    size_t println(long, int = 10);
    size_t println(unsigned long, int = 10);
    size_t println(double, int = 2);
    size_t println();

private:
    uint _DE_pin;
    uint _RE_pin;
    Mode _mode;
    uart_inst_t *_uart;

    size_t print(const std::string &s);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = 10);
    size_t print(int, int = 10);
    size_t print(unsigned int, int = 10);
    size_t print(long, int = 10);
    size_t print(unsigned long, int = 10);
    size_t print(double, int = 2);
};

#endif