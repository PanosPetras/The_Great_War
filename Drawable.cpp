#include "Drawable.h"

Drawable::Drawable() {
	Visibility = true;
}

void Drawable::Draw(){
	if (Visibility) {
		pDraw();
	}
}
