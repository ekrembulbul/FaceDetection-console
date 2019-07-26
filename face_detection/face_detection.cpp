#include "pch.h"
#include "recognizer.h"


int main()
{
	recognizer r;
	r.loadXml("haarcascade-frontalface-default.xml");
	char choice = 0;

	while (choice != 'q' && choice != 'Q')
	{
		std::cout <<
			"Enter the action you want to do:\n" <<
			"1. Take picture\n" <<
			"2. Train\n" <<
			"3. Predict from cam\n" <<
			"4. Predict from image\n" <<
			"5. Multi train\n" <<
			"6. Multi predict from cam\n" <<
			"Q. Quit" <<
			std::endl;
		std::cin >> choice;
		if (choice == '1') r.takePicture();
		else if (choice == '2') r.train();
		else if (choice == '3') r.predictFromCam();
		else if (choice == '4') r.predictFromImage();
		else if (choice == '5') r.multiTrain();
		else if (choice == '6') r.multiPredictFromCam();
		else std::cout << "Invalid choice!" << std::endl;
	}
	std::cout << "[INFO] Program done." << std::endl;
	return 0;
}
