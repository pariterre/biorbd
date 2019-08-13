#ifndef BIORBD_UTILS_ATTITUDE_H
#define BIORBD_UTILS_ATTITUDE_H

#include <vector>
#include <Eigen/Dense>
#include <rbdl/rbdl_math.h>
#include "biorbdConfig.h"

namespace biorbd { namespace utils {
class Node;
class String;

class BIORBD_API Attitude : public Eigen::Matrix4d
{
public:
    Attitude(const Eigen::Matrix4d& = Eigen::Matrix4d::Identity());
    Attitude(
            const Eigen::VectorXd&,
            const Eigen::Vector3d&,
            const biorbd::utils::String&);
    Attitude(
            const Eigen::Matrix3d& rot,
            const Eigen::Vector3d& trans = Eigen::Vector3d::Zero());
    Attitude(const RigidBodyDynamics::Math::SpatialTransform&);

    Eigen::Vector3d axe(int); // Aller récupérer un axe en particulier

    Attitude transpose() const;
    Eigen::Vector3d trans() const;
    Eigen::Matrix3d rot() const;
    void setIdentity();
    bool isIdentity();

    static Attitude SpatialTransform2Attitude(const RigidBodyDynamics::Math::SpatialTransform& st);
    static Attitude combineRotAndTrans(
            const Eigen::Matrix3d& rot,
            const Eigen::Vector3d& trans);
    static Eigen::Matrix4d transformCardanToMatrix(
            const Eigen::VectorXd&,
            const Eigen::Vector3d&,
            const biorbd::utils::String&);
    static Eigen::VectorXd transformMatrixToCardan(
            const Attitude&,
            const biorbd::utils::String &seq);

    const Attitude operator*(const Attitude&);
    const Eigen::Vector3d operator*(const Eigen::Vector3d&);
    const Eigen::Vector3d operator*(const Eigen::Vector4d&);
    const biorbd::utils::Node operator*(const biorbd::utils::Node&);

    static Attitude mean(const std::vector<Attitude>&); // Moyenne des matrices 4x4
protected:
    Eigen::Vector4d expand3dTo4d(const Eigen::Vector3d&);

};
std::ostream& operator<<(std::ostream& os, const Attitude &a);

}}

#endif // BIORBD_UTILS_ATTITUDE_H