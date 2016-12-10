#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<ctype.h>
// Author: Carl Leijonberg

typedef struct colorList {
	int r;
	int g;
	int b;
	struct colorList *next;
	char *name;
} ColorList;

ColorList *first_node = NULL;
ColorList *last_node = NULL;

ColorList *create_new_colorList(char name[], int r, int g, int b) {
	static ColorList new_colorList[100];
	static int index = 0;
	ColorList *n = &(new_colorList[index++]);
	n->name = name;
	n->r = r;
	n->g = g;
	n->b = b;
	n->next = NULL;
	return n;
}

void insert_colorList(char *key, int r, int g, int b) {
	ColorList *new_colorList = create_new_colorList(key, r,g,b);

	if (first_node == NULL) {
		// No nodes
		first_node = new_colorList;
		last_node = new_colorList;
	}
	else if (first_node == last_node) {
		// 1 node
		first_node->next = new_colorList;
		last_node = new_colorList;
	}
	else {
		// At least 2 nodes
		last_node->next = new_colorList;
		last_node = new_colorList;
	}
}

