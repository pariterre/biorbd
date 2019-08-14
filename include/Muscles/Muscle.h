#ifndef S2M_MUSCLE_H
#define S2M_MUSCLE_H

#include "biorbdConfig.h"
#include "Utils/String.h"
#include "Muscles/Compound.h"
#include "Muscles/Geometry.h"
#include "Muscles/Caracteristics.h"
#include "Muscles/StateDynamics.h"

class BIORBD_API s2mMuscle : public s2mMuscleCompound
{
public:
    s2mMuscle(
            const biorbd::utils::String& = "", // Nom du muscle
            const s2mMuscleGeometry& = s2mMuscleGeometry(), // Position origine/insertion
            const s2mMuscleCaracteristics& = s2mMuscleCaracteristics(), // Caractéristiques du muscle
            const s2mMusclePathChangers& = s2mMusclePathChangers(), // Set de wrapping objects
            const s2mMuscleStateDynamics& = s2mMuscleStateDynamics()); // Set d'un état actuel au départ
    s2mMuscle(const s2mMuscle& m);
    virtual ~s2mMuscle();

    // Get and set
    double length(
            s2mJoints&,
            const biorbd::utils::GenCoord&, int = 2);
    double musculoTendonLength(
            s2mJoints&,
            const biorbd::utils::GenCoord&, int = 2);
    double velocity(
            s2mJoints&,
            const biorbd::utils::GenCoord&,
            const biorbd::utils::GenCoord&,
            const bool = true);
    void updateOrientations(
            s2mJoints &m,
            const biorbd::utils::GenCoord &Q,
            int updateKin = 2); // Update de la position de ce muscle
    void updateOrientations(
            s2mJoints &m,
            const biorbd::utils::GenCoord &Q,
            const biorbd::utils::GenCoord &Qdot,
            int updateKin = 2); // Update de la position de ce muscle
    void updateOrientations(
            std::vector<s2mNodeMuscle>& musclePointsInGlobal,
            biorbd::utils::Matrix& jacoPointsInGlobal); // Update de la position de ce muscle
    void updateOrientations(
            std::vector<s2mNodeMuscle>& musclePointsInGlobal,
            biorbd::utils::Matrix& jacoPointsInGlobal,
            const biorbd::utils::GenCoord &Qdot); // Update de la position de ce muscle

    const s2mMuscleGeometry& position() const;
    const s2mMuscleCaracteristics& caract() const;
    void setPosition(const s2mMuscleGeometry &val);
    void setCaract(const s2mMuscleCaracteristics &val);
    const std::vector<s2mNodeMuscle>& musclesPointsInGlobal(
            s2mJoints &j,
            const biorbd::utils::GenCoord &Q,
            const bool updateKin = true);
    void forceIsoMax(double);

    // Get and set
    void setState(const s2mMuscleStateDynamics &s);
    const s2mMuscleStateDynamics& state() const;
    s2mMuscleStateDynamics& state_nonConst() const;
    double activationDot(const s2mMuscleStateDynamics &s, const bool =false);
protected:
    s2mMuscleGeometry m_position;
    s2mMuscleCaracteristics m_caract;
    s2mMuscleStateDynamics * m_state;

};

#endif // S2M_MUSCLE_H