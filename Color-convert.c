#include <stdio.h>
#include "LinkedList.h"
// Author: Carl Leijonberg

void insert(char*, char*);
double square(double);

char* findColor(int r, int g, int b) {
  int deltaR, deltaG, deltaB;
  double dist;
  double minDist = 10000000;
  ColorList minEntry;

  ColorList *current = first_node;
  while (current != NULL) {
    deltaR = r - current->r;
    deltaG = g - current->g;
    deltaB = b - current->b;
    dist = square(deltaR * 0.3)
         + square(deltaG * 0.59)
         + square(deltaB * 0.11);

    if (dist < minDist) {
      minDist = dist;
      minEntry = *current; 
    }
    
    current = current->next;
  }

  return minEntry.name;
}

double square(double num) {
  return num * num;
}

void hexToRgb(char* hexString, int *r, int *g, int *b) {
  int hexValue = strtol(hexString, NULL, 16);
  *b = hexValue & 0xFF;
  hexValue >>= 8;
  *g = hexValue & 0xFF;
  hexValue >>= 8;
  *r = hexValue & 0xFF;
}

void insert(char* colorName, char* hexString) {
  int r, g, b;
  hexToRgb(hexString, &r, &g, &b);
  insert_colorList(colorName, r, g, b);
}

void fill_colorList() {
  insert("cloudy blue", "acc2d9");
  insert("dark pastel green", "56ae57");
  insert("purple", "e1e9c");
  insert("green", "15b01a");
  insert("blue", "0343df");
  insert("pink", "ff81c0");
  insert("brown", "653700");
  insert("red", "e50000");
  insert("light blue", "95d0fc");
  insert("teal", "029386");
  insert("orange", "f97306");
  insert("light green", "96f97b");
  insert("yellow", "ffff14");
  insert("gray", "929591");
  insert("dark green", "033500");
  insert("dark blue", "00035b");
  insert("tan", "d1b26f");
  insert("black", "000000");
  insert("light brown", "ad8150");
  insert("gold", "dbb40c");
  insert("light red", "ff474c");
  insert("white", "ffffff");
  insert("light blue green", "7efbb3");
  insert("cherry red", "f7022a");
  insert("orange yellow", "ffad01");
  insert("dark khaki", "9b8f55");
  insert("cherry", "cf0234");
  insert("golden", "f5bf03");
  insert("chestnut", "742802");
  insert("light gold", "fddc5c");
  insert("electric lime", "a8ff04");
  insert("dust", "b2996e");
  insert("dark red", "840000");
  insert("light pink", "ffd1df");
  insert("peach", "ffb07c");
  insert("beige", "e6daa6");
  insert("salmon", "ff796c");
  insert("dark purple", "35063e");
  insert("cyan", "00ffff");
  insert("turquoise", "06c2ac");
  insert("light purple", "bf77f6");
  insert("magenta", "c20078");
  insert("yellow green", "c0fb2d");
  insert("dark teal", "014d4e");
  insert("light orange", "fdaa48");
  insert("reddish brown", "7f2b0a");
  insert("emerald", "01a049");
  insert("steel", "738595");
  insert("bronze", "a87900");
  insert("amber", "feb308");
  insert("silver", "c5c9c7");
  insert("copper", "b66325");
  insert("sapphire", "2138ab");
}

