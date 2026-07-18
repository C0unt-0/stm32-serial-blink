#include "drivers/button/button_debouncer.hpp"

#include <assert.h>

int main()
{
    drivers::ButtonDebouncer button{20U};
    button.reset(false, 0U);

    assert(!button.update(true, 1U));
    assert(!button.update(false, 5U));
    assert(!button.update(true, 10U));
    assert(!button.update(true, 29U));
    assert(button.update(true, 30U));
    assert(button.is_pressed());
    assert(!button.update(true, 100U));

    assert(!button.update(false, 101U));
    assert(!button.update(false, 120U));
    assert(!button.update(false, 121U));
    assert(!button.is_pressed());

    assert(!button.update(true, 200U));
    assert(button.update(true, 220U));

    return 0;
}
