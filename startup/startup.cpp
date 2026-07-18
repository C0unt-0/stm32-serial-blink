#include <stdint.h>

extern "C" uint32_t _sbss;
extern "C" uint32_t _ebss;
extern "C" uint32_t _sdata;
extern "C" uint32_t _edata;
extern "C" uint32_t _sidata;

extern "C" int main();
extern "C" void Default_Handler() noexcept;

extern "C" void Reset_Handler() noexcept
{
    uint32_t *destination = &_sbss;

    while (destination < &_ebss) {
        *destination++ = 0U;
    }

    const uint32_t *source = &_sidata;
    destination = &_sdata;

    while (destination < &_edata) {
        *destination++ = *source++;
    }

    (void)main();
    Default_Handler();
}
