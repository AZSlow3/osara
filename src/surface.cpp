/*
* OSARA: Open Source Accessibility for the REAPER Application
* Surface header
* Author: Alexey Zhelezov <ac@azslow.com>
* Copyright 2014-2018 NV Access Limited
* License: GNU General Public License version 2.0
*/

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "osara.h"

class Surface : public IReaperControlSurface {
public:
	static Surface *CreateInstance() {
		if (m_pInstance) {
			ShowMessageBox("OSARA surface is already configured", "Error", 0);
			return nullptr; // avoid more then one instance
		}
		return new Surface();
	}
	static BOOL IsActive() {
		return m_pInstance != nullptr;
	}
	~Surface() {
		m_pInstance = nullptr;
	}
	const char* GetTypeString() {
		return "OSARA";
	}
	const char* GetDescString() {
		return "OSARA";
	}
	const char* GetConfigString() {
		return "0"; // empty does not work, this can work as a version
	}
	void SetSurfaceMute(MediaTrack *trackid, bool mute) {
	}
private:
	Surface() {
		m_pInstance = this;
	}
	static Surface* m_pInstance; // singleton
};

Surface *Surface::m_pInstance = nullptr;


// registrations

static IReaperControlSurface *createFunc(const char *type_string, const char *configString, int *errStats) {
	return Surface::CreateInstance();
}

static HWND configFunc(const char *type_string, HWND parent, const char *initConfigString) {
	return nullptr;
}

static reaper_csurf_reg_t csurf_osara_reg = {
	"OSARA",
	"OSARA",
	createFunc,
	configFunc
};

// exposed functions

void Surface_Register(reaper_plugin_info_t *rec) {
	if (!rec)
		return;
	rec->Register("csurf", &csurf_osara_reg);
}

BOOL Surface_IsActive() {
	return Surface::IsActive();
}