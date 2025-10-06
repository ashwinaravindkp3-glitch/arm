
#include <stdint.h>

/* --- RCC Registers --- */
#define RCC_AHB1ENR     (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR     (*(volatile uint32_t*)0x40023840)

/* --- GPIOA Registers --- */
#define GPIOA_MODER     (*(volatile uint32_t*)0x40020000)
#define GPIOA_AFRL      (*(volatile uint32_t*)0x40020020)

/* --- TIM3 Registers (PWM) --- */
#define TIM3_CR1        (*(volatile uint32_t*)0x40000400)
#define TIM3_CCMR1      (*(volatile uint32_t*)0x40000418)
#define TIM3_CCER       (*(volatile uint32_t*)0x40000420)
#define TIM3_CNT        (*(volatile uint32_t*)0x40000424)
#define TIM3_PSC        (*(volatile uint32_t*)0x40000428)
#define TIM3_ARR        (*(volatile uint32_t*)0x4000042C)
#define TIM3_CCR1       (*(volatile uint32_t*)0x40000434)

/* --- TIM2 Registers (for delay) --- */
#define TIM2_CR1        (*(volatile uint32_t*)0x40000000)
#define TIM2_SR         (*(volatile uint32_t*)0x40000010)
#define TIM2_EGR        (*(volatile uint32_t*)0x40000014)
#define TIM2_PSC        (*(volatile uint32_t*)0x40000028)
#define TIM2_ARR        (*(volatile uint32_t*)0x4000002C)

/* --- TIM2-based millisecond delay --- */
void timer2_delay_ms(uint32_t ms)
{
    RCC_APB1ENR |= 0x1;        // Ensure TIM2 clock enabled

    TIM2_PSC = 16000 - 1;      // 16 MHz / 16000 = 1 kHz -> 1 ms tick
    TIM2_ARR = ms;             // Auto-reload = number of ms
    TIM2_EGR = 1;              // Update event
    TIM2_SR = 0;               // Clear update flag
    TIM2_CR1 = 1;              // Start timer

    while ((TIM2_SR & 1) == 0); // Wait for UIF
    TIM2_CR1 = 0;              // Stop timer
    TIM2_SR = 0;               // Clear flag
}

int main(void)
{
    /* --- 1) Enable Clocks --- */
    RCC_AHB1ENR |= 0x1;     // GPIOA clock enable
    RCC_APB1ENR |= 0x2;     // TIM3 clock enable

    /* --- 2) Configure PA6 as Alternate Function --- */
    GPIOA_MODER &= ~(0x3 << (6*2));
    GPIOA_MODER |=  (0x2 << (6*2));  // AF mode

    GPIOA_AFRL &= ~(0xF << (6*4));
    GPIOA_AFRL |=  (0x2 << (6*4));   // AF2 -> TIM3_CH1
    remove all the coments 
    /* --- 3) Configure TIM3 for PWM --- */
    TIM3_PSC = 1600 - 1;     // 16 MHz / 1600 = 10 kHz timer clock
    TIM3_ARR = 1000 - 1;     // 10 kHz / 1000 = 10 Hz PWM (~100 ms period)
    TIM3_CCR1 = 500;         // 50% duty cycle

    TIM3_CCMR1 &= ~0xFF;     // Clear OC1M, OC1PE
    TIM3_CCMR1 |= (0x6 << 4); // PWM mode 1
    TIM3_CCMR1 |= (1 << 3);   // Preload enable

    TIM3_CCER |= 1;          // Enable CH1 output
    TIM3_CR1 |= (1 << 7);    // ARPE = 1
    TIM3_CR1 |= 1;           // CEN = 1

    while(1)
    {
        timer2_delay_ms(1000);
        TIM3_CCR1 = 250;  // Change duty cycle to 25%
        timer2_delay_ms(1000);
        TIM3_CCR1 = 750;  // Change duty cycle to 75%
    }
}
