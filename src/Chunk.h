#pragma once

#include "Block.h"

class Chunk {
private:
	Block** m_arr;
	const unsigned int m_x, m_y, m_z;

public:
	Chunk(const unsigned int x_size, const unsigned int y_size, const unsigned int z_size)
		: m_x(x_size), m_y(y_size), m_z(z_size)
	{
		m_arr = new Block*[GetSize()];
		for (unsigned int i = 0; i < GetSize(); i++) {
			m_arr[i] = new Block();
		}
	}

	void Generate() {
		for (unsigned int i = 0; i < GetSize(); i++) {
			m_arr[i]->SetActive(false);
		}
		for (unsigned int x = 0; x < m_x; x++) {
			for (unsigned int y = 0; y < m_y; y++) {
				for (unsigned int z = 0; z < m_z; z++) {
					if (x * y * z % 10 == 0)
						GetBlockByCoord(x, y, z)->SetActive(true);
				}
			}
		}
	}
	Block* GetBlockByCoord(const unsigned int x, const unsigned int y, const unsigned int z) {
		return m_arr[x * m_y * m_z + y * m_z + z];
	}
	const unsigned int GetSize() {
		return m_x * m_y * m_z;
	}

	void CalculateSides() {
		bool* sides = new bool[6];
		for (unsigned int x = 0; x < m_x; x++) {
			for (unsigned int y = 0; y < m_y; y++) {
				for (unsigned int z = 0; z < m_z; z++) {

					if (z != 0)
						sides[0] = !GetBlockByCoord(x, y, z - 1)->IsActive();
					else
						sides[0] = true;

					if (z != m_z - 1)
						sides[1] = !GetBlockByCoord(x, y, z + 1)->IsActive();
					else
						sides[1] = true;

					if (x != 0)
						sides[2] = !GetBlockByCoord(x - 1, y, z)->IsActive();
					else 
						sides[2] = true;

					if (x != m_x - 1)
						sides[3] = !GetBlockByCoord(x + 1, y, z)->IsActive();
					else
						sides[3] = true;

					if (y != 0) 
						sides[4] = !GetBlockByCoord(x, y - 1, z)->IsActive();
					else
						sides[4] = true;

					if (y != m_y - 1)
						sides[5] = !GetBlockByCoord(x, y + 1, z)->IsActive();
					else
						sides[5] = true;

					GetBlockByCoord(x, y, z)->SetSides(sides);
				}
			}
		}
		delete[] sides;
	}
	void Render(Shader& ourShader) {
		for (unsigned int x = 0; x < m_x; x++) {
			for (unsigned int y = 0; y < m_y; y++) {
				for (unsigned int z = 0; z < m_z; z++) {
					Block* currBlock = GetBlockByCoord(x, y, z);
					if (currBlock->IsActive()) {

						glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float)x, (float)y, (float)z));
						ourShader.setMat4("model", model);

						currBlock->DrawBlock(ourShader);
					}
				}
			}
		}
	}
};