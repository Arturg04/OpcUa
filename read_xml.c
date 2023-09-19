#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

// Structure to hold tag information
struct TagInfo {
    char name[50];
    char node[50];
    char tag_type[50];
};

// Function to parse the XML and store tag information in an array
int parseXmlFile(const char *filename, struct TagInfo tags[], int maxTags) {
    xmlDocPtr doc;
    xmlNodePtr root, tagsNode;

    // Parse the XML file
    doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return -1;
    }

    root = xmlDocGetRootElement(doc);
    int tagCount = 0;

    // Iterate through the Tags elements and store their content
    for (tagsNode = root->children; tagsNode && tagCount < maxTags; tagsNode = tagsNode->next) {
        if (tagsNode->type == XML_ELEMENT_NODE && xmlStrcmp(tagsNode->name, (const xmlChar*)"Tags") == 0) {
            xmlNodePtr child;
            for (child = tagsNode->children; child; child = child->next) {
                if (child->type == XML_ELEMENT_NODE) {
                    if (xmlStrcmp(child->name, (const xmlChar*)"Name") == 0) {
                        snprintf(tags[tagCount].name, sizeof(tags[tagCount].name), "%s", xmlNodeGetContent(child));
                    } else if (xmlStrcmp(child->name, (const xmlChar*)"Node") == 0) {
                        snprintf(tags[tagCount].node, sizeof(tags[tagCount].node), "%s", xmlNodeGetContent(child));
                    } else if (xmlStrcmp(child->name, (const xmlChar*)"Type") == 0) {
                        snprintf(tags[tagCount].tag_type, sizeof(tags[tagCount].tag_type), "%s", xmlNodeGetContent(child));
                    }
                }
            }
            tagCount++;
        }
    }

    // Clean up and free memory
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return tagCount;
}

// Function to print tag information
void printTags(struct TagInfo tags[], int tagCount) {
    for (int i = 0; i < tagCount; i++) {
        printf("Name: %s\n", tags[i].name);
        printf("Node: %s\n", tags[i].node);
        printf("Type: %s\n", tags[i].tag_type);
        printf("\n");
    }
}

int main() {
    const char* filename = "tags.xml";
    struct TagInfo tags[100]; // Assuming a maximum of 100 tags, adjust as needed

    int tagCount = parseXmlFile(filename, tags, sizeof(tags) / sizeof(tags[0]));
    if (tagCount > 0) {
        printTags(tags, tagCount);
    } else {
        fprintf(stderr, "No tags found or error occurred during parsing.\n");
    }

    return 0;
}
