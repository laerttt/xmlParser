// include custom header and standerd libraries to handle i/o, memory management, string manipulation etc
#include "xml_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Decode XML entities (e.g., &amp; -> &, &lt; -> <, etc.)
void decode_entities(char *text) {
    //pointer to traverse the input and a temp buffer
    char *p = text;
    char decoded[1024] = "";
    while (*p) {
        // cheking for xml entities otherwise just copy regula characters
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
            strncat(decoded, p, 1);
            p++;
        }
    }
    strcpy(text, decoded); // copy text to buffer
}

void init_parser(XMLParser *parser) {
    parser->path[0] = '\0';
    parser->current_element[0] = '\0';
    parser->attr_count = 0;
}

void update_path(XMLParser *parser, const char *element, int add) {
    // add element to path if add == 1
    if (add) {
        // if path not empty add / 
        if (strlen(parser->path) > 0) {
            strcat(parser->path, "/");
        }
        // add the element
        strcat(parser->path, element); 
    } else {
        // add == 0
        // find last slash
        char *last_slash = strrchr(parser->path, '/'); 
        if (last_slash) {
            // replace the last slash with null terminator hence removing the string after the slash
            *last_slash = '\0'; 
        } else {
            // no slash terminate whole string ( 1 element left in path)
            parser->path[0] = '\0';
        }
    }
}

void parse_attributes(XMLParser *parser, char *attr_string) {
    parser->attr_count = 0;
    char *token = strtok(attr_string, " "); // tokenize the string
    while (token) {
        char *equal = strchr(token, '='); // find the equals character
        if (equal) {
            *equal = '\0'; // split the token at the equals sign
            strcpy(parser->attributes[parser->attr_count].name, token); // copy attribute name
            strcpy(parser->attributes[parser->attr_count].value, equal + 2); // skip equals sign and copy the value
            parser->attributes[parser->attr_count].value[strlen(parser->attributes[parser->attr_count].value) - 1] = '\0'; // remove trailing parantheses (double quotations)
            decode_entities(parser->attributes[parser->attr_count].value); // decode entities
            parser->attr_count++; // increment attribute count
        }
        token = strtok(NULL, " "); // next token
    }
}

// main parsing function
void parse_xml(XMLParser *parser, const char *xml, Callback callback) {
    const char *p = xml; // pointer to traverse the xml string
    char buffer[1024]; // temp buffer
    while (*p) {
        if (*p == '<') {
            if (*(p + 1) == '/') {
            // handle end tags
                // skip "</"
                p += 2; 
                // find the next > 
                const char *end = strchr(p, '>'); 
                // copy tag name
                strncpy(buffer, p, end - p); 
                buffer[end - p] = '\0';
                // remove element from path
                update_path(parser, buffer, 0); 
                // trigger end_element event
                callback("end_element", parser->path, NULL, NULL); 
                // skip >
                p = end + 1; 
            } else {
                // handle start tags (e.g., <tag>)
                p++; // skip <
                // same logic as above
                const char *end = strchr(p, '>');
                strncpy(buffer, p, end - p); 
                buffer[end - p] = '\0';
                // check for attributes
                char *space = strchr(buffer, ' '); 
                if (space) {
                    // split tag name and attributes
                    *space = '\0'; 
                    strcpy(parser->current_element, buffer);
                    parse_attributes(parser, space + 1);
                } else {
                    strcpy(parser->current_element, buffer); // copy the tag name (no attributes)
                }
                // add element to path
                update_path(parser, parser->current_element, 1);
                for (int i = 0; i < parser->attr_count; i++) {
                    // trigger attribute events for each attribute
                    callback("attribute", parser->path, parser->attributes[i].name, parser->attributes[i].value);
                }
                // trigger the start_element event
                callback("start_element", parser->path, NULL, NULL); 
                // move past the closing >
                p = end + 1; 
            }
        } else if (*p != '\0') {
            // handle text content
            // find the next <
            const char *end = strchr(p, '<'); 
            // uf no < go to the end of the string
            if (!end) end = p + strlen(p); 
            // copy text
            strncpy(buffer, p, end - p); 
            buffer[end - p] = '\0';
            // decode any xml entities
            decode_entities(buffer); 
            if (strlen(buffer) > 0) {
                // trigger the text event
                callback("text", parser->path, "value", buffer);
            }
            p = end; // next <
        }
    }
}