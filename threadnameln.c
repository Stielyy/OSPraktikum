#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>

// Define the data structure to pass to the thread
struct student_data {
    char name[100];           // Fixed maximum length for name
    unsigned int actual_len;  // Will store the actual string length
};

/* Thread function */
void *example_fct(void *args) {
    // Cast the argument back to our struct type
    struct student_data *data = (struct student_data *)args;
    
    // Calculate the actual length of the name
    data->actual_len = strlen(data->name);
    
    // Print the received data
    printf("Thread received: Name='%s', Length will be calculated\n", data->name);
    
    return NULL;  // Return value not used as per requirements
}

int main() {
    pthread_t thread;
    
    // Create and initialize the student data structure
    struct student_data student;
    strncpy(student.name, "Julian", sizeof(student.name));  // Set the name strncpy for copying the name safely in the array with decleared size.
    student.name[sizeof(student.name)-1] = '\0';  // Ensure null-termination
    
    // Start the thread with our data structure
    pthread_create(&thread, NULL, &example_fct, &student); 
    
    // Wait for thread termination
    pthread_join(thread, NULL);
    
    // Print the results after thread completion
    printf("Main received: Name='%s', Calculated Length=%u\n", 
           student.name, student.actual_len);
    
    return 0;
}
