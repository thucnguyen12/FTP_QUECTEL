#include "gsm_data_layer.h"
#include "gsm_ultilities.h"

static bool gprs_done = false;

uint8_t MAIN_step = 0;
uint8_t SETTING_step = 0;
uint8_t PUTTING_step = 0;
uint8_t GETTING_step = 0;
char cmd_temp[20];
char cmd_temp1[20];
char cmd_temp2[20];
/********   variable for handling task *************/



/*****************       GPRS SET UP     ******************/
void GPRS_setup (at_cmd_response_evt_t evt, void *resp_buffer)
{
    if (evt == AT_CMD_EVENT_TIMEOUT)
    {
        DEBUG_INFO("GPRS set up time out, reset module\r\n");
        MAIN_step = POWER_SET;
        return;
    }
    switch (SETTING_step)
    {
        case 0:
            gprs_done = false;
            DEBUG_INFO("GPRS is setting up\r\n");
            if (evt == AT_CMD_EVENT_OK)
            {
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd ("ATE0\r\n", "OK","", "ERROR", "", 3000, 3, GPRS_setup);
            break;
        case 1:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("ECHO OFF\r\n");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+CMEE=2\r\n", "OK","", "ERROR", "", 6000, 5, GPRS_setup); // ENABLE CME ERROR
            break;
        case 2:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("ENABLE CME ERROR \r\n");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd ("ATI\r\n", "","OK\r\n", "ERROR", "", 6000, 5, GPRS_setup);
            break;
        case 3:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("Get module info\r\n");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT\r\n", "", "OK\r\n", "ERROR", "", 6000, 5, GPRS_setup);
        case 4:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("AT test ok\r\n");
                SETTING_step++;
                
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT\r\n", "", "OK\r\n", "ERROR", "", 3000, 5, GPRS_setup);
            break;
        case 5:
            if (evt == AT_CMD_EVENT_OK)
            {
                 DEBUG_INFO("AT test 2 ok\r\n");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+QCFG=\"urc/ri/smsincoming\",\"pulse\",2000\r\n", "OK\r\n", "", "ERROR", "", 10000, 2, GPRS_setup);
            break;
        case 6:
            if (evt == AT_CMD_EVENT_OK)
            {
                 DEBUG_INFO("Set URC ringtype\r\n");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+CNMI=2,1,0,0,0\r\n", "", "OK\r\n", "ERROR", "", 10000, 3, GPRS_setup);
            break;
        case 7:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("Config SMS event report\r\n");
                SETTING_step++;
            }
            else
            {
                return;
            }
                
            gsm_hw_send_at_cmd("AT+CMGF=1\r\n", "", "OK\r\n", "ERROR", "", 5000, 5, GPRS_setup);
            break;
        case 8:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("Set SMS text mode\r\n");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT\r\n", "", "OK\r\n", "ERROR", "", 3000, 5, GPRS_setup);
            break;
        case 9:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ( "AT TEST OK");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+CGSN\r\n", "", "OK\r\n", "+CME ERROR", "", 8000, 5, GPRS_setup);
            break;
        case 10:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("GET CSGN \r\n");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+CIMI\r\n", "", "OK\r\n", "ERROR", "", 3000, 5, GPRS_setup);
            break;
        case 11:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("Get SIM IMSI:");
                SETTING_step++;
            }
            else
            {
                
                return;
            }
            gsm_hw_send_at_cmd("AT+QCCID\r\n", "", "OK\r\n", "ERROR", "", 3000, 3, GPRS_setup);
            break;
        case 12:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("GET QCCID");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+CPIN?\r\n", "+CPIN: READY\r\n", "OK\r\n", "ERROR", "", 5000, 3, GPRS_setup);
            break;
        case 13:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("CPIN : READY");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+QIDEACT=1\r\n", "", "OK\r\n", "ERROR", "", 3000, 3, GPRS_setup);
            break;
        case 14:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("De-activate PDP: OK\r\n");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT\r\n", "", "OK\r\n", "ERROR", "", 5000, 2, GPRS_setup);
            break;
        case 15:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("SCAN NETWORK MODE\r\n");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+CGDCONT=1,\"IP\",\"v-internet\"\r\n", "", "OK\r\n", "ERROR", "", 3000, 5, GPRS_setup);
            break;
        case 16:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("Define PDP context: OK");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+QIACT=1\r\n", "OK", "", "ERROR", "", 15000, 2, GPRS_setup);
            break;
        case 17:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("CSCS=GSM:OK");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+CGREG=2\r\n", "", "OK\r\n", "ERROR", "", 3000, 5, GPRS_setup);
            break;
        case 18:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("Network registration status:OK");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+CGREG?\r\n", "", "OK\r\n", "ERROR", "", 3000, 5, GPRS_setup);
            break;
        case 19:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("Query network status : OK");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+COPS?\r\n", "", "OK\r\n", "ERROR", "", 3000, 5, GPRS_setup);
            break;
        case 20:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("Query network operator: OK");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT\r\n", "OK\r\n", "", "ERROR", "", 3000, 5, GPRS_setup);
            break;
        case 21:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("AT TEST OK");
                SETTING_step++;
            }
            else
            {
                return;
            }
            gsm_hw_send_at_cmd("AT+CSQ\r\n", "", "OK\r\n", "ERROR", "", 3000, 5, GPRS_setup);
            break;
        case 22:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("CSQ GET");
                SETTING_step++;
            }
            else
            {
//                if (csq == 99) // Invalid CSQ =>> Polling CSQ again
                return;
            }
            gsm_hw_send_at_cmd("AT\r\n", "OK","", "ERROR", "", 3000, 5, GPRS_setup);
            break;
        case 23:            
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("GPRS OK\r\n");
                SETTING_step++;
                gprs_done = true;
            }
            else
            {
                return;
            }            
            break;
        default:
            break;
    }
}
/*****************       GPRS SET UP END    ******************/

