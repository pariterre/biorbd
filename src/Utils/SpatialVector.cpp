#define BIORBD_API_EXPORTS
#include "Utils/SpatialVector.h"

#include "Utils/Error.h"
#include "Utils/String.h"
#include "Utils/Vector3d.h"

using namespace BIORBD_NAMESPACE;

utils::SpatialVector::SpatialVector() :
    RigidBodyDynamics::Math::SpatialVector()
{

}

utils::SpatialVector::SpatialVector(
    const utils::SpatialVector& other) :
    RigidBodyDynamics::Math::SpatialVector (other)
{

}

utils::SpatialVector::SpatialVector(
    const RigidBodyDynamics::Math::SpatialVector& other) :
    RigidBodyDynamics::Math::SpatialVector (other)
{

}

utils::SpatialVector::SpatialVector(
    const utils::Vector3d& moment,
    const utils::Vector3d& force) : 
    RigidBodyDynamics::Math::SpatialVector(moment(0), moment(1), moment(2), force(0), force(1), force(2))
{

}

utils::SpatialVector::SpatialVector(
    Scalar mx, Scalar my, Scalar mz,
    Scalar fx, Scalar fy, Scalar fz) :
    RigidBodyDynamics::Math::SpatialVector (mx, my, mz, fx, fy, fz)
{

}

#ifdef BIORBD_USE_CASADI_MATH

utils::SpatialVector::SpatialVector(
    const casadi::MX &v) :
    RigidBodyDynamics::Math::SpatialVector(v)
{

}

utils::SpatialVector::SpatialVector(
    const RBDLCasadiMath::MX_Xd_SubMatrix &m) :
    RigidBodyDynamics::Math::SpatialVector (m)
{

}

#endif

void utils::SpatialVector::operator=(
    const utils::SpatialVector &other)
{
    this->RigidBodyDynamics::Math::SpatialVector::operator=(other);
}

#ifdef BIORBD_USE_CASADI_MATH

void utils::SpatialVector::operator=(
    const RBDLCasadiMath::MX_Xd_SubMatrix& other)
{
    this->RigidBodyDynamics::Math::SpatialVector::operator=(other);
}

void utils::SpatialVector::operator=(
    const casadi::MX &other)
{
    this->RigidBodyDynamics::Math::SpatialVector::operator=(other);
}

#endif

utils::Vector3d utils::SpatialVector::force() const {
    return utils::Vector3d((*this)(3), (*this)(4), (*this)(5));
}

utils::Vector3d utils::SpatialVector::moment() const {
    return utils::Vector3d((*this)(0), (*this)(1), (*this)(2));
}
