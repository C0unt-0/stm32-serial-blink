// #include "app/application.hpp"
// #include "bsp/board.hpp"
// #include "config/firmware_config.hpp"
// #include "platform/stm32f407/system.hpp"

// extern "C" int main()
// {
//     platform::stm32f407::system::initialize(config::systick_reload);

//     drivers::Led led = bsp::make_user_led();
//     drivers::Button button = bsp::make_user_button();
//     drivers::Uart uart = bsp::make_console_uart();

//     app::Application application{led, button, uart};
//     application.initialize(platform::stm32f407::system::milliseconds());

//     for (;;) {
//         application.run_once(
//             platform::stm32f407::system::milliseconds());
//     }
// }
