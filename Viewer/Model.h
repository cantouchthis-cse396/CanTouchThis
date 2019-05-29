#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <fstream>

class Model {
public: 		
	void load_model(std::string model);
	
	void reset() {
		this->model_x = 0;
		this->model_y = 0;
		this->model_z = 0;
		this->model_rotx = 0;
		this->model_roty = 0;
		this->model_rotz = 0;
	}

	void clear() {
		model_x = 0;
		model_y = 0;
		model_z = 0;
		// model_rotx = 0;
		// model_roty = 0;
		// model_rotz = 0;
		vertex_list.clear();
		s_list.clear();
		mean_x = 0;
		mean_y = 0;
		mean_z = 0;
		max_x = 0;
		min_x = 0;
		max_y = 0;
		min_y = 0;
		max_z = 0;
		min_z = 0;
		max_scale = 0;
	}

private:
	void calculate_vertex() {
		unsigned int s = (this->vertex_list.size() / 3);
		this->mean_x = 0;
		this->mean_y = 0;
		this->mean_z = 0;

		for (unsigned int q = 0; q < s; q++){
			this->mean_x += this->vertex_list.at(3*q);
			this->mean_y += this->vertex_list.at(3*q + 1);
			this->mean_z += this->vertex_list.at(3*q + 2);
		}

		this->mean_x = this->mean_x / s;
		this->mean_y = this->mean_y / s;
		this->mean_z = this->mean_z / s;
	}

	void get_max_mins(){
		unsigned int s = this->vertex_list.size() / 3;

		this->max_x = this->vertex_list.at(0);
		for (unsigned int q = 0; q < s; q++) {
			if (this->vertex_list.at(3 * q) > this->max_x) {
				this->max_x = this->vertex_list.at(3 * q);
			}
		}

		this->min_x = this->vertex_list.at(0);
		for (unsigned int q = 0; q < s; q++) {
			if (this->vertex_list.at(3 * q) < this->min_x) {
				this->min_x = this->vertex_list.at(3 * q);
			}
		}

		this->max_y = this->vertex_list.at(1);
		for (unsigned int q = 0; q < s; q++) {
			if (this->vertex_list.at(3 * q + 1) > this->max_y) {
				this->max_y = this->vertex_list.at(3 * q + 1);
			}
		}

		this->min_y = this->vertex_list.at(1);
		for (unsigned int q = 0; q < s; q++) {
			if (this->vertex_list.at(3*q + 1) < this->min_y) {
				this->min_y = this->vertex_list.at(3 * q + 1);
			}
		}

		this->max_z = this->vertex_list.at(2);
		for (unsigned int q = 0; q < s; q++) {
			if (this->vertex_list.at(3*q + 2) > this->max_z) {
				this->max_z = this->vertex_list.at(3 * q + 2);
			}
		}

		this->min_z = this->vertex_list.at(2);
		for (unsigned int q = 0; q < s; q++) {
			if (this->vertex_list.at(3*q + 2) < this->min_z) {
				this->min_z = this->vertex_list.at(3 * q + 2);
			}
		}

		float scale_x = this->max_x - this->min_x;
		float scale_y = this->max_y - this->min_y;
		float scale_z = this->max_z - this->min_z;

		this->max_scale = scale_x;
		if (scale_y > this->max_scale)
			this->max_scale = scale_y;

		if (scale_z > this->max_scale)
			this->max_scale = scale_z;
	}

	void apply_transf_to_matrix(){
		unsigned int s = (this->vertex_list.size() / 3);
		this->calculate_vertex();
		this->get_max_mins();

		for (unsigned int h = 0; h < s; h++) {
			this->vertex_list.at(3 * h) = this->vertex_list.at(3 * h) - this->mean_x;
		}

		for (unsigned int h = 0; h < s; h++) {
			this->vertex_list.at(3 * h + 1) = this->vertex_list.at(3 * h + 1) - this->mean_y;
		}

		for (unsigned int h = 0; h < s; h++) {
			this->vertex_list.at(3 * h + 2) = this->vertex_list.at(3 * h + 2) - this->mean_z;
		}

		for (unsigned int h = 0; h < this->vertex_list.size() ; h++) {
			this->vertex_list.at(h) = this->vertex_list.at(h) / this->max_scale;
		}   

		for (unsigned int h = 0; h < s; h++) {
			this->vertex_list.at(3 * h + 2) -= 2;
		}
	}

public:
	float model_x;
	float model_y;
	float model_z;

	float model_rotx;
	float model_roty;
	float model_rotz;

private:
	std::vector<float> vertex_list;
	std::vector<std::string> s_list;
	float mean_x;
	float mean_y;
	float mean_z;

	float max_x;
	float min_x;
	float max_y;
	float min_y;
	float max_z;
	float min_z;

	float max_scale;
};

#endif