/***************       PUTTING FILE         *******************/

void putFile (at_cmd_response_evt_t evt, void *resp_buffer)
{
    if (evt == AT_CMD_EVENT_TIMEOUT)
    {
        MAIN_step = POWER_SET; 
        PUTTING_step = 0;
        GETTING_step = 0;
        return;
    }
    switch (PUTTING_step)
    {
        case 0:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("PUTTING FUNCTION BEGIN\r\n");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QICSGP=1,1,\"v-internet\",\"\",\"\",1\r\n", "", "OK\r\n", "ERROR", "", 3000, 3, putFile);
            break;
        case 1:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("PDU CONTEXT OK\r\n");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT\r\n", "", "OK\r\n", "ERROR", "", 3000, 3, putFile);
            break;
        case 2:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("Active pdu\r\n");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QIACT?\r\n", "", "OK\r\n", "ERROR", "", 3000, 3, putFile);
            break;
        case 3:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("PDU INFO:");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCFG=\"contextid\",1\r\n", "", "OK\r\n", "ERROR", "", 3000, 3, putFile);
        case 4:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("context id ok\r\n");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCFG=\"account\",\"bytech\",\"abcd@1234\"\r\n", "", "OK\r\n", "ERROR", "", 3000, 3, putFile);
            break;
        case 5:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("account ok\r\n");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCFG=\"filetype\",1\r\n", "", "OK\r\n", "ERROR", "", 5000, 3, putFile);
            break;
        case 6:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("Set file type as binary ok\r\n");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCFG=\"transmode\",1\r\n", "", "OK\r\n", "ERROR", "", 6000, 5, putFile);////"AT+QFTPCFG=\"transmode\",0"
            break;
        case 7:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("passive  mode\r\n");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCFG=\"rsptimeout\",30\r\n", "OK", "", "", "", 6000, 3, putFile);
            break;
        case 8:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("time out response is 20\r\n");
                PUTTING_step ++;                
            }
            else 
            {
                DEBUG_ERROR ("Set timeout error\r\n");
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPOPEN=\"radiotech.vn\",2603\r\n", "+QFTPOPEN: 0,0\r\n", "", "+CME ERROR", "", 10000, 3, putFile);
            break;
        case 9:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("Connected to server\r\n");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCWD=\"/\"\r\n", "+QFTPCWD: 0,0\r\n", "", "", "", 10000, 3, putFile);
            break;
        case 10:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("file path ok \r\n");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPSTAT\r\n", "OK\r\n", "", "ERROR", "", 6000, 3, putFile);
            break;
        case 11:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("Get status ");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPPUT=\"test_my.txt\", \"COM:\",0,10,1\r\n", "CONNECT\r\n", "", "ERROR", "", 6000, 3, putFile);
            break;
        case 12:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("FILE NAME OK");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("1234567890\r\n", "+QFTPPUT: 0,", "", "ERROR", "", 6000, 3, putFile);
            break;
        case 13:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("UP LOADED DATA");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPLEN", "+QFTPLEN: 0,", "", "ERROR", "", 6000, 3, putFile);
            break;
        case 14:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("GET LENGTH TRANS");
                PUTTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPSIZE=\"test_my.txt\"", "+QFTPSIZE:", "", "ERROR", "", 6000, 3, putFile);
            break;
        case 15:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("GET size");
                PUTTING_step ++;
                
            }
            else
            {
                return;
            }
        default:
            DEBUG_INFO ("UPLOAD FINISH");
            break;
    }
}

