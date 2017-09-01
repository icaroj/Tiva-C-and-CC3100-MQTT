#pragma once

#define SL_STOP_TIMEOUT        0xFF

#define STATUS_BIT_PING_DONE  31

#define TCP_SERVER_IP   0xc0a81911 // 192.168.25.17

#define TCP_SERVER_PORT 1883

#define PING_INTERVAL   1000    // msecs

#define PING_TIMEOUT    3000    // msecs

#define PING_PKT_SIZE   20      // bytes

#define NO_OF_ATTEMPTS  3

#define IS_PING_DONE(status_variable)           GET_STATUS_BIT(status_variable, \
                                                               STATUS_BIT_PING_DONE)

/* Application specific status/error codes */
typedef enum {
        LAN_CONNECTION_FAILED = -0x7D0,                                                                   /* Choosing this number to avoid overlap with host-driver's error codes */
        INTERNET_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,
        DEVICE_NOT_IN_STATION_MODE = INTERNET_CONNECTION_FAILED - 1,

        STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;


// Global Vars
_u32 g_Status;
_u32 g_PingPacketsRecv;
_u32 g_GatewayIP;

// Fncts
_i32 configureSimpleLinkToDefaultState();
_i32 establishConnectionWithAP();
_i32 checkLanConnection();
_i32 initializeAppVariables();
