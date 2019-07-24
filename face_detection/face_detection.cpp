#include "pch.h"
#include "recognizer.h"

int main()
{
	recognizer r;
	char choice;

	while (true)
	{
		std::cout <<
			"Enter the action you want to do:\n" <<
			"1. Take picture\n" <<
			"2. Train\n" <<
			"3. Predict from cam\n" <<
			"4. Predict from image\n" <<
			"5. Multi train\n" <<
			"Q. Quit" <<
			std::endl;
		std::cin >> choice;
		if (choice == 'q' || choice == 'Q')
		{
			std::cout << "[INFO] Program done." << std::endl;
			return 0;
		}
		else if (choice == '1') r.takePicture();
		else if (choice == '2') r.train();
		else if (choice == '3') r.predictFromCam();
		else if (choice == '4') r.predictFromImage();
		else if (choice == '5') r.multiTrain();
		else std::cout << "Invalid choice!" << std::endl;
	}

	return 0;
}