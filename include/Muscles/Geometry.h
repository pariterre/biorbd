#ifndef S2M_MUSCLE_GEOMETRY_H
#define S2M_MUSCLE_GEOMETRY_H

#include "biorbdConfig.h"
#include "s2mJoints.h"
#include "Utils/Matrix.h"
#include "Utils/GenCoord.h"
#include "Muscles/MuscleNode.h"
#include "Muscles/PathChangers.h"
#include "Muscles/Caracteristics.h"

class BIORBD_API s2mMuscleGeometry
{
public:
    s2mMuscleGeometry(
            const s2mNodeMuscle &origin = s2mNodeMuscle(),
            const s2mNodeMuscle &insertion = s2mNodeMuscle());
    virtual ~s2mMuscleGeometry();

    // Fonction a appeler avant d'appeler longueur/velocity ou autres!
    void updateKinematics(
            s2mJoints &model,
            const biorbd::utils::GenCoord* Q = nullptr,
            const biorbd::utils::GenCoord* Qdot = nullptr,
            const s2mMuscleCaracteristics& = s2mMuscleCaracteristics(),
            const s2mMusclePathChangers& = s2mMusclePathChangers(),
            int updateKin = 2);
    void updateKinematics(
            std::vector<s2mNodeMuscle>& musclePointsInGlobal,
            biorbd::utils::Matrix& jacoPointsInGlobal,
            const biorbd::utils::GenCoord* Qdot = nullptr,
            const s2mMuscleCaracteristics& = s2mMuscleCaracteristics());

    // Get and set des position d'origine et insertions
    const s2mNodeMuscle& originInLocal() const;
    const s2mNodeMuscle& insertionInLocal() const;
    void originInLocal(const s2mNodeMuscle &val);
    void insertionInLocal(const s2mNodeMuscle &val);

    // Position des muscles dans l'espace
    const s2mNodeMuscle& originInGlobal() const; // Attention il est impératif d'avoir update une premiere fois pour que ceci fonctionne!
    const s2mNodeMuscle& insertionInGlobal() const; // Attention il est impératif d'avoir update une premiere fois pour que ceci fonctionne!
    const std::vector<s2mNodeMuscle>& musclesPointsInGlobal() const; // Return already computed via points

    // Retour des longueur et vitesse musculaires
    double length() const; // Return the already computed muscle length
    double musculoTendonLength() const; // Return the already computed muscle-tendon length
    double velocity() const; // Return the already computed velocity

    // Retour des jacobiennes
    const biorbd::utils::Matrix& jacobian() const; // Retourne la derniere jacobienne
    biorbd::utils::Matrix jacobianOrigin() const;
    biorbd::utils::Matrix jacobianInsertion() const ;
    biorbd::utils::Matrix jacobian(const unsigned int i) const;
    const biorbd::utils::Matrix& jacobianLength() const;


protected:
    // Update commun de la cinématique
    void _updateKinematics(
            const biorbd::utils::GenCoord *Qdot,
            const s2mMuscleCaracteristics &c,
            const s2mMusclePathChangers* o = nullptr);

    // Calcul de la position des points dans le global
    const s2mNodeMuscle& originInGlobal(
            s2mJoints &model,
            const biorbd::utils::GenCoord &Q); // Update la cinématique puis retourne la position de l'origine dans l'espace
    const s2mNodeMuscle& insertionInGlobal(
            s2mJoints &model,
            const biorbd::utils::GenCoord &Q); // Update la cinématique puis retourne la position de l'insertion dans l'espace
    void musclesPointsInGlobal(std::vector<s2mNodeMuscle>& ptsInGlobal); // Forcer les points dans le global
    void musclesPointsInGlobal(
            s2mJoints &,
            const biorbd::utils::GenCoord &Q,
            const s2mMusclePathChangers&);

    // Calcul de la longueur musculaire
    double length(
            const s2mMuscleCaracteristics&,
            const s2mMusclePathChangers* pathChanger = nullptr); // Update the kinematics and compute and return muscle length
    // Calcul de la vitesse musculaire
    double velocity(
            const biorbd::utils::GenCoord &Qdot); // Update the kinematics and compute and return muscle velocity assuming no via points nor wrapping objects
    // Calcul des jacobiennes des points
    void setJacobianDimension(s2mJoints &model);
    void jacobian(const biorbd::utils::Matrix &jaco); // Forcer une jacobienne
    void jacobian(
            s2mJoints &model,
            const biorbd::utils::GenCoord &Q);
    void computeJacobianLength();

    // Position des nodes dans le repere local
    s2mNodeMuscle m_origin; // Node de l'origine
    s2mNodeMuscle m_insertion; // Node de l'insertion

    s2mNodeMuscle m_originInGlobal; // Position de l'origine dans le repere global
    s2mNodeMuscle m_insertionInGlobal; // Position de l'origine dans le repere global
    std::vector<s2mNodeMuscle> m_pointsInGlobal; // position de tous les points dans le global
    std::vector<s2mNodeMuscle> m_pointsInLocal; // position de tous les points dans le global
    biorbd::utils::Matrix m_jacobian;
    biorbd::utils::Matrix m_G;
    biorbd::utils::Matrix m_jacobianLength; // Incluant la

    double m_length; // Longueur du muscle
    double m_muscleTendonLength; // Longueur du muscle et du tendon
    double m_velocity; // Vitesse de l'élongation musculaire

    bool m_isGeometryComputed; // Savoir si la geometry a été faite au moins une fois
    bool m_isVelocityComputed; // Savoir si dans le dernier update, la vitesse a été calculée
    bool m_posAndJacoWereForced; // On a utilisé la procédure d'override de la position du muscle et de la jacobienne

};

#endif // S2M_MUSCLE_GEOMETRY_H