/***************       PUTTING FILE END           *****************/

/************       GETTING FILE        *********/
void getFile (at_cmd_response_evt_t evt, void *resp_buffer)
{
//    uint8_t p[11];
    static int32_t file_handle;
//    uint32_t size;
    if (evt == AT_CMD_EVENT_TIMEOUT)
    {
        MAIN_step = POWER_SET;
        // debug print (time out)
        return;
    }
    switch (GETTING_step)
    {
        case 0:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("GETTING FUNCTION BEGIN\r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QICSGP=1,1,\"v-internet\",\"\",\"\",1\r\n", "OK\r\n", "", "ERROR", "", 6000, 5, getFile);
            break;
        case 1:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("PDU CONTEXT OK\r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT\r\n", "OK", "", "ERROR", "", 6000, 3, getFile);
            break;
        case 2:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO("Active pdu\r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QIACT?\r\n", "OK\r\n", "", "ERROR", "", 6000, 3, getFile);
            break;
        case 3:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("PDU INFO:");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCFG=\"contextid\",1\r\n", "OK\r\n", "", "ERROR", "", 6000, 3, getFile);
        case 4:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("context id ok\r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCFG=\"account\",\"bytech\",\"abcd@1234\"\r\n", "OK\r\n", "", "ERROR", "", 6000, 3, getFile);
            break;
        case 5:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("account ok\r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCFG=\"filetype\",1\r\n", "OK\r\n", "", "ERROR", "", 6000, 3, getFile);
            break;
        case 6:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("file type as binary");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCFG=\"transmode\",1\r\n", "OK\r\n", "", "ERROR", "", 6000, 3, getFile);
            break;
        case 7:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("passive  mode\r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCFG=\"rsptimeout\",30\r\n", "OK\r\n", "", "ERROR", "", 6000, 3, getFile);
            break;
        case 8:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("time out response is 20");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPOPEN=\"radiotech.vn\",2603\r\n", "+QFTPOPEN: 0,0", "", "+CME ERROR", "", 20000, 2, getFile);
            break;
        case 9:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("Connected to sever\r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPCWD=\"/\"\r\n", "+QFTPCWD: 0,0", "", "ERROR", "", 10000, 3, getFile);
            break;
        case 10:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("file path ok \r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPSTAT\r\n", "OK\r\n", "", "ERROR", "", 6000, 3, getFile);
            break;
        case 11:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("test ");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFTPGET=\"TEST2.txt\",\"UFS:test.txt\"\r\n", "+QFTPGET: 0,", "", "+CME ERROR", "", 7000, 3, getFile);
            break;
        case 12:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO (" get file OK");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFLST=\"UFS:*\"\r\n", "OK\r\n", "", "ERROR", "", 6000, 3, getFile);
            break;
        case 13:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("list file in UFS\r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFLDS=\"UFS\"\r\n", "OK", "", "ERROR", "", 6000, 3, getFile);
            break;
        case 14:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("get space from ram\r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            gsm_hw_send_at_cmd ("AT+QFOPEN=\"test.txt\",0\r\n", "+QFOPEN:", "", "ERROR", "", 15000, 2, getFile);
            break;
        case 15:
            if (evt == AT_CMD_EVENT_OK)
            {
                gsm_utilities_parse_file_handle(resp_buffer, &file_handle);
                DEBUG_INFO ("GET file handler\r\n");
                GETTING_step ++;       
            }
            else
            {
                return;
            }
            sprintf (cmd_temp, "AT+QFSEEK=%d,0,0\r\n", file_handle);
            gsm_hw_send_at_cmd (cmd_temp, "OK", "", "ERROR", "", 6000, 3, getFile);
        case 16:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("set the pointer file\r\n");
                GETTING_step ++;       
            }
            else
            {
                return;
            }
            sprintf (cmd_temp1, "AT+QFREAD=%d,10\r\n", file_handle);
            gsm_hw_send_at_cmd (cmd_temp1, "CONNECT", "OK\r\n", "ERROR", "", 30000, 2, getFile);
            break;
        case 17:
            {
                uint8_t *q = NULL;
                uint32_t q_size = 0;
                if (evt == AT_CMD_EVENT_OK)
                {
                    DEBUG_INFO ("read from ufs\r\n");
                    GETTING_step ++; 
                    
                    gsm_utilities_get_qfile_content (resp_buffer, &q, &q_size);
                    DEBUG_INFO("data nhan duoc la: %s\r\n", q);

                }
                else
                {
                    return;
                }  
            }
            sprintf (cmd_temp2, "AT+QFCLOSE=%d\r\n", file_handle);
            gsm_hw_send_at_cmd (cmd_temp2, "OK\r\n", "", "ERROR", "", 8000, 3, getFile);
            break;
        case 18:
            if (evt == AT_CMD_EVENT_OK)
            {
                DEBUG_INFO ("closed file\r\n");
                GETTING_step ++;                
            }
            else 
            {
                return;
            }
            DEBUG_INFO ("DOWNLOAD FINISH\r\n");
            break;
        default:
                       
            break;
    }
}
/************       GETTING FILE END         *********/

