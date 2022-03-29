#ifndef _GSM_DATA_LAYER__
#define _GSM_DATA_LAYER__

#include "main.h"
#include "hw_polling_task.h"
#include "app_debug.h"

#define     POWER_SET        0
#define     POWER_SETTING    1
#define     GPRS_SET         2
#define     GPRS_SETTIING    3
#define     SETTING_DONE     4
/*        defination     */

void gm_state_machine_polling(void);

void GPRS_setup (at_cmd_response_evt_t evt, void *resp_buffer); // data setup handle 

void putFile (at_cmd_response_evt_t evt, void *resp_buffer);    // data putfile handle

void getFile (at_cmd_response_evt_t evt, void *resp_buffer);    // data get file handle
/**     prototype         **/


#endif
