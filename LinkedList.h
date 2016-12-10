#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<ctype.h>
// Author: Carl Leijonberg

typedef struct colorList {
	double l1;
	double a1;
	double b1;
	struct colorList *next;
	char *key;
} ColorList;

ColorList *first_node = NULL;
ColorList *last_node = NULL;

ColorList *create_new_colorList(char key[], double l1,double a1, double b1) {
	static ColorList new_colorList[100];
	static int index = 0;
	ColorList *n = &(new_colorList[index++]);
	n->key = key;
	n->l1 = l1;
	n->a1 = a1;
	n->b1 = b1;
	n->next = NULL;
	return n;
}

void insert_colorList(char *key, double l1, double a1, double b1) {
	ColorList *new_colorList = create_new_colorList(key, l1,a1,b1);

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