/************      STATE MACHINE FUNCTION          ************/
void gm_state_machine_polling(void)
{
    static uint32_t m_last_tick = 0;
    uint32_t now = HAL_GetTick();
    if (now - m_last_tick >= (uint32_t)1000) // Poll every 1 second
    {
        m_last_tick = now;
        switch (MAIN_step)
        {
            case POWER_SET:
                MAIN_step = POWER_SETTING;
                            
                break;
            case POWER_SETTING:
                hw_power_on ();
                break;
            case GPRS_SET:
                MAIN_step = GPRS_SETTIING;
                HAL_Delay (3000);
                gsm_hw_send_at_cmd ("ATV1\r\n", "OK\r\n", "", "ERROR", "", 3000, 5, GPRS_setup);
                break;
            case GPRS_SETTIING:
                if (gprs_done)
                {
                    MAIN_step = SETTING_DONE;
                    
                }
                break;
            case SETTING_DONE:
                MAIN_step = MAIN_step + 1;
                gsm_hw_send_at_cmd ("AT\r\n", "OK\r\n","", "", "", 1000, 2, getFile);
                break;
            default:
                break;        
        }
    }

}
/***************     POWER RESET           **********/
void hw_power_on (void)
{
    static uint8_t step = 0;
    switch (step)
    {
        case 0:
            
            HAL_GPIO_WritePin (GSM_PWR_EN_GPIO_Port, GSM_PWR_EN_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin (GSM_RESET_GPIO_Port, GSM_RESET_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin (GSM_PWR_KEY_GPIO_Port, GSM_PWR_KEY_Pin, GPIO_PIN_RESET);
            step++;
            break;
        case 1:
            HAL_GPIO_WritePin (GSM_RESET_GPIO_Port, GSM_RESET_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin (GSM_PWR_EN_GPIO_Port, GSM_PWR_EN_Pin, GPIO_PIN_SET);
            step++;
            DEBUG_INFO("Gsm power on\r\n");
            break;
        case 2:
            step ++;
            break;        
        case 3:
            DEBUG_INFO("Pulse power key\r\n");
            /* Generate pulse from (1-0-1) |_| to Power On module */
            HAL_GPIO_WritePin (GSM_PWR_KEY_GPIO_Port, GSM_PWR_KEY_Pin, GPIO_PIN_SET);
            step++;            
            break;
        case 4:
            HAL_GPIO_WritePin (GSM_PWR_KEY_GPIO_Port, GSM_PWR_KEY_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin (GSM_RESET_GPIO_Port, GSM_RESET_Pin, GPIO_PIN_RESET);
            step++;
            break;
        case 5: /// polling every 1 s => wait 2 second
        case 6:
            step ++;
            break;
        case 7:
            step = 0;
            DEBUG_INFO ("POWER ON DONE\r\n");                    
            MAIN_step = GPRS_SET; 
        default:
        break;
        
    }
}
/***************     POWER RESET end          **********/
