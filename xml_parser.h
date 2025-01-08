// custom header for constant vars and functions
#ifndef XML_PARSER_H
#define XML_PARSER_H

// constants for maximum attribute size and path size
#define ATTR_SIZE 128 
#define PATH_SIZE 256

// attribute & parser structure
typedef struct {
    char name[ATTR_SIZE];
    char value[ATTR_SIZE];
} Attribute;
typedef struct {
    // path - xml path (e.g., "/root/order")
    // current_element - curr element being processed
    // attr_count - attributes in the current element
    char path[PATH_SIZE];           
    char current_element[ATTR_SIZE];
    Attribute attributes[ATTR_SIZE];
    int attr_count;                      
} XMLParser;

// called for events like "start_element", "end_element", "attribute", and "text"
typedef void (*Callback)(const char *event, const char *path, const char *key, const char *value);

// initializer for the XML parser structure
void init_parser(XMLParser *parser);

// Function to parse an XML string
// - `parser`: Pointer to the XMLParser structure
// - `xml`: The XML string to parse
// - `callback`: Function to handle parsing events
void parse_xml(XMLParser *parser, const char *xml, Callback callback);

#endif