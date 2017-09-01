// GCC vector table modified from scanlime's tiva c tm4c vectors.c file
// Copyright 2017 Micah Elizabeth Scott

#include <stdint.h>
#include <string.h>
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/rom_map.h"
#include "utils/uartstdio.h"

// extern void UARTStdioIntHandler(void);

// Default Fault Handlers
void startup(void);
static void nmi_int(void);
static void fault_int(void);
static void no_interrupt(void);


// GPIOM Int Handler (CC3100)
void GPIOM_intHandler();


// Main fnct
extern int main(void);


// Global variables
uint32_t stack_segment[4096];

extern uint32_t _etext;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;


// Def types
typedef void (*ivt_t)(void);


// Vector table
__attribute__ ((section(".isr_vector"))) const ivt_t g_intVectorTable[] = {
        (ivt_t) ((uintptr_t)stack_segment + sizeof stack_segment), // Stack pointer
        startup,                       // Start up
        nmi_int,                       // The NMI
        fault_int,                     // The hard fault
        no_interrupt,                  // The MPU fault
        no_interrupt,                  // The bus fault
        no_interrupt,                  // The usage fault
        0,                             // Reserved
        0,                             // Reserved
        0,                             // Reserved
        0,                             // Reserved
        no_interrupt,                  // SVCall
        no_interrupt,                  // Debug monitor
        0,                             // Reserved
        no_interrupt,                  // The PendSV
        no_interrupt,                  // The SysTick
        no_interrupt,                  // GPIO Port A
        no_interrupt,                  // GPIO Port B
        no_interrupt,                  // GPIO Port C
        no_interrupt,                  // GPIO Port D
        no_interrupt,                  // GPIO Port E
        no_interrupt,                  // UART0 Rx and Tx
        no_interrupt,                  // UART1 Rx and Tx
        no_interrupt,                  // SSI0 Rx and Tx
        no_interrupt,                  // I2C0 Master and Slave
        no_interrupt,                  // PWM Fault
        no_interrupt,                  // PWM Generator 0
        no_interrupt,                  // PWM Generator 1
        no_interrupt,                  // PWM Generator 2
        no_interrupt,                  // Quadrature Encoder 0
        no_interrupt,                  // ADC Sequence 0
        no_interrupt,                  // ADC Sequence 1
        no_interrupt,                  // ADC Sequence 2
        no_interrupt,                  // ADC Sequence 3
        no_interrupt,                  // Watchdog timer
        no_interrupt,                  // Timer 0 subtimer A
        no_interrupt,                  // Timer 0 subtimer B
        no_interrupt,                  // Timer 1 subtimer A
        no_interrupt,                  // Timer 1 subtimer B
        no_interrupt,                  // Timer 2 subtimer A
        no_interrupt,                  // Timer 2 subtimer B
        no_interrupt,                  // Analog Comparator 0
        no_interrupt,                  // Analog Comparator 1
        no_interrupt,                  // Analog Comparator 2
        no_interrupt,                  // System Control (PLL, OSC, BO)
        no_interrupt,                  // FLASH Control
        no_interrupt,                  // GPIO Port F
        no_interrupt,                  // GPIO Port G
        no_interrupt,                  // GPIO Port H
        no_interrupt,                  // UART2 Rx and Tx
        no_interrupt,                  // SSI1 Rx and Tx
        no_interrupt,                  // Timer 3 subtimer A
        no_interrupt,                  // Timer 3 subtimer B
        no_interrupt,                  // I2C1 Master and Slave
        no_interrupt,                  // CAN0
        no_interrupt,                  // CAN1
        no_interrupt,                  // Ethernet
        no_interrupt,                  // Hibernate
        no_interrupt,                  // USB0
        no_interrupt,                  // PWM Generator 3
        no_interrupt,                  // uDMA Software Transfer
        no_interrupt,                  // uDMA Error
        no_interrupt,                  // ADC1 Sequence 0
        no_interrupt,                  // ADC1 Sequence 1
        no_interrupt,                  // ADC1 Sequence 2
        no_interrupt,                  // ADC1 Sequence 3
        no_interrupt,                  // External Bus Interface 0
        no_interrupt,                  // GPIO Port J
        no_interrupt,                  // GPIO Port K
        no_interrupt,                  // GPIO Port L
        no_interrupt,                  // SSI2 Rx and Tx
        no_interrupt,                  // SSI3 Rx and Tx
        no_interrupt,                  // UART3 Rx and Tx
        no_interrupt,                  // UART4 Rx and Tx
        no_interrupt,                  // UART5 Rx and Tx
        no_interrupt,                  // UART6 Rx and Tx
        no_interrupt,                  // UART7 Rx and Tx
        no_interrupt,                  // I2C2 Master and Slave
        no_interrupt,                  // I2C3 Master and Slave
        no_interrupt,                  // Timer 4 subtimer A
        no_interrupt,                  // Timer 4 subtimer B
        no_interrupt,                  // Timer 5 subtimer A
        no_interrupt,                  // Timer 5 subtimer B
        no_interrupt,                  // FPU
        0,                             // Reserved
        0,                             // Reserved
        no_interrupt,                  // I2C4 Master and Slave
        no_interrupt,                  // I2C5 Master and Slave
        GPIOM_intHandler,              // GPIO Port M
        no_interrupt,                  // GPIO Port N
        0,                             // Reserved
        no_interrupt,                  // Tamper
        no_interrupt,                  // GPIO Port P
        no_interrupt,                  // GPIO Port P1
        no_interrupt,                  // GPIO Port P2
        no_interrupt,                  // GPIO Port P3
        no_interrupt,                  // GPIO Port P4
        no_interrupt,                  // GPIO Port P5
        no_interrupt,                  // GPIO Port P6
        no_interrupt,                  // GPIO Port P7
        no_interrupt,                  // GPIO Port Q
        no_interrupt,                  // GPIO Port Q1
        no_interrupt,                  // GPIO Port Q2
        no_interrupt,                  // GPIO Port Q3
        no_interrupt,                  // GPIO Port Q4
        no_interrupt,                  // GPIO Port Q5
        no_interrupt,                  // GPIO Port Q6
        no_interrupt,                  // GPIO Port Q7
        no_interrupt,                  // GPIO Port R
        no_interrupt,                  // GPIO Port S
        no_interrupt,                  // SHA/MD5 0
        no_interrupt,                  // AES 0
        no_interrupt,                  // DES3DES 0
        no_interrupt,                  // LCD Controller 0
        no_interrupt,                  // Timer 6 subtimer A
        no_interrupt,                  // Timer 6 subtimer B
        no_interrupt,                  // Timer 7 subtimer A
        no_interrupt,                  // Timer 7 subtimer B
        no_interrupt,                  // I2C6 Master and Slave
        no_interrupt,                  // I2C7 Master and Slave
        no_interrupt,                  // HIM Scan Matrix Keyboard 0
        no_interrupt,                  // One Wire 0
        no_interrupt,                  // HIM PS/2 0
        no_interrupt,                  // HIM LED Sequencer 0
        no_interrupt,                  // HIM Consumer IR 0
        no_interrupt,                  // I2C8 Master and Slave
        no_interrupt,                  // I2C9 Master and Slave
        no_interrupt                   // GPIO Port T
};

void startup(void){
        memcpy(&_data, &_etext, (uint32_t)&_edata - (uint32_t)&_data);
        memset(&_bss, 0, (uint32_t)&_ebss - (uint32_t)&_bss);
        MAP_FPUEnable();
        MAP_FPULazyStackingEnable();
        main();
        while(1) ;
}

// If processor receives and NMI, enter an infinite loop preserve the system state to debug
static void nmi_int(void){
        while(1) ;
}

// Fault Interrupt
static void fault_int(void){
        while(1) ;
}

// Unexpected Interrupt
static void no_interrupt(void){
        while(1) ;
}
