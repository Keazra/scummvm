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
#include "saga2/motion.h"
#include "saga2/actor.h"

namespace Saga2 {

/* ===================================================================== *
   Prototypes
 * ===================================================================== */

void updateTasks(TaskList &list);

/* ===================================================================== *
   MotionTask member functions
 * ===================================================================== */

//-----------------------------------------------------------------------
//	Constructor

MotionTask::MotionTask(Actor &a) : _actor(a) {
	_type = kMotionNone;
	_motionFlags = 0;
	_uSpeed = _vSpeed = _zSpeed = 0;
	_uFrac = _vFrac = _zFrac = 0;
	_uDist = _vDist = _zDist = 0;
	_dir = 0;
	_runCount = 0;

	//  Insert task into the list
	g_vm->_mTaskList->insert(this);
}

//-----------------------------------------------------------------------
//	Destructor

MotionTask::~MotionTask() {
	if (_actor._moveTask == this)
		_actor._moveTask = nullptr;

	//  Remove task from the list
	g_vm->_mTaskList->remove(this);
}

//-----------------------------------------------------------------------
//	Perform setup for walking to a specific location

void MotionTask::walkTo(Actor &a, const TilePoint &dest, bool run, bool wait) {
	MotionTask      *mt;

	if (a._moveTask) {
		a._moveTask->abort();
		delete a._moveTask;
	}

	mt = new MotionTask(a);
	mt->_type = kMotionWalk;
	mt->_uDest = dest.u;
	mt->_vDest = dest.v;
	mt->_zDest = dest.z;

	if (run) mt->_motionFlags |= kMTRun;
	if (wait) mt->_motionFlags |= kMTWait;

	a._moveTask = mt;
}

//-----------------------------------------------------------------------
//	Perform setup for walking to a specific location in a straight line

void MotionTask::walkToDirect(Actor &a, const TilePoint &dest, bool run, bool wait) {
	MotionTask      *mt;

	if (a._moveTask) {
		a._moveTask->abort();
		delete a._moveTask;
	}

	mt = new MotionTask(a);
	mt->_type = kMotionWalkDirect;
	mt->_uDest = dest.u;
	mt->_vDest = dest.v;
	mt->_zDest = dest.z;

	if (run) mt->_motionFlags |= kMTRun;
	if (wait) mt->_motionFlags |= kMTWait;

	a._moveTask = mt;
}

//-----------------------------------------------------------------------
//	Perform setup for following another object

void MotionTask::follow(Actor &a, GameObject &target, bool run, bool wait) {
	MotionTask      *mt;

	if (a._moveTask) {
		a._moveTask->abort();
		delete a._moveTask;
	}

	mt = new MotionTask(a);
	mt->_type = kMotionFollow;
	mt->_targetID = target.thisID();

	if (run) mt->_motionFlags |= kMTRun;
	if (wait) mt->_motionFlags |= kMTWait;

	a._moveTask = mt;
}

//-----------------------------------------------------------------------
//	Perform setup for facing a specific direction

void MotionTask::face(Actor &a, int16 direction, bool wait) {
	MotionTask      *mt;

	if (a._moveTask) {
		a._moveTask->abort();
		delete a._moveTask;
	}

	mt = new MotionTask(a);
	mt->_type = kMotionFace;
	mt->_dir = direction;

	if (wait) mt->_motionFlags |= kMTWait;

	a._moveTask = mt;
}

//-----------------------------------------------------------------------
//	Perform setup for waiting

void MotionTask::wait(Actor &a) {
	if (a._moveTask) {
		a._moveTask->abort();
		delete a._moveTask;
	}
}

//-----------------------------------------------------------------------
//	Change the target of a walk task

void MotionTask::changeTarget(const TilePoint &dest) {
	assert(_type == kMotionWalk);

	_uDest = dest.u;
	_vDest = dest.v;
	_zDest = dest.z;
}

//-----------------------------------------------------------------------
//	Change the target of a walk direct task

void MotionTask::changeDirectTarget(const TilePoint &dest, bool run) {
	assert(_type == kMotionWalkDirect);

	_uDest = dest.u;
	_vDest = dest.v;
	_zDest = dest.z;

	if (run) _motionFlags |= kMTRun;
	else _motionFlags &= ~kMTRun;
}

//-----------------------------------------------------------------------
//	Perform abortion of the motion task

void MotionTask::abort() {
	if (_type == kMotionWalk)
		abortWalk();
	_type = kMotionNone;
}

//-----------------------------------------------------------------------
//	Finish the walk task (walk to current tile center)

void MotionTask::finishWalk() {
	assert(_type == kMotionWalk);

	_uDest = (_actor._location.u & ~31) + 16;
	_vDest = (_actor._location.v & ~31) + 16;
}

//-----------------------------------------------------------------------
//	Write the state of the task to a stream

void MotionTask::write(Common::MemoryWriteStreamDynamic *out) const {
	out->writeUint16LE(_type);
	out->writeUint16LE(_motionFlags);
	out->writeInt32LE(_uDest);
	out->writeInt32LE(_vDest);
	out->writeInt32LE(_zDest);
	out->writeUint32LE(_targetID);
	out->writeInt16LE(_dir);
	out->writeInt32LE(_runCount);
}

//-----------------------------------------------------------------------
//	Read the state of the task from a stream

void MotionTask::read(Common::InSaveFile *in) {
	_type = (MotionType)in->readUint16LE();
	_motionFlags = in->readUint16LE();
	_uDest = in->readInt32LE();
	_vDest = in->readInt32LE();
	_zDest = in->readInt32LE();
	_targetID = in->readUint32LE();
	_dir = in->readInt16LE();
	_runCount = in->readInt32LE();
}

/* ===================================================================== *
   MotionTaskList member functions
 * ===================================================================== */

//-----------------------------------------------------------------------
//	Constructor

MotionTaskList::MotionTaskList() : TaskList() {
}

//-----------------------------------------------------------------------
//	Update all tasks in the list

void MotionTaskList::update() {
	updateTasks(*this);
}

} // End of namespace Saga2
