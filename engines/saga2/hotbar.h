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

#ifndef SAGA2_HOTBAR_H
#define SAGA2_HOTBAR_H

#include "saga2/objects.h"
#include "saga2/panel.h"

namespace Saga2 {

constexpr int kNumCharacters = 3;
constexpr int kMaxHotbarSlots = 9;

class HotbarState {
public:
	struct Slot {
		ObjectID item;
		ObjectID proto;
	};

private:
	Slot _slots[kNumCharacters][kMaxHotbarSlots];

public:
	HotbarState();

	void assign(int characterIdx, int slotIdx, ObjectID obj);
	void clear(int characterIdx, int slotIdx);
	bool isPresent(int characterIdx, int slotIdx) const;
	void validate(int characterIdx);
};

class HotbarPanel : public gControl {
public:
	HotbarPanel(gPanelList &list, const Rect16 &box);
	virtual ~HotbarPanel();

	virtual void draw() override;
};

} // End of namespace Saga2

#endif
