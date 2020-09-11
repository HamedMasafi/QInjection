#include "poolprivate.h"
namespace Dependency {

Pool *PoolPrivate::instance = nullptr;

PoolPrivate::PoolPrivate(Dependency::Pool *parent) : q_ptr(parent)
{}

}
