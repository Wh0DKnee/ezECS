#include "Component.h"
#include "Config.h"

BaseComponentMaskGetter::Id BaseComponentMaskGetter::next_id = 0;

ComponentManager::ComponentManager()
{
	positions.resize(INITIAL_POOL_SIZE);
	velocities.resize(INITIAL_POOL_SIZE);
}
