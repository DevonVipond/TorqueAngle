#pragma once
namespace threading {
    /* Syntax:
        xTaskCreatePinnedToCore(
                     coreTask,   // Function to implement the task
                     "coreTask", // Name of the task 
                     10000,      // Stack size in words 
                     NULL,       // Task input parameter 
                     0,          // Priority of the task 
                     NULL,       // Task handle. 
                     taskCore);  // Core where the task should run 
    */
   /*
    void create_thread(void (* task)( const unsigned char*, const unsigned char*, int ), unsigned int core ) {
        unsigned int TASK_PRIORITY = 2;
        xTaskCreatePinnedToCore(task, "FibonacciTask", 5000, NULL, TASK_PRIORITY, &Task1, core);
    }
    */

}