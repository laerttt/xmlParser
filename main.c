#include "xml_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// callback function to handle parsing events
void callback(const char *event, const char *path, const char *key, const char *value) {
    // static variable to store the current order ID
    static char current_id[ATTR_SIZE]; 
    // check if the event is in "attribute" state and the key is named "id"
    if (strcmp(event, "attribute") == 0 && strcmp(key, "id") == 0) {
        // store the current order id
        strcpy(current_id, value); 
    } 
    // check if the event is in "text" state and the path matches "/order/amount"
    else if (strcmp(event, "text") == 0 && strstr(path, "/order/amount") != NULL) {
        int amount = atoi(value); // convert the text (char type) to integer
        printf("Order ID: %s\tAmount: %d\n", current_id, amount); // Print the order ID and amount
    }
}

int main() {
    // mock data to be parsed
    const char *xml_data =
        "<root>"
        "    <order id=\"111\">"
        "        <amount>150</amount>"
        "    </order>"
        "    <order id=\"222\">"
        "        <amount>2</amount>"
        "    </order>"
        "    <order id=\"333\">"
        "        <amount>2000</amount>"
        "    </order>"
        "    <order id=\"123\">"
        "        <amount>15</amount>"
        "    </order>"
        "    <order id=\"321\">"
        "        <amount>4</amount>"
        "    </order>"
        "    <order id=\"231\">"
        "        <amount>200</amount>"
        "    </order>"
        "    <order id=\"213\">"
        "        <amount>200</amount>"
        "    </order>"
        "</root>";

    XMLParser parser;               // declare an XMLParser object
    init_parser(&parser);           // initialize the parser
    parse_xml(&parser, xml_data, callback); // oarse the XML data with the callback function

    return 0;
}