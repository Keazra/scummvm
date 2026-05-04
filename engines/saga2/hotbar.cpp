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

#include "saga2/hotbar.h"
#include "common/system.h"
#include "common/str.h"
#include "saga2/saga2.h"

namespace Saga2 {

// Hotbar State Implementation

HotbarState::HotbarState() {
	for (int charIdx = 0; charIdx < kNumCharacters; ++charIdx) {
		for (int slotIdx = 0; slotIdx < kMaxHotbarSlots; ++slotIdx) {
			_slots[charIdx][slotIdx].item = Nothing;
			_slots[charIdx][slotIdx].proto = 0;
		}
	}
}

void HotbarState::assign(int characterIdx, int slotIdx, ObjectID obj) {
	// Stub
}

void HotbarState::clear(int characterIdx, int slotIdx) {
	// Stub
}

bool HotbarState::isPresent(int characterIdx, int slotIdx) const {
	return false; // Stub
}

void HotbarState::validate(int characterIdx) {
	// Stub
}

// Hotbar Panel Implementation

constexpr int kHotbarSlotWidth = 64;
constexpr int kHotbarSlotHeight = 36;
constexpr int kHotbarPadding = 4;
constexpr int kHotbarDimColorIndex = 10; // Placeholder index for dim border

const char *const slotLabels[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

HotbarPanel::HotbarPanel(gPanelList &list, const Rect16 &box) : gControl(list, box, nullptr, 0) {
}

HotbarPanel::~HotbarPanel() {
}

void HotbarPanel::draw() {
	gPort &port = _window._windowPort;
	
	int startX = _extent.x + kHotbarPadding;
	int startY = _extent.y + 2;
	
	for (int i = 0; i < kMaxHotbarSlots; ++i) {
		Rect16 slotRect(startX + (kHotbarSlotWidth + kHotbarPadding) * i, startY, kHotbarSlotWidth, kHotbarSlotHeight);
		
		// Draw empty frame (dim border)
		port.frameRect(slotRect, kHotbarDimColorIndex);
		
		// Draw number label
		port.moveTo(slotRect.x + 2, slotRect.y + 2);
		port.drawText(slotLabels[i], 1);
	}
}

} // End of namespace Saga2
