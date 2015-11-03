// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Alessandro Tasora
// =============================================================================
//
// Base class for a sprocket brake.
//
// =============================================================================

#ifndef CH_TRACK_BRAKE_H
#define CH_TRACK_BRAKE_H

#include <vector>

#include "chrono/core/ChShared.h"
#include "chrono/physics/ChLinkLock.h"

#include "chrono_vehicle/ChApiVehicle.h"

namespace chrono {
namespace vehicle {

///
/// Base class for a brake subsystem
///
class CH_VEHICLE_API ChTrackBrake : public ChShared {
  public:
    ChTrackBrake();
    virtual ~ChTrackBrake() {}

    /// Initialize the brake by providing the sprocket's revolute link.
    virtual void Initialize(ChSharedPtr<ChLinkLockRevolute> hub) = 0;

    /// Update the brake subsystem for the given braking driver input.
    ///   braking = 0 : completely free,
    ///   braking = 1 : provide maximum braking torque
    virtual void Update(double braking) = 0;

    /// Get the current brake torque.
    virtual double GetBrakeTorque() = 0;
};

/// Vector of handles to brake subsystems.
typedef std::vector<ChSharedPtr<ChTrackBrake> > ChTrackBrakeList;

}  // end namespace vehicle
}  // end namespace chrono

#endif
