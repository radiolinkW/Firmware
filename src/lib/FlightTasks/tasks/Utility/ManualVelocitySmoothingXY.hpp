/****************************************************************************
 *
 *   Copyright (c) 2019 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file ManualVelocitySmoothingXY.hpp
 *
 */

#pragma once

#include "VelocitySmoothing.hpp"

#include <matrix/matrix/math.hpp>

using matrix::Vector2f;

class ManualVelocitySmoothingXY
{
public:
	ManualVelocitySmoothingXY() = default;

	virtual ~ManualVelocitySmoothingXY() = default;

	void reset(Vector2f accel, Vector2f vel, Vector2f pos);
	void update(float dt, Vector2f velocity_target);

	void setVelSpFeedback(const Vector2f fb) { _velocity_setpoint_feedback = fb; }

	void setMaxJerk(const float max_jerk) {
		_smoothing[0].setMaxJerk(max_jerk);
		_smoothing[1].setMaxJerk(max_jerk);
	}
	float getMaxJerk() const { return _smoothing[0].getMaxJerk(); }

	void setMaxAccel(const float max_accel) {
		_smoothing[0].setMaxAccel(max_accel);
		_smoothing[1].setMaxAccel(max_accel);
	}
	float getMaxAccel() const { return _smoothing[0].getMaxAccel(); }
	void setMaxVel(const float max_vel) {
		_smoothing[0].setMaxVel(max_vel);
		_smoothing[1].setMaxVel(max_vel);
	}
	float getMaxVel() const { return _smoothing[0].getMaxVel(); }

	Vector2f getCurrentJerk() const { return _state.j; }
	Vector2f getCurrentAcceleration() const { return _state.a; }
	void setCurrentVelocity(const Vector2f vel) {
		_state.v = vel;
		_smoothing[0].setCurrentVelocity(vel(0));
		_smoothing[1].setCurrentVelocity(vel(1));
	}
	Vector2f getCurrentVelocity() const { return _state.v; }
	void setCurrentPosition(const Vector2f pos) {
		_state.x = pos;
		_smoothing[0].setCurrentPosition(pos(0));
		_smoothing[1].setCurrentPosition(pos(1));
	}
	Vector2f getCurrentPosition() const { return _position_setpoint_locked; }
	void setCurrentPositionEstimate(Vector2f pos) { _position_estimate = pos; }

private:
	void resetPositionLock();
	void updateTrajectories(float dt);
	void updateTrajConstraints();
	void checkPositionLock(Vector2f velocity_target);
	void updateTrajDurations(Vector2f velocity_target);

	VelocitySmoothing _smoothing[2]; ///< Smoothing in x and y directions

	bool _position_lock_active{false};

	Vector2f _position_setpoint_locked;

	Vector2f _velocity_setpoint_feedback;
	Vector2f _position_estimate;

	struct {
		Vector2f j;
		Vector2f a;
		Vector2f v;
		Vector2f x;
	} _state;
};
