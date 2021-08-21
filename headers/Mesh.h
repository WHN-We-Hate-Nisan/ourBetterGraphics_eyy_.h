#pragma once
#include"Essentials.h"
#include "Triangle.h"

struct Mesh {
	std::vector<Triangle> triangles;
	typedef std::vector<Vec3> VertSet;
	struct Vertex
	{
		Vec3 position;
		Vec2 texcoord;
		Vec3 normal;
	};
	struct VertRef
	{
		VertRef(int v, int vt, int vn) : v(v), vt(vt), vn(vn) { }
		int v, vt, vn;
	};

	//Make this boi load colors
	bool LoadFromObjectFile(std::string FileName) {
		std::ifstream f(FileName);
		if (!f.is_open()) return false;

		std::vector<Vertex> verts;
		//Vertices
		VertSet vertices(1, { 0,0,0,1 });
		std::vector<Vec2> texcoords(1, { 0,0,0 });
		VertSet normals(1, { 0,0,0,1 });

		while (!f.eof()) {
			std::string line;
			std::getline(f, line);
			std::istringstream lineSS(line);
			std::string lineType;
			lineSS >> lineType;

			//Getting vertices and making a pool
			if (lineType == "v")
			{
				float x = 0, y = 0, z = 0, w = 1;
				lineSS >> x >> y >> z >> w;
				vertices.emplace_back(Vec3{ x, y, z, w });
			}

			//Getting textures and making a pool
			if (lineType == "vt")
			{
				float u = 0, v = 0, w = 0;
				lineSS >> u >> v >> w;
				texcoords.emplace_back(Vec2{ u, v, w });
			}

			//Getting pramissnormal and making a pool
			if (lineType == "vn")
			{
				float i = 0, j = 0, k = 0;
				lineSS >> i >> j >> k;
				normals.emplace_back(Vec3{ i, j, k }.normalize());
			}

			if (lineType == "f")
			{
				std::vector<VertRef> refs;
				std::string refStr;
				while (lineSS >> refStr) {
					std::istringstream ref(refStr);
					std::string vStr, vtStr, vnStr;
					std::getline(ref, vStr, '/');
					std::getline(ref, vtStr, '/');
					std::getline(ref, vnStr, '/');
					int v = atoi(vStr.c_str());
					int vt = atoi(vtStr.c_str());
					int vn = atoi(vnStr.c_str());
					v = (v >= 0 ? v : vertices.size() + v);
					vt = (vt >= 0 ? vt : texcoords.size() + vt);
					vn = (vn >= 0 ? vn : normals.size() + vn);
					refs.emplace_back(VertRef(v, vt, vn));
				}

				// triangulate, assuming n>3-gons are convex and coplanar
				for (size_t i = 1; i + 1 < refs.size(); ++i) {
					const VertRef* p[3] = { &refs[0], &refs[i], &refs[i + 1] };

					Vec3 U = vertices[p[1]->v] - vertices[p[0]->v];
					Vec3 V = vertices[p[2]->v] - vertices[p[0]->v];
					Vec3 faceNormal = (U * V).normalize();

					for (size_t j = 0; j < 3; ++j) {
						Vertex vert;
						vert.position = vertices[p[j]->v];
						vert.texcoord = texcoords[p[j]->vt];
						vert.normal = (p[j]->vn != 0 ? normals[p[j]->vn] : faceNormal);
						verts.emplace_back(vert);
					}
				}
			}
		}

		//Getting triangles and making a pool
		consoleLogSpace(verts.size());
		for (int i = 0; i < verts.size() / 3; i++) {
			Triangle tri;
			for (int j = 0; j < 3; j++) {
				tri.vertex[j].position = verts[i * 3 + j].position;
				tri.vertex[j].textureCood = verts[i * 3 + j].texcoord;
				tri.vertex[j].normal = verts[i * 3 + j].normal;
			}
			triangles.emplace_back(tri);
		}
		return true;
	}
};


