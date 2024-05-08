#ifndef USER_OPENMV_H_
#define USER_OPENMV_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    int8_t angle1;
    int8_t angle2;
    int8_t turn;
    int8_t fork;
    int8_t number;
}openmv_message_t;

void Openmv_Init(void);
void Openmv_Receive_Data(uint8_t data);
void openmv_request_data(uint8_t content);
extern openmv_message_t OpenmvMessage;

#endif /* USER_OPENMV_OPENMV_H_ */
