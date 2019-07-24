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
			"1. Take Picture\n" <<
			"2. Train\n" <<
			"3. Predict from Cam\n" <<
			"4. Predict from Image\n" <<
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
		else std::cout << "Invalid choice!" << std::endl;
	}

	return 0;
}