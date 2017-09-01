#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "utils/ustdlib.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"

#include "platform/board.h"

#include "simplelink.h"
#include "sl_common.h"
#include "sl_config.h"

#include "MQTTPacket.h"

extern _u32 g_SysClock;

// Driverlib error routine
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line)
{

}
#endif

// ADC0 configuration
void adc0_init(void)
{
        // Enable GPIO PE3 - AN0 pin
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

        // Enable ADC0
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

        // Wait for the peripherals to be ready
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0) && !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) ;

        // Set analog function to PE3 (AN0)
        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

        // ADC0 clock @32MHz (2msps)
        ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 15);

        // Enable Sampler Sequencer 3 - single sample
        // Triggered by the processor
        // Priority 0
        ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

        // Configure a step of the sample sequencer
        ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_END | ADC_CTL_IE);

        // Enables sample sequencer 3
        ADCSequenceEnable(ADC0_BASE, 3);
        ADCIntClear(ADC0_BASE, 3);
}


int main(int argc, char** argv)
{
        // ADC0 result buffer
        _u32 pui32ADC0Result[1];
        _i32 retVal = -1;

        retVal = initializeAppVariables();
        ASSERT_ON_ERROR(retVal);

        // Clk and WDT
        stopWDT();
        initClk();

        CLI_Configure();

        adc0_init();

        retVal = configureSimpleLinkToDefaultState();
        if(retVal < 0)
        {
                if (DEVICE_NOT_IN_STATION_MODE == retVal)
                        CLI_Write((_u8 *)" Failed to configure the device in its default state \n\r");

                LOOP_FOREVER();
        }

        CLI_Write((_u8 *)" Device is configured in default state \n\r");

        // device is in STA mode
        retVal = sl_Start(0, 0, 0);
        if ((retVal < 0) ||
            (ROLE_STA != retVal) )
        {
                CLI_Write((_u8 *)" Failed to start the device \n\r");
                LOOP_FOREVER();
        }

        CLI_Write((_u8 *)" Device started as STATION \n\r");

        // Connect to AP
        retVal = establishConnectionWithAP();
        if(retVal < 0)
        {
                CLI_Write((_u8 *)" Failed to establish connection w/ an AP \n\r");
                LOOP_FOREVER();
        }

        CLI_Write((_u8 *)" Connection established w/ AP and IP is acquired \n\r");

        retVal = checkLanConnection();
        if(retVal < 0)
        {
                CLI_Write((_u8 *)" Device couldn't connect to LAN \n\r");
                LOOP_FOREVER();
        }

        CLI_Write((_u8 *)" Device successfully connected to the LAN\r\n");

        // TCP socket
        _u16 sckt_id;
        _i16 sckt_status;

        SlSockAddrIn_t srv_addr;
        srv_addr.sin_family = SL_AF_INET; // IPv4
        srv_addr.sin_port = sl_Htons((_u16) TCP_SERVER_PORT);
        srv_addr.sin_addr.s_addr = sl_Htonl((_u32) TCP_SERVER_IP);

        // MQTT package
        MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
        _i32 rc = 0;
        _i8 buf[200];
        MQTTString topicString = MQTTString_initializer;
        _u8 payload[16];
        _i32 payloadlen;
        _i32 buflen = sizeof(buf);
        _i32 len = 0;

        data.clientID.cstring = "me";
        data.keepAliveInterval = 20;
        data.cleansession = 1;
        data.username.cstring = "";
        data.password.cstring = "";
        data.MQTTVersion = 4;

        topicString.cstring = "tiva/TM4C1294XL";

        // main loop
        _u32 counter = 0;
        while(counter < 30) { // 30 measures sent through mqtt

                // trigger adc0 conversion
                ADCProcessorTrigger(ADC0_BASE, 3);

                // gets the current raw interrupt status
                while(!ADCIntStatus(ADC0_BASE, 3, false)) ;

                // Clear the Interrupt Flag
                ADCIntClear(ADC0_BASE, 3);

                // Read the ADC0 value0
                ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Result);

                // Create the TCP socket
                sckt_id = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, 0); // IPv4/TCP

                // Connect to the socket
                sckt_status = sl_Connect(sckt_id, (SlSockAddr_t *) &srv_addr, sizeof(SlSockAddrIn_t));

                CLI_Write((_u8 *)" Socket was created successfully\r\n");

                // Format message
                usprintf(payload, "#%u: %u\r\n", counter, pui32ADC0Result[0]);
                payloadlen = strlen(payload);

                // MQTT
                memset(buf, 0, sizeof(buf));
                len  = MQTTSerialize_connect(buf, buflen, &data);
                len += MQTTSerialize_publish(buf + len, buflen - len, 0, 0, 0, 0, topicString, payload, payloadlen);
                len += MQTTSerialize_disconnect(buf + len, buflen - len);

                // Sending the value of the AN0 pin through the TCP socket
                sckt_status = sl_Send(sckt_id, buf, len, 0);
                counter++;

                // Delay
                CLI_Write((_u8 *)" MQTT packet sent!\r\n");

                // Close socket
                sl_Close(sckt_id);

                SysCtlDelay(g_SysClock);
        }

        return 0;
}
