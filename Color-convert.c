#include<stdio.h>
#include "LinkedList.h"
// Author: Carl Leijonberg

double pow(double, double);
void insert(char*, char*);

struct colorValues {
	double x1;
	double y1;
	double z1;
};
// Converts RGB values to XYZ values.
struct colorValues convertToXYZ(int r, int g, int b) {
	struct colorValues XYZ;
	double Red = r / (double) 255;
	double Green = g / (double) 255;
	double Blue = b / (double) 255;
	if (Red > 0.04045) {
		double t = (Red + 0.055) / 1.055;
		Red = pow(t,2.4);
	}
	else {
		Red = Red / 12.92;
	}
	if (Green > 0.04045) {
		double t = (Green + 0.055) / 1.055;
		Green = pow(t, 2.4);
	}
	else {
		Green = Green / 12.92;
	}
	if (Blue > 0.04045) {
		double t = (Blue + 0.055) / 1.055;
		Blue = pow(t, 2.4);
	}
	else {
		Blue = Blue / 12.92;
	}
	Red = Red * 100;
	Green = Green * 100;
	Blue = Blue * 100;

	double X = Red * 0.4124 + Green * 0.3576 + Blue * 0.1805;
	double Y = Red * 0.2126 + Green* 0.7152 + Blue * 0.0722;
	double Z = Red * 0.0193 + Green * 0.1192 + Blue * 0.9505;
	XYZ.x1 = X;
	XYZ.y1 = Y;
	XYZ.z1 = Z;
	return XYZ;
}
// Converts XYZ values to CIE-L*ab values.
struct colorValues convertToCIE(double x, double y, double z) {
	struct colorValues LAB;
	double var_X = x / 95.047;          
	double var_Y = y / 100;          
	double var_Z = z / 108.883;  
	if (var_X > 0.008856) { 
		var_X = pow(var_X, (double) 1/3);
	}
	else {
		var_X = (7.787 * var_X) + ((double) 16 / 116);
	}
	if (var_Y > 0.008856) {
		var_Y = pow(var_Y, (double) 1/3);
	}
	else {
		var_Y = (7.787 * var_Y) + ((double) 16 / 116);
	}
	if (var_Z > 0.008856) {
		var_Z = pow(var_Z, (double) 1/3);
	}
	else {
		var_Z = (7.787 * var_Z) + ((double) 16 / 116);
	}

	double l = (116 * var_Y) - 16;
	double a = 500 * (var_X - var_Y);
	double b = 200 * (var_Y - var_Z);
	LAB.x1 = l;
	LAB.y1 = a;
	LAB.z1 = b;
	return LAB;
}

// Converts RGB values to CIE-L*ab values.
struct colorValues convertColor(int r, int g, int b) {
	struct colorValues LABfinal;
	struct colorValues XYZ1;
	XYZ1 = convertToXYZ(r, g, b);
	LABfinal = convertToCIE(XYZ1.x1, XYZ1.y1, XYZ1.z1);
	return LABfinal;
}

const char *findColor(int r, int g, int b) {
	//build_colorList_dict();
	struct colorValues LAB= convertColor( r, g, b);
	double L = LAB.x1;
	double A = LAB.y1;
	double B = LAB.z1;
	double deltaE;
	double deltaL;
	double deltaA;
	double deltaB;
	double smallestDeltaE = 1000000000;
	const char *name = NULL;
	ColorList *current = first_node;
	while (current != NULL) {
		deltaL = L - (current->l1);
		deltaA = A - (current->a1);
		deltaB = B - (current->b1);
		deltaE = pow(deltaL, 2) + pow(deltaA, 2) + pow(deltaB, 2);
		if (deltaE < smallestDeltaE) {
			smallestDeltaE = deltaE;
			name = current->key;
		}
		current = current->next;
	}
  
  return name;
}

double pow(double a, double b) {
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
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
  struct colorValues lab = convertColor(r, g, b);
  insert_colorList(colorName, lab.x1, lab.y1, lab.z1);
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

