#include <stdint.h>

#define RCC_AHB1ENR     (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR     (*(volatile uint32_t*)0x40023840)


#define GPIOA_MODER     (*(volatile uint32_t*)0x40020000)
#define GPIOA_OTYPER    (*(volatile uint32_t*)0x40020004)
#define GPIOA_OSPEEDR   (*(volatile uint32_t*)0x40020008)
#define GPIOA_PUPDR     (*(volatile uint32_t*)0x4002000C)
#define GPIOA_ODR       (*(volatile uint32_t*)0x40020014)
#define GPIOA_BSRR      (*(volatile uint32_t*)0x40020018)


#define TIM2_CR1        (*(volatile uint32_t*)0x40000000)
#define TIM2_SR         (*(volatile uint32_t*)0x40000010)
#define TIM2_EGR        (*(volatile uint32_t*)0x40000014)
#define TIM2_CCMR1      (*(volatile uint32_t*)0x40000018)
#define TIM2_CCER       (*(volatile uint32_t*)0x40000020)
#define TIM2_CNT        (*(volatile uint32_t*)0x40000024)
#define TIM2_PSC        (*(volatile uint32_t*)0x40000028)
#define TIM2_ARR        (*(volatile uint32_t*)0x4000002C)


#define TIM_SR_UIF      (1U << 0)   
#define TIM_CR1_CEN     (1U << 0)   

int main(void)
{
   
    RCC_AHB1ENR |= (1U << 0);

   
    RCC_APB1ENR |= (1U << 0);


    GPIOA_MODER &= ~(3U << (5 * 2));   
    GPIOA_MODER |=  (1U << (5 * 2));  
    GPIOA_OTYPER &= ~(1U << 5);        
    GPIOA_OSPEEDR &= ~(3U << (5 * 2)); 
    GPIOA_PUPDR &= ~(3U << (5 * 2));   


    TIM2_PSC = 16000 - 1;   
    TIM2_ARR = 5000 - 1;    
    TIM2_CNT = 0;           
    TIM2_EGR |= 1;        

    TIM2_SR &= ~TIM_SR_UIF; 
    TIM2_CR1 |= TIM_CR1_CEN;


    while (1)
    {
      
        while (!(TIM2_SR & TIM_SR_UIF));

        TIM2_SR &= ~TIM_SR_UIF;   

   
        GPIOA_ODR ^= (1U << 5);
    }
}
