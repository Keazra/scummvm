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
 *
 * Based on the original sources
 *   Faery Tale II -- The Halls of the Dead
 *   (c) 1993-1996 The Wyrmkeep Entertainment Co.
 */

#include "saga2/saga2.h"
#include "saga2/idtypes.h"
#include "saga2/objects.h"
#include "saga2/actor.h"
#include "saga2/contain.h"
#include "saga2/intrface.h"
#include "saga2/mouseimg.h"
#include "saga2/tile.h"

namespace Saga2 {

/* ===================================================================== *
   GameObject member functions
 * ===================================================================== */

//-----------------------------------------------------------------------
//	Determine name of object for cursor text

void GameObject::objCursorText(char *buf, int buflen) {
	Common::strlcpy(buf, name(), buflen);

	// if this is armor, show its defense
	if (isArmor()) {
		ArmorProto *proto = (ArmorProto *)this->proto();
		Common::strlcat(buf, " (", buflen);
		char defenseStr[16];
		snprintf(defenseStr, sizeof(defenseStr), "Def: %d", proto->_defense);
		Common::strlcat(buf, defenseStr, buflen);
		Common::strlcat(buf, ")", buflen);
	}
	// if this is a weapon, show damage stats and type
	else if (isWeapon()) {
		weaponID id = this->getWeaponID();
		WeaponStuff &ws = getWeapon(id);
		int8 dice, sides, base;
		effectDamageTypes type;

		if (ws.getPrimaryDamageStats(dice, sides, base, type)) {
			Common::strlcat(buf, " (", buflen);
			char damageStr[32];
			snprintf(damageStr, sizeof(damageStr), "%dd%d+%d ", dice, sides, base);
			Common::strlcat(buf, damageStr, buflen);

			const char *typeName = "Unknown";
			switch (type) {
			case kDamageImpact:     typeName = "Impact"; break;
			case kDamageSlash:      typeName = "Slash"; break;
			case kDamagePierce:     typeName = "Pierce"; break;
			case kDamageFire:       typeName = "Fire"; break;
			case kDamageCold:       typeName = "Cold"; break;
			case kDamageAcid:       typeName = "Acid"; break;
			case kDamageElectrical: typeName = "Elec"; break;
			case kDamagePoison:     typeName = "Poison"; break;
			case kDamageMagic:      typeName = "Magic"; break;
			default: break;
			}
			Common::strlcat(buf, typeName, buflen);
			Common::strlcat(buf, ")", buflen);
		}
	}
}

//-----------------------------------------------------------------------
//	Place object in the world or in a container

void GameObject::placeObject(const Location &loc, ObjectID mergeObjID) {
	//  If we are to merge this object with another object, do it now
	if (mergeObjID != Nothing) {
		GameObject *mergeObj = GameObject::objectAddress(mergeObjID);
		assert(mergeObj != nullptr);

		if (mergeObj->canMerge(this)) {
			mergeObj->merge(this);
			return;
		}
	}

	//  If this object is currently in a container, remove it
	if (isMemberOf(Nothing))
		removeFromWorld();
	else
		removeFromContainer();

	//  Place this object in its new home
	if (loc._context == Nothing)
		addToWorld(loc);
	else
		addToContainer(loc);
}

//-----------------------------------------------------------------------
//	Determine if object is carryable

bool GameObject::isCarryable() const {
	return (proto()->flags & ResourceObjectPrototype::kObjPropCarryable) != 0;
}

//-----------------------------------------------------------------------
//	Determine if object is armor

bool GameObject::isArmor() const {
	return (proto()->flags & ResourceObjectPrototype::kObjPropArmor) != 0;
}

//-----------------------------------------------------------------------
//	Determine if object is a weapon

bool GameObject::isWeapon() const {
	return (proto()->flags & ResourceObjectPrototype::kObjPropWeapon) != 0;
}

//-----------------------------------------------------------------------
//	Determine weapon ID of object

weaponID GameObject::getWeaponID() const {
	assert(isWeapon());
	return (weaponID)getExtra();
}

//-----------------------------------------------------------------------
//	Determine if object is obscured by something

bool GameObject::isObscured() const {
	//  For now, just return false
	return false;
}

//-----------------------------------------------------------------------
//	Determine if object's roof has been ripped

bool objRoofRipped(GameObject *obj) {
	//  For now, just return false
	return false;
}

} // End of namespace Saga2
