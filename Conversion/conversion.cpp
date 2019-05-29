#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <cmath>
#include <thread>
#include <chrono>
#include <cstdlib>
#define S_NEW 40

#include "../communication/ConversionClient.h"

using namespace std;

const string c_Usage = "conversion <heightmap text>";
int counter = 0;

void bil(vector<int> hMap, int w, int h,
	int w2, int h2, vector<int>& hMapScaled) {
	// vector<int> temp;
	int index;
	int A, B, C, D, x, y, gray;
	double x_ratio = ((double)(w - 1)) / w2;
	double y_ratio = ((double)(h - 1)) / h2;
	double x_diff, y_diff, ya, yb;
	int offset = 0;
	for (int i = 0; i < h2; i++) {
		for (int j = 0; j < w2; j++) {
			x = (x_ratio * j);
			y = (y_ratio * i);
			x_diff = (x_ratio * j) - x;
			y_diff = (y_ratio * i) - y;
			index = y * w + x;

			// range is 0 to 255 thus bitwise AND with 0xff
			A = hMap[index] & 0xff;
			B = hMap[index + 1] & 0xff;
			C = hMap[index + w] & 0xff;
			D = hMap[index + w + 1] & 0xff;

			// Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
			gray = (A * (1 - x_diff) * (1 - y_diff) + B * (x_diff) * (1 - y_diff) +
				C * (y_diff) * (1 - x_diff) + D * (x_diff * y_diff)
				);
			hMapScaled.push_back(gray);
		}
	}
}

string convertHeightmap(vector<tuple<int, int, double> > hMap) {
	double planeDistance = 16;
	int hMapLength = 20 * 20;

#if 0
	for (int i = 1; i < 380; i++) {
		if (fabs(get<2>(hMap[i]) - get<2>(hMap[i - 1])) < 0.15) {
			get<2>(hMap[i]) = get<2>(hMap[i - 1]);
		}
	}
#endif


	int vertexCount = hMapLength;

	// Writing to file to read again
	// because I don't know how to append
	// integer or double to string
	// and << operator looks cleaner.
	string outputFileName("out.obj");
	ofstream outputFile(outputFileName);

	vector<int> temp;

	for (int i = 0; i < hMapLength; i++) {
		double height = get<2>(hMap[i]);
		height /= 1.0;
		height *= 255;
		temp.push_back((int)height);
	}

	vector<int> hMapScaled;
	bil(temp, 20, 20, S_NEW, S_NEW, hMapScaled);

	for (int i = 0; i < S_NEW; i++) {
		for (int j = 0; j < S_NEW; j++) {
			outputFile << "v " << (j * planeDistance) << " " << (i * planeDistance) << " " << (((double)hMapScaled[i * S_NEW + j]) / 2) << "\n";
			if (counter == 399)
				cout << "v " << (j * planeDistance) << " " << (i * planeDistance) << " " << (((double)hMapScaled[i * S_NEW + j]) / 2) << "\n";
		}
	}

	if (counter == 399) {
		for (int i = 0; i < S_NEW; ++i) {
			for (int j = 0; j < S_NEW; ++j) {
				cout << hMapScaled[i * S_NEW + j] << " ";
			}
			cout << endl;
		}
	}

	outputFile << "g World\n";

	int sideLength = S_NEW;

	int faceCount = (sideLength * 2 - 2) * (sideLength - 1);

	for (int i = 0; i < faceCount / 2; i++) {
		int v1;
		int v2;
		int v3;

		v1 = i + 1;
		v2 = i + 2;
		v3 = (i + 1) + sideLength;

		outputFile << "f " << v1 << " " << v2 << " " << v3 << "\n";

		v1 = i + 2;
		v2 = (i + 2) + sideLength;
		v3 = (i + 1) + sideLength;

		outputFile << "f " << v1 << " " << v2 << " " << v3 << "\n";
	}

	outputFile.close();

	// Reading from the intermediate obj file
	// to compose its lines as a string
	// and send it to viewer module
	string fullContent("");
	string line;
	ifstream inFile("out.obj");
	if (inFile.is_open()) {
		while (getline(inFile, line)) {
			fullContent.append(line);
			fullContent.append("\n");
		}
		inFile.close();
	}
	else {
		cerr << "Can not open " << "out.obj\n";
		string str("");
		return str;
	}

	return fullContent;
}

int main(int argc, char* argv[]) {
	CanTouchThis::ConversionClient client(CanTouchThis::LOCALHOST, CanTouchThis::PORT);
	vector<tuple<int, int, double> > hMap;
	std::tuple<int, int, double> point;

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {
			hMap.push_back(std::make_tuple(i, j, .0));
		}
	}

	std::this_thread::sleep_for(chrono::milliseconds(2000));
	while (client.getLocationData(point)) {
		if (get<2>(point) < 0.075) {
			get<2>(point) = 0;
		}
		int x = get<0>(point);
		int y = get<1>(point);
		hMap[x * 20 + y] = std::move(point);
		client.sendObjFile(convertHeightmap(hMap));
		++counter;
	}

	return 0;
}
