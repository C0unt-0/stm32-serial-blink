#include "app/input/double_press_detector.hpp"

#include <assert.h>

int main()
{
    app::DoublePressDetector detector{2000U};

    assert(!detector.register_press(100U));
    assert(detector.register_press(2100U));

    assert(!detector.register_press(3000U));
    assert(!detector.register_press(5001U));
    assert(detector.register_press(6000U));

    detector.reset();
    assert(!detector.register_press(10U));

    return 0;
}
