#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

typedef struct {
	char *name;
	char *node;
	char *type;
	char *value;
} xmlTag;


// Function to parse an XML file and populate an array of xmlTag structures
xmlTag* parseXML(const char* xmlFilePath, int* numTags) {
	xmlDocPtr doc;
	xmlNodePtr root, node;
	xmlTag* tags = NULL;

	// Load and parse the XML file
	doc = xmlReadFile(xmlFilePath, NULL, 0);
	if (doc == NULL) {
		fprintf(stderr, "Error: Unable to parse the XML file.\n");
		return NULL;
	}

	// Get the root element
	root = xmlDocGetRootElement(doc);

	// Count the number of child nodes
	*numTags = 0;
	for (node = root->children; node; node = node->next) {
		(*numTags)++;
	}

	// Allocate memory for the array of xmlTag structures
	tags = (xmlTag*)malloc(*numTags * sizeof(xmlTag));
	if (tags == NULL) {
		fprintf(stderr, "Error: Memory allocation failed.\n");
		xmlFreeDoc(doc);
		return NULL;
	}

	// Populate the array with data from XML
	int i = 0;
		for (node = root->children; node; node = node->next) {
			if (node->type == XML_ELEMENT_NODE) {
			xmlTag* currentTag = &tags[i];
			currentTag->name = strdup((char*)node->name);
			currentTag->node = strdup((char*)node->name);
			xmlNodePtr child = node->children;

			// Assuming "type" is an attribute of the element
			xmlChar* typeAttr = xmlGetProp(node, (const xmlChar*)"type");
			currentTag->type = strdup((char*)typeAttr);
			xmlFree(typeAttr);

			// Assuming the value is stored within a child element
			if (child && child->type == XML_TEXT_NODE) {
				currentTag->value = strdup((char*)child->content);
			}

			i++;
		}
	}

	// Cleanup and return the array
	xmlFreeDoc(doc);
	return tags;
}


