#include <stdint.h>

#define RCC_AHB1ENR     (*(volatile uint32_t*)0x40023830)

#define GPIOA_MODER     (*(volatile uint32_t*)0x40020000)
#define GPIOA_OTYPER    (*(volatile uint32_t*)0x40020004)
#define GPIOA_OSPEEDR   (*(volatile uint32_t*)0x40020008)
#define GPIOA_PUPDR     (*(volatile uint32_t*)0x4002000C)
#define GPIOA_IDR       (*(volatile uint32_t*)0x40020010)
#define GPIOA_ODR       (*(volatile uint32_t*)0x40020014)
#define GPIOA_BSRR      (*(volatile uint32_t*)0x40020018)
#define GPIOA_LCKR      (*(volatile uint32_t*)0x4002001C)
#define GPIOA_AFRL      (*(volatile uint32_t*)0x40020020)
#define GPIOA_AFRH      (*(volatile uint32_t*)0x40020024)

#define GPIOC_MODER     (*(volatile uint32_t*)0x40020800)
#define GPIOC_OTYPER    (*(volatile uint32_t*)0x40020804)
#define GPIOC_OSPEEDR   (*(volatile uint32_t*)0x40020808)
#define GPIOC_PUPDR     (*(volatile uint32_t*)0x4002080C)
#define GPIOC_IDR       (*(volatile uint32_t*)0x40020810)
#define GPIOC_ODR       (*(volatile uint32_t*)0x40020814)
#define GPIOC_BSRR      (*(volatile uint32_t*)0x40020818)
#define GPIOC_LCKR      (*(volatile uint32_t*)0x4002081C)
#define GPIOC_AFRL      (*(volatile uint32_t*)0x40020820)

int main()
{

    RCC_AHB1ENR |= (5U << 0);
    GPIOA_MODER &= ~(3U << (5 * 2));
    GPIOA_MODER |=  (1U << (5 * 2));
    GPIOC_MODER &= ~(3U << (13 * 2));
    GPIOA_BSRR = (1U << (5+16));
    while (1)
        {
            if (!(GPIOC_IDR & (1U << 13))) {

                GPIOA_BSRR = (1U << 5);
            } else {

                GPIOA_BSRR = (1U << (5 + 16));
            }
        }

}
