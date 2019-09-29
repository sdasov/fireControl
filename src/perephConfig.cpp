/*
 * perephConfig.cpp
 *
 *  Created on: 20 сент. 2019 г.
 *      Author: serg
 */
#include <stm32f103xb.h>


void RCCConfig(void)
{
//  //PLL
  RCC->CR |=  RCC_CR_HSEON;
  while(!(RCC->CR & RCC_CR_HSERDY));
  RCC->CFGR=RCC_CFGR_PLLMULL9|RCC_CFGR_PLLSRC|RCC_CFGR_ADCPRE_DIV8|RCC_CFGR_PPRE1_DIV2;
  RCC->CR|=RCC_CR_HSION;
  RCC->CR|=RCC_CR_PLLON; while(!(RCC->CR & RCC_CR_PLLRDY));


  //FLASH LATENCY
  FLASH->ACR|=FLASH_ACR_PRFTBE|FLASH_ACR_LATENCY_2;
  //Switch SYSCLK to PLL
  RCC->CFGR|=RCC_CFGR_SW_PLL;

  //APB2 Clock Enable 72 MHzz
  RCC->APB2ENR|=RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPBEN|RCC_APB2ENR_IOPCEN|RCC_APB2ENR_IOPDEN|RCC_APB2ENR_AFIOEN;
  //APB1 Clock Enable 36 MHz
  RCC->APB1ENR|=RCC_APB1ENR_I2C2EN|RCC_APB1ENR_TIM3EN|RCC_APB1ENR_TIM4EN;
  //RCC->APB1ENR|=RCC_APB1ENR_USART2EN|RCC_APB1ENR_USART3EN|RCC_APB1ENR_UART4EN|RCC_APB1ENR_UART5EN|RCC_APB1ENR_TIM2EN|RCC_APB1ENR_TIM3EN;



  //  //AHB Clock enable Ethernet;
//  RCC->AHBENR|=RCC_AHBENR_ETHMACEN|RCC_AHBENR_ETHMACTXEN|RCC_AHBENR_ETHMACRXEN;
//
//  //UART2 Remap, UART3 full Remap
//  AFIO->MAPR|=AFIO_MAPR_USART2_REMAP|AFIO_MAPR_USART3_REMAP_FULLREMAP;
//
}
// ----------------------------------------------------------------------------
void NVICConfig(void){
  //interrupt TIM2,TIM3
  //NVIC->ISER[0]=NVIC_ISER_SETENA_28|NVIC_ISER_SETENA_29;
//  NVIC->ISER[0]=(1<<28)|(1<<29);
	//TIM3, TIM4
  NVIC->ISER[0]=(1<<29)|(1<<30);

  //Interrupt enable USART1-USART5, ETH
  //NVIC->ISER[1]=NVIC_ISER_SETENA_5|NVIC_ISER_SETENA_6|NVIC_ISER_SETENA_7|NVIC_ISER_SETENA_20|NVIC_ISER_SETENA_21|NVIC_ISER_SETENA_29;
  NVIC->ISER[1]= (1<<5)|(1<<6)|(1<<7)|(1<<20)|(1<<21)|(1<<29);
}
// ----------------------------------------------------------------------------
void portConfig(void){
    //
    GPIOA->CRL= (0&GPIO_CRL_MODE0_1)|(0&GPIO_CRL_MODE0_0)|(0&GPIO_CRL_CNF0_1)|(~0&GPIO_CRL_CNF0_0)|\
                (0&GPIO_CRL_MODE1_1)|(0&GPIO_CRL_MODE1_0)|(0&GPIO_CRL_CNF1_1)|(~0&GPIO_CRL_CNF1_0)|\
                (0&GPIO_CRL_MODE2_1)|(0&GPIO_CRL_MODE2_0)|(0&GPIO_CRL_CNF2_1)|(~0&GPIO_CRL_CNF2_0)|\
                (0&GPIO_CRL_MODE3_1)|(0&GPIO_CRL_MODE3_0)|(0&GPIO_CRL_CNF3_1)|(~0&GPIO_CRL_CNF3_0)|\
                (0&GPIO_CRL_MODE4_1)|(0&GPIO_CRL_MODE4_0)|(0&GPIO_CRL_CNF4_1)|(~0&GPIO_CRL_CNF4_0)|\
                (0&GPIO_CRL_MODE5_1)|(0&GPIO_CRL_MODE5_0)|(0&GPIO_CRL_CNF5_1)|(~0&GPIO_CRL_CNF5_0)|\
                (0&GPIO_CRL_MODE6_1)|(0&GPIO_CRL_MODE6_0)|(0&GPIO_CRL_CNF6_1)|(~0&GPIO_CRL_CNF6_0)|\
                (0&GPIO_CRL_MODE7_1)|(0&GPIO_CRL_MODE7_0)|(0&GPIO_CRL_CNF7_1)|(~0&GPIO_CRL_CNF7_0);

    //PA8-RS,PA9-RW,PA10-E Display, PA13-SWDI, PA14-SWCLK, PA15-JTDI
    GPIOA->CRH= (0&GPIO_CRH_MODE8_1) |(~0&GPIO_CRH_MODE8_0) |(0&GPIO_CRH_CNF8_1) |(0&GPIO_CRH_CNF8_0) |\
                (0&GPIO_CRH_MODE9_1) |(~0&GPIO_CRH_MODE9_0) |(0&GPIO_CRH_CNF9_1) |(0&GPIO_CRH_CNF9_0) |\
                (0&GPIO_CRH_MODE10_1)|(~0&GPIO_CRH_MODE10_0)|(0&GPIO_CRH_CNF10_1)|(0&GPIO_CRH_CNF10_0)|\
                (0&GPIO_CRH_MODE11_1)|(0&GPIO_CRH_MODE11_0)|(0&GPIO_CRH_CNF11_1)|(~0&GPIO_CRH_CNF11_0)|\
                (0&GPIO_CRH_MODE12_1)|(0&GPIO_CRH_MODE12_0)|(0&GPIO_CRH_CNF12_1)|(~0&GPIO_CRH_CNF12_0)|\
                (0&GPIO_CRH_MODE13_1)|(0&GPIO_CRH_MODE13_0)|(0&GPIO_CRH_CNF13_1)|(~0&GPIO_CRH_CNF13_0)|\
                (0&GPIO_CRH_MODE14_1)|(0&GPIO_CRH_MODE14_0)|(0&GPIO_CRH_CNF14_1)|(~0&GPIO_CRH_CNF14_0)|\
                (0&GPIO_CRH_MODE15_1)|(0&GPIO_CRH_MODE15_0)|(0&GPIO_CRH_CNF15_1)|(~0&GPIO_CRH_CNF15_0);

    //PB3-ds18b20,PB4-NJTRST, PB4-PB7 - keyboard
    GPIOB->CRL= (0&GPIO_CRL_MODE0_1)|(0&GPIO_CRL_MODE0_0)|(0&GPIO_CRL_CNF0_1)|(~0&GPIO_CRL_CNF0_0)|\
                (0&GPIO_CRL_MODE1_1)|(0&GPIO_CRL_MODE1_0)|(0&GPIO_CRL_CNF1_1)|(~0&GPIO_CRL_CNF1_0)|\
                (0&GPIO_CRL_MODE2_1)|(0&GPIO_CRL_MODE2_0)|(0&GPIO_CRL_CNF2_1)|(~0&GPIO_CRL_CNF2_0)|\
                (0&GPIO_CRL_MODE3_1)|(0&GPIO_CRL_MODE3_0)|(0&GPIO_CRL_CNF3_1)|(~0&GPIO_CRL_CNF3_0)|\
                (0&GPIO_CRL_MODE4_1)|(0&GPIO_CRL_MODE4_0)|(~0&GPIO_CRL_CNF4_1)|(0&GPIO_CRL_CNF4_0)|\
                (0&GPIO_CRL_MODE5_1)|(0&GPIO_CRL_MODE5_0)|(~0&GPIO_CRL_CNF5_1)|(0&GPIO_CRL_CNF5_0)|\
                (0&GPIO_CRL_MODE6_1)|(0&GPIO_CRL_MODE6_0)|(~0&GPIO_CRL_CNF6_1)|(0&GPIO_CRL_CNF6_0)|\
                (0&GPIO_CRL_MODE7_1)|(0&GPIO_CRL_MODE7_0)|(~0&GPIO_CRL_CNF7_1)|(0&GPIO_CRL_CNF7_0);

    //PB8,PB9-ds18b20, PB10-SCL2, PB11-SDA2,PB12-PB15 - Display data
    GPIOB->CRH= (0&GPIO_CRH_MODE8_1) |(~0&GPIO_CRH_MODE8_0) |(0&GPIO_CRH_CNF8_1) |(0&GPIO_CRH_CNF8_0) |\
                (0&GPIO_CRH_MODE9_1) |(~0&GPIO_CRH_MODE9_0) |(0&GPIO_CRH_CNF9_1) |(0&GPIO_CRH_CNF9_0) |\
                (~0&GPIO_CRH_MODE10_1)|(0&GPIO_CRH_MODE10_0)|(~0&GPIO_CRH_CNF10_1)|(0&GPIO_CRH_CNF10_0)|\
                (~0&GPIO_CRH_MODE11_1)|(0&GPIO_CRH_MODE11_0)|(~0&GPIO_CRH_CNF11_1)|(0&GPIO_CRH_CNF11_0)|\
                (0&GPIO_CRH_MODE12_1)|(~0&GPIO_CRH_MODE12_0)|(0&GPIO_CRH_CNF12_1)|(0&GPIO_CRH_CNF12_0)|\
                (0&GPIO_CRH_MODE13_1)|(~0&GPIO_CRH_MODE13_0)|(0&GPIO_CRH_CNF13_1)|(0&GPIO_CRH_CNF13_0)|\
                (0&GPIO_CRH_MODE14_1)|(~0&GPIO_CRH_MODE14_0)|(0&GPIO_CRH_CNF14_1)|(0&GPIO_CRH_CNF14_0)|\
                (0&GPIO_CRH_MODE15_1)|(~0&GPIO_CRH_MODE15_0)|(0&GPIO_CRH_CNF15_1)|(0&GPIO_CRH_CNF15_0);

    //keyboard button to down
    GPIOB->BSRR=GPIO_BSRR_BR4|GPIO_BSRR_BR5|GPIO_BSRR_BR6|GPIO_BSRR_BR7;
    //DS18B20 - to up
    GPIOB->BSRR=GPIO_BSRR_BS3|GPIO_BSRR_BS8|GPIO_BSRR_BS9;


    //
    GPIOC->CRL= (0&GPIO_CRL_MODE0_1)|(0&GPIO_CRL_MODE0_0)|(0&GPIO_CRL_CNF0_1)|(~0&GPIO_CRL_CNF0_0)|\
                (0&GPIO_CRL_MODE1_1)|(0&GPIO_CRL_MODE1_0)|(0&GPIO_CRL_CNF1_1)|(~0&GPIO_CRL_CNF1_0)|\
                (0&GPIO_CRL_MODE2_1)|(0&GPIO_CRL_MODE2_0)|(0&GPIO_CRL_CNF2_1)|(~0&GPIO_CRL_CNF2_0)|\
                (0&GPIO_CRL_MODE3_1)|(0&GPIO_CRL_MODE3_0)|(0&GPIO_CRL_CNF3_1)|(~0&GPIO_CRL_CNF3_0)|\
                (0&GPIO_CRL_MODE4_1)|(0&GPIO_CRL_MODE4_0)|(0&GPIO_CRL_CNF4_1)|(~0&GPIO_CRL_CNF4_0)|\
                (0&GPIO_CRL_MODE5_1)|(0&GPIO_CRL_MODE5_0)|(0&GPIO_CRL_CNF5_1)|(~0&GPIO_CRL_CNF5_0)|\
                (0&GPIO_CRL_MODE6_1)|(0&GPIO_CRL_MODE6_0)|(0&GPIO_CRL_CNF6_1)|(~0&GPIO_CRL_CNF6_0)|\
                (0&GPIO_CRL_MODE7_1)|(0&GPIO_CRL_MODE7_0)|(0&GPIO_CRL_CNF7_1)|(~0&GPIO_CRL_CNF7_0);

    //PC13-LED
    GPIOC->CRH= (0&GPIO_CRH_MODE8_1) |(0&GPIO_CRH_MODE8_0) |(0&GPIO_CRH_CNF8_1) |(~0&GPIO_CRH_CNF8_0) |\
                (0&GPIO_CRH_MODE9_1) |(0&GPIO_CRH_MODE9_0) |(0&GPIO_CRH_CNF9_1) |(~0&GPIO_CRH_CNF9_0) |\
                (0&GPIO_CRH_MODE10_1)|(0&GPIO_CRH_MODE10_0)|(0&GPIO_CRH_CNF10_1)|(~0&GPIO_CRH_CNF10_0)|\
                (0&GPIO_CRH_MODE11_1)|(0&GPIO_CRH_MODE11_0)|(0&GPIO_CRH_CNF11_1)|(~0&GPIO_CRH_CNF11_0)|\
                (0&GPIO_CRH_MODE12_1)|(0&GPIO_CRH_MODE12_0)|(0&GPIO_CRH_CNF12_1)|(~0&GPIO_CRH_CNF12_0)|\
                (~0&GPIO_CRH_MODE13_1)|(0&GPIO_CRH_MODE13_0)|(0&GPIO_CRH_CNF13_1)|(~0&GPIO_CRH_CNF13_0)|\
                (0&GPIO_CRH_MODE14_1)|(0&GPIO_CRH_MODE14_0)|(0&GPIO_CRH_CNF14_1)|(~0&GPIO_CRH_CNF14_0)|\
                (0&GPIO_CRH_MODE15_1)|(0&GPIO_CRH_MODE15_0)|(0&GPIO_CRH_CNF15_1)|(~0&GPIO_CRH_CNF15_0);

    //
    GPIOD->CRL= (0&GPIO_CRL_MODE0_1)|(0&GPIO_CRL_MODE0_0)|(0&GPIO_CRL_CNF0_1)|(~0&GPIO_CRL_CNF0_0)|\
                (0&GPIO_CRL_MODE1_1)|(0&GPIO_CRL_MODE1_0)|(0&GPIO_CRL_CNF1_1)|(~0&GPIO_CRL_CNF1_0)|\
                (0&GPIO_CRL_MODE2_1)|(0&GPIO_CRL_MODE2_0)|(0&GPIO_CRL_CNF2_1)|(~0&GPIO_CRL_CNF2_0)|\
                (0&GPIO_CRL_MODE3_1)|(0&GPIO_CRL_MODE3_0)|(0&GPIO_CRL_CNF3_1)|(~0&GPIO_CRL_CNF3_0)|\
                (0&GPIO_CRL_MODE4_1)|(0&GPIO_CRL_MODE4_0)|(0&GPIO_CRL_CNF4_1)|(~0&GPIO_CRL_CNF4_0)|\
                (0&GPIO_CRL_MODE5_1)|(0&GPIO_CRL_MODE5_0)|(0&GPIO_CRL_CNF5_1)|(~0&GPIO_CRL_CNF5_0)|\
                (0&GPIO_CRL_MODE6_1)|(0&GPIO_CRL_MODE6_0)|(0&GPIO_CRL_CNF6_1)|(~0&GPIO_CRL_CNF6_0)|\
                (0&GPIO_CRL_MODE7_1)|(0&GPIO_CRL_MODE7_0)|(0&GPIO_CRL_CNF7_1)|(~0&GPIO_CRL_CNF7_0);


    //
    GPIOD->CRH= (0&GPIO_CRH_MODE8_1) |(0&GPIO_CRH_MODE8_0) |(0&GPIO_CRH_CNF8_1) |(~0&GPIO_CRH_CNF8_0) |\
                (0&GPIO_CRH_MODE9_1) |(0&GPIO_CRH_MODE9_0) |(0&GPIO_CRH_CNF9_1) |(~0&GPIO_CRH_CNF9_0) |\
                (0&GPIO_CRH_MODE10_1)|(0&GPIO_CRH_MODE10_0)|(0&GPIO_CRH_CNF10_1)|(~0&GPIO_CRH_CNF10_0)|\
                (0&GPIO_CRH_MODE11_1)|(0&GPIO_CRH_MODE11_0)|(0&GPIO_CRH_CNF11_1)|(~0&GPIO_CRH_CNF11_0)|\
                (0&GPIO_CRH_MODE12_1)|(0&GPIO_CRH_MODE12_0)|(0&GPIO_CRH_CNF12_1)|(~0&GPIO_CRH_CNF12_0)|\
                (0&GPIO_CRH_MODE13_1)|(0&GPIO_CRH_MODE13_0)|(0&GPIO_CRH_CNF13_1)|(~0&GPIO_CRH_CNF13_0)|\
                (0&GPIO_CRH_MODE14_1)|(0&GPIO_CRH_MODE14_0)|(0&GPIO_CRH_CNF14_1)|(~0&GPIO_CRH_CNF14_0)|\
                (0&GPIO_CRH_MODE15_1)|(0&GPIO_CRH_MODE15_0)|(0&GPIO_CRH_CNF15_1)|(~0&GPIO_CRH_CNF15_0);
}

void initI2C2()
{
    I2C2->CR2 &= ~I2C_CR2_FREQ;
    I2C2->CR2 |= 36;                 //APB1 = 36MHz
    I2C2->CCR &= ~I2C_CCR_CCR;
    I2C2->CCR = 180;                 //(36MHz/100KHz/2)
    I2C2->TRISE = 37;                //(1mcs/(1/36MHz)+1)
    I2C2->CR1 |= I2C_CR1_PE;
}



