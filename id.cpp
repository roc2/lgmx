#include "id.h"


/**
 * Default constructor.
 */

Id::Id()
{
	id_ = 0;
}

/**
 * Default destructor.
 */

Id::~Id()
{
}

/**
 * Generates an unique source file ID.
 * @return unique file ID.
 */

unsigned int Id::generate_id()
{
	unsigned int res;

	if (!avail_id_.empty()) {
		res = avail_id_.top();
		avail_id_.pop();
		return res;
	}

	return id_++;
}

/**
 * Returns the ID to the available IDs pool.
 * @param id -> released ID.
 */

void Id::release_id(unsigned int id)
{
	avail_id_.push(id);
}

