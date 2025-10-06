
#include <stdint.h>


#define RCC_AHB1ENR     (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR     (*(volatile uint32_t*)0x40023840)
#define RCC_APB2ENR     (*(volatile uint32_t*)0x40023844)

#define GPIOA_MODER     (*(volatile uint32_t*)0x40020000)
#define GPIOA_OTYPER    (*(volatile uint32_t*)0x40020004)
#define GPIOA_OSPEEDR   (*(volatile uint32_t*)0x40020008)
#define GPIOA_PUPDR     (*(volatile uint32_t*)0x4002000C)
#define GPIOA_ODR       (*(volatile uint32_t*)0x40020014)

#define ADC1_SR         (*(volatile uint32_t*)0x40012000)
#define ADC1_CR1        (*(volatile uint32_t*)0x40012004)
#define ADC1_CR2        (*(volatile uint32_t*)0x40012008)
#define ADC1_SMPR2      (*(volatile uint32_t*)0x40012010)
#define ADC1_SQR3       (*(volatile uint32_t*)0x40012034)
#define ADC1_DR         (*(volatile uint32_t*)0x4001204C)

#define TIM2_CR1        (*(volatile uint32_t*)0x40000000)
#define TIM2_SR         (*(volatile uint32_t*)0x40000010)
#define TIM2_EGR        (*(volatile uint32_t*)0x40000014)
#define TIM2_PSC        (*(volatile uint32_t*)0x40000028)
#define TIM2_ARR        (*(volatile uint32_t*)0x4000002C)

#define ADC_CR2_ADON    (1U << 0)
#define ADC_CR2_SWSTART (1U << 30)
#define ADC_SREOC      (1U << 1)

#define THRESHOLD       2000  

void timer2_delay_ms(uint32_t ms)
{
    TIM2_PSC = 16000 - 1;      
    TIM2_ARR = ms;             
    TIM2_EGR |= 1;            
    TIM2_SR &= ~(1U<<0);      
    TIM2_CR1 |= (1U<<0);   
    while (!(TIM2_SR & (1U<<0))); 
    TIM2_CR1 &= ~(1U<<0);     
    TIM2_SR &= ~(1U<<0);        

int main(void)
{
    RCC_AHB1ENR |= (1U << 0);   
    RCC_APB2ENR |= (1U << 8);   
    RCC_APB1ENR |= (1U << 0);   
    GPIOA_MODER &= ~(3U << (5 * 2));
    GPIOA_MODER |=  (1U << (5 * 2));
    GPIOA_OTYPER &= ~(1U << 5);       
    GPIOA_OSPEEDR &= ~(3U << (5 * 2));
    GPIOA_PUPDR &= ~(3U << (5 * 2));  
    GPIOA_MODER |= (3U << (0 * 2));   
    ADC1_SQR3 = 0;          
    ADC1_SMPR2 |= (3U << 0); 
    ADC1_CR2 |= (1U<<0); 
    while (1)
    {
        ADC1_CR2 |= (1U<<30);
        while (!(ADC1_SR & (1U<<1)));
        uint32_t adc_val = ADC1_DR;
        if (adc_val > THRESHOLD) {
            GPIOA_ODR |= (1U << 5);  
        } else {
            GPIOA_ODR &= ~(1U << 5); 
        }
        timer2_delay_ms(500); 
    }
}
