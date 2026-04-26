/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "saga2/saga2.h"
#include "saga2/detection.h"
#include "saga2/console.h"
#include "saga2/gfx.h"
#include "saga2/pal.h"
#include "saga2/actor.h"
#include "saga2/calendar.h"
#include "saga2/tilemode.h"
#include "saga2/contain.h"
#include "saga2/grabinfo.h"
#include "saga2/task.h"
#include "saga2/timers.h"
#include "saga2/sensor.h"
#include "saga2/audio.h"
#include "saga2/dispnode.h"
#include "saga2/mouseimg.h"
#include "saga2/oncall.h"
#include "saga2/uidialog.h"

namespace Saga2 {

Saga2Engine::Saga2Engine(OSystem *syst, const SAGA2GameDescription *desc) : Engine(syst), _gameDescription(desc) {
	g_vm = this;

	_rnd = new Common::RandomSource("saga2");
	_console = new Console();
	_renderer = new Renderer();
	_audio = nullptr;
	_pal = new PaletteManager();
	_act = new ActorManager();
	_calendar = new CalendarTime();
	_tmm = new TileModeManager();
	_cnm = new ContainerManager();

	_grandMasterFTA = nullptr;
	_frate = nullptr;
	_lrate = nullptr;

	_gameRunning = false;
	_autoAggression = false;
	_autoWeapon = false;
	_showNight = true;
	_speechText = true;
	_speechVoice = true;

	_teleportOnClick = false;
	_teleportOnMap = false;
	_showPosition = false;
	_showStats = false;
	_showStatusMsg = false;

	_indivControlsFlag = false;
	_userControlsSetup = false;
	_classicDblClick = false;
	_fadeDepth = 0;
	_currentMapNum = 0;

	_autosaveSlotIndex = 0;
	_autosaveAlarm.set(5 * 60 * kTicksPerSecond); // 5 minutes default
}

Saga2Engine::~Saga2Engine() {
	delete _rnd;
	delete _console;
	delete _renderer;
	delete _pal;
	delete _act;
	delete _calendar;
	delete _tmm;
	delete _cnm;
}

Common::Error Saga2Engine::run() {
	initGraphics(640, 480);

	_audio = audioInterfaceInit();

	loadExeResources();

	_pointer = new gMousePointer();
	_pointer->setPanel(&_mainPort);

	_mouseInfo = new GrabInfo();
	_mainDisplayList = new DisplayNodeList();

	initTileBanks();
	initTileModeState();

	_gameRunning = true;

	GameMode::set(&TileMode);

	while (!shouldQuit()) {
		GameMode::update();
		_pointer->update();
		_system->updateScreen();
		_system->delayMillis(10);
	}

	_gameRunning = false;

	freeAllTileBanks();
	freeExeResources();

	delete _mainDisplayList;
	delete _mouseInfo;
	delete _pointer;

	audioInterfaceExit(_audio);

	return Common::kNoError;
}

// ... other engine methods ...

} // End of namespace Saga2
