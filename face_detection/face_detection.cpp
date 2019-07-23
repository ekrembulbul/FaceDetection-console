#include "pch.h"
#include "recognizer.h"

int main()
{
	recognizer r;
	r.takePicture();
	r.train();
	r.predict();

	return 0;
}