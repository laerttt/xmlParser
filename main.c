#include "xml_parser.h" // Include the custom XML parser header file
#include <stdio.h>      // Include standard I/O library for printing
#include <stdlib.h>     // Include standard library for atoi and memory functions
#include <string.h>     // Include string library for string manipulation functions

// Callback function to handle parsing events
void example_callback(const char *event, const char *path, const char *key, const char *value) {
    static char current_id[ATTR_SIZE]; // Static variable to store the current order ID

    // Check if the event is an "attribute" and the key is "id"
    if (strcmp(event, "attribute") == 0 && strcmp(key, "id") == 0) {
        strcpy(current_id, value); // Store the current order ID
    } 
    // Check if the event is "text" and the path matches "/order/amount"
    else if (strcmp(event, "text") == 0 && strstr(path, "/order/amount") != NULL) {
        int amount = atoi(value); // Convert the text value to an integer
        printf("Order ID: %s\tAmount: %d\n", current_id, amount); // Print the order ID and amount
    }
}

int main() {
    // XML data to be parsed
    const char *xml_data =
        "<root>"
        "    <order id=\"1111\">"
        "        <amount>150</amount>"
        "    </order>"
        "    <order id=\"222\">"
        "        <amount>2</amount>"
        "    </order>"
        "    <order id=\"333\">"
        "        <amount>2000</amount>"
        "    </order>"
        "    <order id=\"1112\">"
        "        <amount>15</amount>"
        "    </order>"
        "    <order id=\"223\">"
        "        <amount>4</amount>"
        "    </order>"
        "    <order id=\"334\">"
        "        <amount>200</amount>"
        "    </order>"
        "    <order id=\"334\">"
        "        <amount>200</amount>"
        "    </order>"
        "</root>";

    XMLParser parser;               // Declare an XMLParser object
    init_parser(&parser);           // Initialize the parser
    parse_xml(&parser, xml_data, example_callback); // Parse the XML data with the callback function

    return 0; // Exit the program
}