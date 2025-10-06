
#include <stdint.h>

#define RCC_AHB1ENR     (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR     (*(volatile uint32_t*)0x40023840)
#define RCC_APB2ENR     (*(volatile uint32_t*)0x40023844)

#define GPIOA_MODER     (*(volatile uint32_t*)0x40020000)
#define GPIOA_OTYPER    (*(volatile uint32_t*)0x40020004)
#define GPIOA_OSPEEDR   (*(volatile uint32_t*)0x40020008)
#define GPIOA_PUPDR     (*(volatile uint32_t*)0x4002000C)
#define GPIOA_BSRR      (*(volatile uint32_t*)0x40020018)

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

#define NVIC_ISER0      (*(volatile uint32_t*)0xE000E100)

#define THRESHOLD 2000  

void timer2_delay_ms(uint32_t ms)
{
    TIM2_PSC = 16000 - 1;  
    TIM2_ARR = ms;
    TIM2_EGR = 1;          
    TIM2_SR = 0;           
    TIM2_CR1 = 1;        
    while ((TIM2_SR & 1) == 0);
    TIM2_CR1 = 0;          
    TIM2_SR = 0;           
}

void setup(void)
{
 
    RCC_AHB1ENR = RCC_AHB1ENR | 0x1;   
    RCC_APB2ENR = RCC_APB2ENR | 0x100; 
    RCC_APB1ENR = RCC_APB1ENR | 0x1;  
    GPIOA_MODER = (GPIOA_MODER & ~(3 << (5*2))) | (1 << (5*2));
    GPIOA_OTYPER = GPIOA_OTYPER & ~(1 << 5);   
    GPIOA_OSPEEDR = GPIOA_OSPEEDR & ~(3 << (5*2)); 
    GPIOA_PUPDR = GPIOA_PUPDR & ~(3 << (5*2)); 
    GPIOA_MODER = (GPIOA_MODER & ~(3 << (0*2))) | (3 << (0*2));
    ADC1_CR1 = 0;          
    ADC1_CR2 = 0;           
    ADC1_SMPR2 = (ADC1_SMPR2 & ~7) | 7; 
    ADC1_SQR3 = 0;          
    ADC1_CR1 = ADC1_CR1 | (1 << 5);  
    ADC1_CR2 = ADC1_CR2 | 1;        
    NVIC_ISER0 = NVIC_ISER0 | (1 << 18);
}

void ADC_IRQHandler(void)
{
    if ((ADC1_SR & 0x2) != 0) {  
        uint32_t value = ADC1_DR; 
        if (value > THRESHOLD) {
            GPIOA_BSRR = (1 << 5);       
        } else {
            GPIOA_BSRR = (1 << (5+16)); 
        }
    }
}

int main(void)
{
    setup();
    timer2_delay_ms(2); 
    while (1) {
        ADC1_CR2 = ADC1_CR2 | (1 << 30); 
        timer2_delay_ms(200);           
    }
}
