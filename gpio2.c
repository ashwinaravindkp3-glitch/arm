#include <stdint.h>


#define RCC_AHB1ENR     (*(volatile uint32_t*)0x40023830)
#define RCC_APB2ENR     (*(volatile uint32_t*)0x40023844)


#define GPIOA_MODER     (*(volatile uint32_t*)0x40020000)
#define GPIOA_OTYPER    (*(volatile uint32_t*)0x40020004)
#define GPIOA_OSPEEDR   (*(volatile uint32_t*)0x40020008)
#define GPIOA_PUPDR     (*(volatile uint32_t*)0x4002000C)
#define GPIOA_IDR       (*(volatile uint32_t*)0x40020010)
#define GPIOA_ODR       (*(volatile uint32_t*)0x40020014)
#define GPIOA_BSRR      (*(volatile uint32_t*)0x40020018)


#define GPIOC_MODER     (*(volatile uint32_t*)0x40020800)
#define GPIOC_PUPDR     (*(volatile uint32_t*)0x4002080C)
#define GPIOC_IDR       (*(volatile uint32_t*)0x40020810)


#define SYSCFG_EXTICR4  (*(volatile uint32_t*)0x40013814)


#define EXTI_IMR        (*(volatile uint32_t*)0x40013C00)
#define EXTI_RTSR       (*(volatile uint32_t*)0x40013C08)
#define EXTI_PR         (*(volatile uint32_t*)0x40013C14)


#define NVIC_ISER1      (*(volatile uint32_t*)0xE000E104)

void EXTI15_10_IRQHandler(void);

int main(void)
{

    RCC_AHB1ENR |= (1U << 0);
    RCC_AHB1ENR |= (1U << 2);


    RCC_APB2ENR |= (1U << 14);


    GPIOA_MODER &= ~(3U << (5 * 2));
    GPIOA_MODER |=  (1U << (5 * 2));


    GPIOC_MODER &= ~(3U << (13 * 2));
    GPIOC_PUPDR &= ~(3U << (13 * 2));


    SYSCFG_EXTICR4 &= ~(0xF << 4);
    SYSCFG_EXTICR4 |=  (0x2 << 4);


    EXTI_IMR  |= (1U << 13);
    EXTI_RTSR |= (1U << 13);
    NVIC_ISER1 |= (1U << (40 - 32));

    while (1)
    {

    }
}


void EXTI15_10_IRQHandler(void)
{
    if (EXTI_PR & (1U << 13))
    {
        GPIOA_ODR ^= (1U << 5);
        EXTI_PR |= (1U << 13);
    }
}
