#include "Drawable.h"

Drawable::Drawable(Anchor anchor) {
	Visibility = true;
	dAnchor = anchor;
}

void Drawable::Draw(){
	if (Visibility) {
		pDraw();
	}
}

InputDrawable::InputDrawable(Anchor anchor) : Drawable(anchor) {
}
