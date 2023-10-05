#include "../include/main.h"
#include "../include/dio.h"
#include "../include/buzzer.h"
#include  "../include/ws2812.h"


extern note_struct turnON[];
extern note_struct wish[];

void blink_led();
static const uint8_t PIN_PWM = 14u;
uint  slice_num;

int main() {

    stdio_init_all();    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_set_function(PIN_PWM, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(PIN_PWM); 

    leds_init();
    buttons_init();


    // todo get free sm
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    xTaskCreate(blink_led, "Blink_led_task", 256, NULL, 1, NULL);
    xTaskCreate(led_ws2812, "Led_WS2812 Controll", 256, NULL, 1, NULL);

    vTaskStartScheduler();
    return 0;
}

void blink_led(){

    play_melody(slice_num, turnON, 200, 3);
    vTaskDelay(500 / portTICK_PERIOD_MS);


    while (true) {
        play_melody(slice_num, wish, 200, 30);
        printf("Set ON LED\n");
        test();
        gpio_put(LED_PIN, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        printf("Set OFF LED\n");     
        //gpio_put(LED_PIN, 0);  
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void test(){

        bool test1;
        bool test2;
        bool test3; 

        test1 = gpio_get(BLACK_ONE_BUTTON_PIN);
        test2 = gpio_get(BLACK_TWO_BUTTON_PIN);
        test3 = gpio_get(BLACK_THREE_BUTTON_PIN);

        if (test1) gpio_put(LED_BUTTON_1, 1);
        else gpio_put(LED_BUTTON_1, 0);

        if (test2) gpio_put(LED_BUTTON_2, 1);
        else gpio_put(LED_BUTTON_2, 0);

        if (test3) gpio_put(LED_BUTTON_3, 1);
        else gpio_put(LED_BUTTON_3, 0);

}
