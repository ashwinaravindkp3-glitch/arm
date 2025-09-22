#include <stdint.h>


#define RCC_AHB1ENR     (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR     (*(volatile uint32_t*)0x40023840)


#define GPIOA_MODER     (*(volatile uint32_t*)0x40020000)
#define GPIOA_OTYPER    (*(volatile uint32_t*)0x40020004)
#define GPIOA_OSPEEDR   (*(volatile uint32_t*)0x40020008)
#define GPIOA_PUPDR     (*(volatile uint32_t*)0x4002000C)
#define GPIOA_ODR       (*(volatile uint32_t*)0x40020014)
#define GPIOA_BSRR      (*(volatile uint32_t*)0x40020018)


#define TIM3_CR1        (*(volatile uint32_t*)0x40000400)
#define TIM3_SR         (*(volatile uint32_t*)0x40000410)
#define TIM3_EGR        (*(volatile uint32_t*)0x40000414)
#define TIM3_CNT        (*(volatile uint32_t*)0x40000424)
#define TIM3_PSC        (*(volatile uint32_t*)0x40000428)
#define TIM3_ARR        (*(volatile uint32_t*)0x4000042C)


#define TIM_CR1_CEN     (1U << 0)
#define TIM_SR_UIF      (1U << 0)

void delay_ms(uint32_t delay)
{
    uint32_t maxDelay;
    TIM3_PSC = 16000 - 1;
    maxDelay = 0xFFFF;
    while (delay > 0)
    {
        uint32_t thisDelay;
        if (delay > maxDelay)
            thisDelay = maxDelay;
        else
            thisDelay = delay;
        TIM3_CR1 &= ~TIM_CR1_CEN;
        TIM3_ARR = thisDelay;
        TIM3_CNT = 0;
        TIM3_EGR |= 1;
        TIM3_SR &= ~TIM_SR_UIF;
        TIM3_CR1 |= TIM_CR1_CEN;
        while (!(TIM3_SR & TIM_SR_UIF));
        TIM3_SR &= ~TIM_SR_UIF;
        TIM3_CR1 &= ~TIM_CR1_CEN;
        delay -= thisDelay;
    }
}

int main(void)
{

    RCC_AHB1ENR |= (1U << 0);
    RCC_APB1ENR |= (1U << 1);
    GPIOA_MODER &= ~(3U << (5 * 2));
    GPIOA_MODER |=  (1U << (5 * 2));
    GPIOA_OTYPER &= ~(1U << 5);
    GPIOA_OSPEEDR &= ~(3U << (5 * 2));
    GPIOA_PUPDR &= ~(3U << (5 * 2));

    while (1)
    {
        GPIOA_ODR ^= (1U << 5);
        delay_ms(70000);
    }
}
