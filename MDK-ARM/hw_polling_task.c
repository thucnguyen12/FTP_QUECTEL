#include "hw_polling_task.h"
#include "app_debug.h"
#include "ringBuffer.h"

                 
atc_t atc;
/********  AT CMD VAR  ********/

/*******         HARWARE HANDLER       *************/
void gsm_hw_polling_task(void)
{
    static uint32_t m_last_poll = 0;
    uint32_t now = HAL_GetTick();
    if ((now - m_last_poll) < (uint32_t)5)
    {
        return;
    }
    
    while (getByteFromRingBufferAvailableToRead(&ringBuff))
    {
        atc.recv_buff.buffer [atc.recv_buff.index ++] = readFromRingBuffer (&ringBuff);
//        DEBUG_INFO ("doc du lieu trong ring buffer \r\n");
    }
    
    if (atc.timeout_ms)
    {
        atc.current_timeout ++;
        if (atc.current_timeout > atc.timeout_ms)
        {
            atc.current_timeout = 0;
                            
            if (atc.retry_cnt)
            {
                atc.retry_cnt --;
            }
            if (atc.retry_cnt == 0)
            { 
                atc. timeout_ms = 0;
                if (atc.send_at_callback != NULL)
                {
                    atc.send_at_callback( AT_CMD_EVENT_TIMEOUT, NULL); /// viet ham reset he thong
                }
            }
            else
            {
                DEBUG_INFO ("RESEND ATC %d\r\n",atc.retry_cnt);
                uart_tx (USART1, (uint8_t *)atc.cmd, strlen(atc.cmd)); 
            }
        }
    }
    if (strstr ((char *) atc.recv_buff.buffer, atc.expected_response))
    {
        bool do_cb = true;
        if( atc.expected_response_at_the_end_of_response)
        {
            uint32_t current_response_length = atc.recv_buff.index;
            uint32_t expect_compare_length = strlen(atc.expected_response_at_the_end_of_response);
            uint8_t *p_compare_end_str = &atc.recv_buff.buffer[current_response_length - expect_compare_length];
            if (expect_compare_length <= current_response_length 
                && (memcmp(p_compare_end_str, atc.expected_response_at_the_end_of_response, expect_compare_length) == 0))
                {
                    do_cb = true;
                }
                else
                {
                    do_cb = false;
                }
        }
        if (do_cb)
        {
            atc.current_timeout = 0;
            if (atc.send_at_callback != NULL)
            {
                
//                if (strstr ((char *)(atc .recv_buff.buffer), "+FTPGET: 2,5"))
//                {
//                    strcpy (buff, (char *)atc.recv_buff.buffer);
//                    p = strstr (buff, "+FTPGET: 2,5\r\n");
//                    p = p + strlen ("+FTPGET: 2,5\r\n");
//                    p = strtok(p, "\r\n");
//                }                
                atc.send_at_callback (AT_CMD_EVENT_OK, atc.recv_buff.buffer);               
            }
        }
    }
    else if (atc.expected_error 
            && strstr((char *)(atc.recv_buff.buffer), atc.expected_error))
    {
        bool do_cb = true;
        if( atc.expected_error_at_the_end_of_response)
        {
            uint32_t current_response_length = atc.recv_buff.index;
            uint32_t expect_compare_length = strlen(atc.expected_error_at_the_end_of_response);
            uint8_t *p_compare_end_str = &atc.recv_buff.buffer[current_response_length - expect_compare_length];
            if (expect_compare_length <= current_response_length 
                    && (memcmp(p_compare_end_str, atc.expected_error_at_the_end_of_response, expect_compare_length) == 0))
                {
                    do_cb = true;
                }
                else
                {
                    do_cb = false;
                }
        }
        if (do_cb)
        {
            atc. current_timeout = 0;

            if (atc.send_at_callback != NULL)
            {
                atc.send_at_callback(AT_CMD_EVENT_ERROR, atc.recv_buff.buffer);
            }
        }
    }
//    memset (atc.recv_buff.buffer, '\0', GSM_ATC_BUFFER_SIZE);
    m_last_poll = HAL_GetTick();
} 
/*******         HARWARE HANDLER end      *************/
void gsm_hw_send_at_cmd(char *cmd,
                        char *expect_response,
                        char *expected_response_at_the_end_of_response,
                        char *expected_error,
                        char *expected_error_at_the_end_of_response,
                        uint16_t timeout_ms,
                        uint8_t retry_cnt,
                        gsm_send_at_cb_t callback)
{
    if (timeout_ms == 0 || callback == NULL)
    {
        uart_tx(USART1, (uint8_t *)cmd, strlen(cmd));
        return;
    }
#if 0
    DEBUG_PRINTF("%s", cmd);
    static uint32_t max_at_cmd_size = 0;
    if (strlen(cmd) > max_at_cmd_size)
    {
        max_at_cmd_size = strlen(cmd);
        DEBUG_PRINTF("AT cmd max size %d\r\n", max_at_cmd_size);
    }
#endif
  
    atc.cmd = cmd;
    atc.expected_response = expect_response;
    atc.expected_response_at_the_end_of_response = expected_response_at_the_end_of_response;
    atc.expected_error = expected_error;
    atc.expected_error_at_the_end_of_response = expected_error_at_the_end_of_response;
    atc.retry_cnt = retry_cnt;
    atc.send_at_callback = callback;
    atc.timeout_ms = timeout_ms / 5;
    atc.current_timeout = 0;
    atc.recv_buff.index = 0;
    memset (atc.recv_buff.buffer, '\0', GSM_ATC_BUFFER_SIZE);
    uart_tx (USART1, (uint8_t *)cmd, strlen(cmd));
    DEBUG_INFO ("TX =>> %s\r\n", cmd);
}
/**********    ASSIGN  AT CMD end           ***********/

