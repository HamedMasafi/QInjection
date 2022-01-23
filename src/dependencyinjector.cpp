#include "dependencyinjector.h"

Dependency::Injecter Inject;

Dependency::Injecter::Injecter() : _key{nullptr}
{}

Dependency::Injecter::Injecter(const char *key) : _key(key)
{}
