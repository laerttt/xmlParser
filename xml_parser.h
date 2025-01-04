#ifndef XML_PARSER_H
#define XML_PARSER_H

// Define constants for maximum attribute size and path size
#define ATTR_SIZE 128 // Maximum size for attribute name and value
#define PATH_SIZE 256 // Maximum size for the XML path

// Structure to represent a single XML attribute (name-value pair)
typedef struct {
    char name[ATTR_SIZE];  // Name of the attribute
    char value[ATTR_SIZE]; // Value of the attribute
} Attribute;

// Structure to represent the state of the XML parser
typedef struct {
    char path[PATH_SIZE];                // Current XML path (e.g., "/root/order")
    char current_element[ATTR_SIZE];     // Name of the current element being processed
    Attribute attributes[ATTR_SIZE];     // Array to hold attributes of the current element
    int attr_count;                      // Number of attributes in the current element
} XMLParser;

// Type definition for a callback function
// The callback is called for events like "start_element", "end_element", "attribute", and "text"
typedef void (*Callback)(const char *event, const char *path, const char *key, const char *value);

// Function to initialize the XML parser structure
void init_parser(XMLParser *parser);

// Function to parse an XML string
// - `parser`: Pointer to the XMLParser structure
// - `xml`: The XML string to parse
// - `callback`: Function to handle parsing events
void parse_xml(XMLParser *parser, const char *xml, Callback callback);

#endif // XML_PARSER_H