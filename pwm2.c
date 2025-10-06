
#include <stdint.h>

/* --- RCC --- */
#define RCC_AHB1ENR     (*(volatile uint32_t*)0x40023830)
#define RCC_APB2ENR     (*(volatile uint32_t*)0x40023844)

/* --- GPIOA Registers --- */
#define GPIOA_MODER     (*(volatile uint32_t*)0x40020000)
#define GPIOA_AFRH      (*(volatile uint32_t*)0x40020024)

/* --- GPIOB Registers --- */
#define GPIOB_MODER     (*(volatile uint32_t*)0x40020400)
#define GPIOB_AFRH      (*(volatile uint32_t*)0x40020424)

/* --- TIM1 Registers --- */
#define TIM1_CR1        (*(volatile uint32_t*)0x40010000)
#define TIM1_CCMR1      (*(volatile uint32_t*)0x40010018)
#define TIM1_CCER       (*(volatile uint32_t*)0x40010020)
#define TIM1_CNT        (*(volatile uint32_t*)0x40010024)
#define TIM1_PSC        (*(volatile uint32_t*)0x40010028)
#define TIM1_ARR        (*(volatile uint32_t*)0x4001002C)
#define TIM1_CCR1       (*(volatile uint32_t*)0x40010034)
#define TIM1_BDTR       (*(volatile uint32_t*)0x40010044)

/* --- TIM2 for delay --- */
#define TIM2_CR1        (*(volatile uint32_t*)0x40000000)
#define TIM2_SR         (*(volatile uint32_t*)0x40000010)
#define TIM2_EGR        (*(volatile uint32_t*)0x40000014)
#define TIM2_PSC        (*(volatile uint32_t*)0x40000028)
#define TIM2_ARR        (*(volatile uint32_t*)0x4000002C)

/* TIM2-based delay in ms */
void timer2_delay_ms(uint32_t ms)
{
    /* Enable TIM2 clock */
    volatile uint32_t dummy = *((volatile uint32_t*)0x40023840);

    TIM2_PSC = 16000 - 1;  // 16 MHz / 16000 = 1 kHz -> 1 ms tick
    TIM2_ARR = ms;
    TIM2_EGR = 1;
    TIM2_SR = 0;
    TIM2_CR1 = 1;
    while ((TIM2_SR & 1) == 0); // wait for update
    TIM2_CR1 = 0;
    TIM2_SR = 0;
}

int main(void)
{
    /* --- 1) Enable Clocks --- */
    RCC_AHB1ENR |= 0x3;       // GPIOA + GPIOB
    RCC_APB2ENR |= 0x1;       // TIM1

    /* --- 2) Configure GPIOA PA8 as AF1 (TIM1_CH1) --- */
    GPIOA_MODER &= ~(0x3 << (8*2));
    GPIOA_MODER |=  (0x2 << (8*2));   // AF mode
    GPIOA_AFRH &= ~(0xF << ((8-8)*4));
    GPIOA_AFRH |=  (0x1 << ((8-8)*4)); // AF1 -> TIM1_CH1

    /* --- 3) Configure GPIOB PB13 as AF1 (TIM1_CH1N) --- */
    GPIOB_MODER &= ~(0x3 << (13*2));
    GPIOB_MODER |=  (0x2 << (13*2));  // AF mode
    GPIOB_AFRH &= ~(0xF << ((13-8)*4));
    GPIOB_AFRH |=  (0x1 << ((13-8)*4)); // AF1 -> TIM1_CH1N

    /* --- 4) Configure TIM1 --- */
    TIM1_PSC = 1600 - 1;      // 16 MHz / 1600 = 10 kHz timer clock
    TIM1_ARR = 1000 - 1;      // 10 kHz / 1000 = 10 Hz PWM
    TIM1_CCR1 = 500;          // 50% duty cycle

    /* PWM mode 1, enable preload for CH1 */
    TIM1_CCMR1 &= ~0xFF;      // clear OC1M and OC1PE
    TIM1_CCMR1 |= (0x6 << 4); // PWM mode 1
    TIM1_CCMR1 |= (1 << 3);   // OC1PE = 1 (preload)

    /* Enable CH1 and CH1N outputs */
    TIM1_CCER &= ~0xF;
    TIM1_CCER |= (1 << 0) | (1 << 2); // CC1E = 1, CC1NE = 1

    /* Dead-time and main output enable */
    TIM1_BDTR = 0x8000 | 50; // MOE = 1, dead-time = 50 (approx 5 us @16MHz/1600 PSC)

    /* Enable ARR preload and start timer */
    TIM1_CR1 |= (1 << 7);     // ARPE = 1
    TIM1_CR1 |= 1;            // CEN = 1

    while(1)
    {
        /* Optional: vary duty cycle dynamically */
        timer2_delay_ms(1000);
        TIM1_CCR1 = 250;  // 25% duty cycle
        timer2_delay_ms(1000);
        TIM1_CCR1 = 750;  // 75% duty cycle
    }
}
