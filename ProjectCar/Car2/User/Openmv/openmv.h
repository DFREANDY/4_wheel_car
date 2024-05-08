#ifndef USER_OPENMV_H_
#define USER_OPENMV_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    int16_t angle1;
    int16_t angle2;
    int16_t turn;
    int16_t fork;
}openmv_message_t;

void Openmv_Init(void);
void Openmv_Receive_Data(uint8_t data);

extern openmv_message_t OpenmvMessage;

#endif /* USER_OPENMV_OPENMV_H_ */
