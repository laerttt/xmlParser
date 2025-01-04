#include "xml_parser.h" // Include custom XML parser header
#include <stdio.h>      // Standard I/O library for functions like printf
#include <stdlib.h>     // Standard library for functions like malloc, free, and atoi
#include <string.h>     // String manipulation functions like strcpy, strcat, and strchr
#include <ctype.h>      // Character handling functions like isspace

// Decode XML entities (e.g., &amp; -> &, &lt; -> <, etc.)
void decode_entities(char *text) {
    char *p = text; // Pointer to traverse the input text
    char decoded[1024] = ""; // Temporary buffer to store the decoded text
    while (*p) {
        // Check for known XML entities and replace them with their decoded equivalents
        if (strncmp(p, "&quot;", 6) == 0) {
            strcat(decoded, "\"");
            p += 6;
        } else if (strncmp(p, "&amp;", 5) == 0) {
            strcat(decoded, "&");
            p += 5;
        } else if (strncmp(p, "&apos;", 6) == 0) {
            strcat(decoded, "'");
            p += 6;
        } else if (strncmp(p, "&lt;", 4) == 0) {
            strcat(decoded, "<");
            p += 4;
        } else if (strncmp(p, "&gt;", 4) == 0) {
            strcat(decoded, ">");
            p += 4;
        } else {
            strncat(decoded, p, 1); // Copy regular characters
            p++;
        }
    }
    strcpy(text, decoded); // Copy the decoded text back to the original buffer
}

// Initialize the parser structure
void init_parser(XMLParser *parser) {
    parser->path[0] = '\0'; // Clear the path
    parser->current_element[0] = '\0'; // Clear the current element
    parser->attr_count = 0; // Reset attribute count
}

// Update the XML path
void update_path(XMLParser *parser, const char *element, int add) {
    if (add) {
        // Add a new element to the path
        if (strlen(parser->path) > 0) {
            strcat(parser->path, "/"); // Add a slash if the path is not empty
        }
        strcat(parser->path, element); // Append the element to the path
    } else {
        // Remove the last element from the path
        char *last_slash = strrchr(parser->path, '/'); // Find the last slash
        if (last_slash) {
            *last_slash = '\0'; // Truncate the path at the last slash
        } else {
            parser->path[0] = '\0'; // Clear the path if no slash is found
        }
    }
}

// Parse attributes from a tag
void parse_attributes(XMLParser *parser, char *attr_string) {
    parser->attr_count = 0; // Reset attribute count
    char *token = strtok(attr_string, " "); // Tokenize the attribute string
    while (token) {
        char *equal = strchr(token, '='); // Find the '=' character
        if (equal) {
            *equal = '\0'; // Split the token at '='
            strcpy(parser->attributes[parser->attr_count].name, token); // Copy the attribute name
            strcpy(parser->attributes[parser->attr_count].value, equal + 2); // Skip ="
            parser->attributes[parser->attr_count].value[strlen(parser->attributes[parser->attr_count].value) - 1] = '\0'; // Remove trailing "
            decode_entities(parser->attributes[parser->attr_count].value); // Decode entities in the attribute value
            parser->attr_count++; // Increment the attribute count
        }
        token = strtok(NULL, " "); // Get the next token
    }
}

// Main XML parsing function
void parse_xml(XMLParser *parser, const char *xml, Callback callback) {
    const char *p = xml; // Pointer to traverse the XML string
    char buffer[1024]; // Buffer to store temporary data
    while (*p) {
        if (*p == '<') {
            if (*(p + 1) == '/') {
                // Handle end tags (e.g., </tag>)
                p += 2; // Skip "</"
                const char *end = strchr(p, '>'); // Find the closing '>'
                strncpy(buffer, p, end - p); // Copy the tag name
                buffer[end - p] = '\0';
                update_path(parser, buffer, 0); // Remove the element from the path
                callback("end_element", parser->path, NULL, NULL); // Trigger the end_element event
                p = end + 1; // Move past the closing '>'
            } else {
                // Handle start tags (e.g., <tag>)
                p++; // Skip "<"
                const char *end = strchr(p, '>'); // Find the closing '>'
                strncpy(buffer, p, end - p); // Copy the tag name and attributes
                buffer[end - p] = '\0';
                char *space = strchr(buffer, ' '); // Check for attributes
                if (space) {
                    *space = '\0'; // Split tag name and attributes
                    strcpy(parser->current_element, buffer); // Copy the tag name
                    parse_attributes(parser, space + 1); // Parse the attributes
                } else {
                    strcpy(parser->current_element, buffer); // Copy the tag name (no attributes)
                }
                update_path(parser, parser->current_element, 1); // Add the element to the path
                for (int i = 0; i < parser->attr_count; i++) {
                    // Trigger attribute events for each attribute
                    callback("attribute", parser->path, parser->attributes[i].name, parser->attributes[i].value);
                }
                callback("start_element", parser->path, NULL, NULL); // Trigger the start_element event
                p = end + 1; // Move past the closing '>'
            }
        } else if (*p != '\0') {
            // Handle text content
            const char *end = strchr(p, '<'); // Find the next '<'
            if (!end) end = p + strlen(p); // If no '<', go to the end of the string
            strncpy(buffer, p, end - p); // Copy the text content
            buffer[end - p] = '\0';
            decode_entities(buffer); // Decode any XML entities
            if (strlen(buffer) > 0) {
                // Trigger the text event
                callback("text", parser->path, "value", buffer);
            }
            p = end; // Move to the next '<'
        }
    }
}