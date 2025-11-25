#pragma once

#include <fstream>

#include "3dvec.h"
#include "intersectable.h"
#include "triangle.h"


class TriangleMesh {
public:
    std::vector<shared_ptr<intersectable>> triangles;

    // Constructor loads a PLY file and creates triangle objects
    TriangleMesh(const std::string& filename, shared_ptr<material> mat) {
        loadPLY(filename, mat);
    }

private:
    void loadPLY(const std::string& filename, shared_ptr<material> mat) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening PLY file: " << filename << "\n";
            return;
        }

        std::vector<point3d> vertices;
        std::string line;
        bool header_done = false;
        int vertex_count = 0, face_count = 0;

        // Read header
        while (std::getline(file, line)) {
            if (line.substr(0, 14) == "element vertex") {
                vertex_count = std::stoi(line.substr(15));
            } else if (line.substr(0, 12) == "element face") {
                face_count = std::stoi(line.substr(13));
            } else if (line == "end_header") {
                header_done = true;
                break;
            }
        }

        if (!header_done) {
            std::cerr << "Invalid PLY file: missing end_header\n";
            return;
        }

		double scale = filename.find("dragon")!=std::string::npos ? 1000 : 2000;

        // Read vertices
        for (int i = 0; i < vertex_count; ++i) {
            double x, y, z;
            file >> x >> y >> z;

			if (filename.find("dragon")==std::string::npos) {
				std::getline(file >> std::ws, line);
			}

            vertices.push_back(point3d(scale*x, scale*y, scale*z));
        }

        // Read faces
        for (int i = 0; i < face_count; ++i) {
            int n, a, b, c;
            file >> n >> a >> b >> c;
            if (n != 3) continue; // skip non-triangle faces
            triangles.push_back(make_shared<triangle>(vertices[a], vertices[b], vertices[c], mat));
        }
    }
};
