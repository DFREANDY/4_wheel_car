// #include "task_decision.h"

// #include "infoprint.h"
// #include "key.h"

// #include "headfile.h"

// #include "mission.h"
// //-------------------------------------------------------------------
// //  —°‘Ò»ŒŒÒ
// //-------------------------------------------------------------------

// uint8_t pointer = 0;


// void choose_task()
// {
//     while (1)
//     {
//         info_print_mainmenu();
//         switch (KEY_Input())
//         {
//         case KEY1_PRESSED:
//             pointer = (pointer + 1) % 4;
//             info_print_mainmenu();
//             break;
//         case KEY2_PRESSED:
//             LED2_ALL_OFF();
//             StateMachine.state = 1;
//             switch (pointer)
//             {
//             case 0:
//                 Mission_1();
//                 break;
//             case 1:
//                 Mission_2();
//                 break;
//             case 2:
//                 Mission_3();
//                 break;
//             case 3:
//                 Mission_4();
//                 break;
//             case 4:
//                 Mission_5();
//             case 5:
//                 Mission_6();
//             default:
//                 break;
//             }
//             break;
//         default:
//             break;
//         }
//     }
// }

