#pragma once

#include "Shader.h"

class Block {
private:
	bool m_active;
	bool* m_sides;

public:
	Block()
		: m_active(false) 
	{
		m_sides = new bool[6];
		for (int i = 0; i < 6; i++) {
			m_sides[i] = false;
		}
	}
	~Block() {
		delete m_sides;
	}

	void SetSides(bool* s) {
		for (int i = 0; i < 6; i++) {
			m_sides[i] = s[i];
		}
	}
	void SetActive(bool a) {
		m_active = a;
	}
	bool IsActive() {
		return m_active;
	}

	void DrawBlock(Shader& ourShader) {
		ourShader.setVec4("texAtlasCoords", ourShader.CalcTexCoords(3, 0));

		ourShader.setFloat("brightness", 0.9f);
		if (m_sides[0]) {
			glDrawArrays(GL_TRIANGLES, 0, 6);	// front
		}
		if (m_sides[3]) {
			glDrawArrays(GL_TRIANGLES, 18, 6);	// right
		}

		ourShader.setFloat("brightness", 0.75f);
		if (m_sides[1]) {
			glDrawArrays(GL_TRIANGLES, 6, 6);	// back
		}
		if (m_sides[2]) {
			glDrawArrays(GL_TRIANGLES, 12, 6);	// left
		}
		if (m_sides[4]) {
			ourShader.setVec4("texAtlasCoords", ourShader.CalcTexCoords(2, 0));
			glDrawArrays(GL_TRIANGLES, 24, 6);	// bot
		}

		if (m_sides[5]) {
			ourShader.setFloat("brightness", 1.0f);
			ourShader.setVec4("texAtlasCoords", ourShader.CalcTexCoords(0, 0));
			glDrawArrays(GL_TRIANGLES, 30, 6);	// top
		}
	}
};