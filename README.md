# XML Parser Project

## **How to run**
In terminal navigate to this directory and enter commands:
```bash 
 $ make
 ```
 ```bash 
 $ ./xml_parser
 ```
 You can change the value of the mock input variable ```*xml_data``` in main.c to test different inputs.
## **Overview**

This project addresses the challenge of parsing large XML documents where performance is critical. The goal is to build a **forward-only XML parser** that processes XML efficiently, providing access to data as it traverses the document in a single pass.

### **Core Requirements**
1. **Forward-Only Parsing**: Traverse the XML document once without revisiting nodes.
2. **Event-Based Reporting**: Report the current path, element attributes, and values in real-time.
3. **Efficient Handling**:
   - Identify and decode XML escaped entities (e.g., `&quot;`, `&amp;`, etc.).
   - Ignore comments.
4. **Support Path and Value Tracking**:
   - Include the full XML path (e.g., `/root/order/amount`).
   - Report attributes and text values at the current path.
5. **Example Use Case**: Output the `order id` for any order with an `amount > 100`.

---

## **Design and Implementation**

### **1. State Machine**
The parser is implemented as a **state machine** that traverses the XML document. Key states include:
- **Start Tag**: Handles opening tags and parses attributes.
- **End Tag**: Handles closing tags.
- **Text Content**: Extracts and decodes text between tags.
- **Comments**: Ignores XML comments (`<!-- ... -->`).

### **2. Path and Attribute Tracking**
- **Path Maintenance**: Tracks the current XML path and updates it dynamically as elements are encountered.
- **Attributes**: Parses and stores attributes for the current element, making them available during parsing.

### **3. XML Entity Decoding**
Handles standard XML entities:
- `&quot;` -> `"`
- `&amp;` -> `&`
- `&apos;` -> `'`
- `&lt;` -> `<`
- `&gt;` -> `>`

### **4. Event Reporting**
The parser triggers callback functions to report events such as:
- `start_element`: When an element starts.
- `end_element`: When an element ends.
- `text`: When text content is encountered.
- `attribute`: When attributes are parsed.

---

## **Example Code**

### **XML Document**
```xml
<root>
    <order id="1111">
        <amount>150</amount>
    </order>
    <order id="222">
        <amount>2</amount>
    </order>
    <order id="333">
        <amount>2000</amount>
    </order>
</root>
```
## Example Usage

### Parser Callback

```c
void example_callback(const char *event, const char *path, const char *key, const char *value) {
    static char current_id[128];
    if (strcmp(event, "attribute") == 0 && strcmp(key, "id") == 0) {
        strcpy(current_id, value); // Store the order id
    } else if (strcmp(event, "text") == 0 && strstr(path, "/order/amount") != NULL) {
        int amount = atoi(value);
        if (amount > 100) {
            printf("Order ID: %s\tAmount: %d\n", current_id, amount);
        }
    }
}
```
### Main Function

```c
int main() {
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
        "</root>";

    XMLParser parser;
    init_parser(&parser); // Initialize the parser
    parse_xml(&parser, xml_data, example_callback); // Parse the XML with the callback

    return 0;
}
```
## Expected Output

For the example XML provided, the program should output:
```bash
Order ID: 111   Amount: 150
Order ID: 222   Amount: 2
Order ID: 333   Amount: 2000
Order ID: 123   Amount: 15
Order ID: 321   Amount: 4
Order ID: 231   Amount: 200
Order ID: 213   Amount: 200
```
**_Summary_**

This project provides a forward-only XML parser that efficiently processes XML documents in a single pass. The parser is lightweight, fast, and ideal for scenarios where only immediate access to current data is required.

By implementing this parser, you can handle large-scale XML parsing tasks with minimal overhead while ensuring real-time reporting of attributes, paths, and text content.