#pragma once

// Application settings/state - holds all configurable values
// This separates data from presentation (UI)

class Settings
{
public:
	// Rendering settings
	float windowColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	bool wireframeMode = false;
	bool m_AutoRotate = false;

	// Triangle settings
	// float triangleColor[3] = {1.0f, 1.0f, 1.0f};


};

