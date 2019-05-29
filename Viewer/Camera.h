#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <string>
#include <fstream>

class Camera {
public: 	
	void reset() {
		this->camera_x = 0;
		this->camera_y = 0;
		this->camera_z = 0;
		this->camera_rotx = 0;
		this->camera_roty = 0;
		this->camera_rotz = 0;
	}
		
	float camera_x;
	float camera_y;
	float camera_z;

	float camera_rotx;
	float camera_roty;
	float camera_rotz;
};

#endif