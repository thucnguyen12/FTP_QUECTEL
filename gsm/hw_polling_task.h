#ifndef HW_POLLING_TASK
#define HW_POLLING_TASK

#include "main.h"
#include <string.h>
#include <stdbool.h>
#define GSM_ATC_BUFFER_SIZE             (256)   // AT command buffer size
typedef struct
{
    uint8_t buffer[GSM_ATC_BUFFER_SIZE];
    uint16_t index;
} gsm_atc_buffer_t;

typedef enum
{
    AT_CMD_EVENT_OK = 0,
    AT_CMD_EVENT_TIMEOUT,
    AT_CMD_EVENT_ERROR,
    AT_CMD_EVENT_NOTHING        // dont care to this event
} at_cmd_response_evt_t;
typedef void (*gsm_send_at_cb_t)(at_cmd_response_evt_t evt, void *response_buffer);
typedef struct
{
    char *cmd;
    char *expected_response;
    char *expected_response_at_the_end_of_response;
    char *expected_error;
    char *expected_error_at_the_end_of_response;
    uint16_t timeout_ms;
    uint16_t current_timeout;
    uint8_t retry_cnt;
    gsm_atc_buffer_t recv_buff;
    gsm_send_at_cb_t send_at_callback;
} atc_t;
//*********    typedef for send at command       ********/
static uint32_t m_last_poll = 0;
//*********    var for polling       ********/
void gsm_hw_polling_task(void);
/**************   hardware handle  ******************/   
void gsm_hw_send_at_cmd(char *cmd,
                        char *expect_response,
                        char *expected_response_at_the_end_of_response,
                        char *expected_error,
                        char *expected_error_at_the_end_of_response,
                        uint16_t timeout_ms,
                        uint8_t retry_cnt,
                        gsm_send_at_cb_t callback);
/**************   send at command parameter  ******************/
void hw_power_on (void);
/*************        POWER RESET FUNCTION    ****************/                        
                     
extern atc_t atc;
/********  AT CMD VAR  ********/


                        
                        
                        
#endif




