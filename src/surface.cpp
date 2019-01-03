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

// from reaper_osara.cpp
void postGoToTrack(int command);
bool isTrackMuted(MediaTrack* track);

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

	void SetTrackListChange() {
		// TODO: should we remember all current states here?
	}

	void SetSurfaceMute(MediaTrack *trackid, bool mute) {
		m_ChangeType = TRACK_MUTED;
		m_pChangeTrack = trackid;
	}

	void OnTrackSelection(MediaTrack *trackid) {
		m_ChangeType = TRACK_TOUCHED;
		m_pChangeTrack = trackid;
	}

	void Run() {
		switch (m_ChangeType) {
			case TRACK_TOUCHED:
				postGoToTrack(0);
				break;
			case TRACK_MUTED:
				outputMessage(isTrackMuted(m_pChangeTrack) ? "muted" : "unmuted");
				break;
			default:
				break;
				m_ChangeType = NO_CHANGES;
		}
		m_ChangeType = NO_CHANGES;
	}

	void OnTrackLastTouched(MediaTrack *trackid) {

	}

	int Extended(int call, void *parm1, void *parm2, void *parm3) {
		switch (call) {
		case CSURF_EXT_SETLASTTOUCHEDTRACK:
			OnTrackLastTouched(static_cast<MediaTrack *>(parm1));
			return 1;
		default:
			break;
		}
		return 0;
	}

private:
	static Surface* m_pInstance; // singleton

	enum CHANGE_TYPE {
		NO_CHANGES = 0,
		TRACK_TOUCHED,
		TRACK_MUTED,
	} m_ChangeType;
	MediaTrack* m_pChangeTrack;

	Surface() {
		m_pInstance = this;
		m_ChangeType = NO_CHANGES;
		m_pChangeTrack = nullptr;
	}
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